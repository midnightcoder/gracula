/*
 * PatternMatch.cc
 *
 * Copyright (C) 2009 Linas Vepstas
 *
 * Author: Linas Vepstas <linasvepstas@gmail.com>  January 2009
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "PatternMatch.h"
#include "DefaultPatternMatchCB.h"
#include "CrispLogicPMCB.h"

#include <opencog/atomspace/ClassServer.h>
#include <opencog/atomspace/SimpleTruthValue.h>
#include <opencog/atomspace/TLB.h>
#include <opencog/guile/SchemeEval.h>
#include <opencog/util/Logger.h>

#include <boost/algorithm/string/trim.hpp>
#include <boost/format.hpp>


using namespace opencog;

PatternMatch::PatternMatch(void)
{
	atom_space = NULL;
}

/* ================================================================= */
/**
 * Solve a predicate by pattern matching.
 * The predicate is defined in terms of two hypergraphs: one is a
 * hypergraph defining a pattern to be matched for, and the other is a
 * list of bound variables in the first.
 *
 * The bound variables are, by definition, nodes. (XXX It might be
 * useful to loosen this restriction someday). The list of bound variables
 * is then assumed to be listed using the ListLink type. So, for
 * example:
 *
 *    ListLink
 *        SomeNode "variable 1"
 *        SomeOtherNode "another variable"
 *
 * The predicate hypergraph is assumed to be a list of "clauses", where
 * each "clause" is a tree. The clauses are assumed to be connected,
 * i.e. share common nodes or links.  The algorithm to find solutions
 * will fail on disconnected hypergraphs.  The list of clauses is
 * specified by means of an AndLink, so, for example:
 *
 *     AndLink
 *        SomeLink ....
 *        SomeOtherLink ...
 *
 * The solution proceeds by requiring each clause to match some part of
 * the atomspace (i.e. of the universe of hypergraphs stored in the
 * atomspace). When a solution is found, PatternMatchCallback::solution
 * method is called, and it is passed two maps: one mapping the bound
 * variables to their solutions, and the other mapping the pattern
 * clauses to their corresponding solution clauses.
 *
 * At this time, the list of clauses is understood to be a single
 * disjunct; that is, all of the clauses must be simultaneously
 * satisfied.  Thus, in principle, one could build a layer on top of
 * this that accepts clauses in disjunctive normal form (and so on...)
 * It is not clear at this time how to benefit from Boolean SAT solver
 * technlogy (or at what point this would be needed).
 */
void PatternMatch::match(PatternMatchCallback *cb,
                         Handle hvarbles,
                         Handle hclauses,
                         Handle hnegates)
{
	Atom * avarbles = TLB::getAtom(hvarbles);
	Atom * aclauses = TLB::getAtom(hclauses);
	Atom * anegates = TLB::getAtom(hnegates);
	Link * lvarbles = dynamic_cast<Link *>(avarbles);
	Link * lclauses = dynamic_cast<Link *>(aclauses);
	Link * lnegates = dynamic_cast<Link *>(anegates);

	// Both must be non-empty.
	if (!lclauses || !lvarbles) return;

	// Types must be as expected
	Type tvarbles = lvarbles->getType();
	Type tclauses = lclauses->getType();
	if (LIST_LINK != tvarbles)
	{
		logger().warn("%s: expected ListLink for bound variable list",
			__FUNCTION__);
		return;
	}
	if (AND_LINK != tclauses)
	{
		logger().warn("%s: expected AndLink for clause list", __FUNCTION__);
		return;
	}

	const std::vector<Handle> &vars = lvarbles->getOutgoingSet();

	// negation clauses are optionally present
	std::vector<Handle> negs;
	if (NULL != lnegates)
	{
		Type tnegates = lnegates->getType();
		if (AND_LINK != tnegates)
		{
			logger().warn("%s: expected AndLink for clause list", __FUNCTION__);
			return;
		}
		negs = lnegates->getOutgoingSet();
		bool bogus = pme.validate(vars, negs);
		if (bogus)
		{
			logger().warn("%s: Constant clauses removed from pattern rejection",
				__FUNCTION__);
		}
	}

	// Make sure that the user did not pass in bogus clauses
	std::vector<Handle> clauses;
	clauses = lclauses->getOutgoingSet();


	bool bogus = pme.validate(vars, clauses);
	if (bogus)
	{
		logger().warn("%s: Constant clauses removed from pattern matching",
			__FUNCTION__);
	}

	pme.match(cb, vars, clauses, negs);


}

/* ================================================================= */
// Handy dandy utility class.
//
class FindVariables
{
	public:
		std::vector<Handle> varlist;

		/**
		 * Create a list of all of the VariableNodes that lie in the
		 * outgoing set of the handle (recursively).
		 */
		inline bool find_vars(Handle h)
		{
			Atom *a = TLB::getAtom(h);
			Node *n = dynamic_cast<Node *>(a);
			if (n)
			{
				if (n->getType() == VARIABLE_NODE)
				{
					varlist.push_back(h);
				}
				return false;
			}

			return foreach_outgoing_handle(h, &FindVariables::find_vars, this);
		}
};

/* ================================================================= */
/**
 * class Instantiator -- create grounded expressions from ungrounded ones.
 * Given an ungrounded expression (i.e. an expression containing variables)
 * and a map between variables and ground terms, it will create a new
 * expression, with the ground terms substituted for the variables.
 */
class Instantiator
{
	private:
		std::map<Handle, Handle> *vmap;

		std::vector<Handle> oset;
		bool walk_tree(Handle tree);
		Handle execution_link(void);
		bool did_exec;

	public:
		AtomSpace *as;
		Handle instantiate(Handle expr, std::map<Handle, Handle> &vars);
};

Handle Instantiator::execution_link()
{
	// The oset contains the grounded schema.
	if (2 != oset.size()) return Handle::UNDEFINED;
	Handle gs = oset[0];
	Node *gsn = dynamic_cast<Node *>(TLB::getAtom(gs));
	if (NULL == gsn) return Handle::UNDEFINED;

	// Get the schema name.
	const std::string& schema = gsn->getName();
	// printf ("Grounded schema name: %s\n", schema.c_str());

	// At this point, we only run scheme schemas.
	if (0 == schema.compare(0,4,"scm:", 4))
	{
#ifdef HAVE_GUILE
		SchemeEval &applier = SchemeEval::instance();
		Handle h = applier.apply(schema.substr(4), oset[1]);
		return h;
#else
		return Handle::UNDEFINED;
#endif /* HAVE_GUILE */
	}
	else
	{
		// Unkown proceedure type.  Return it, maybe some other
		// execution-link handler will be able to process it.
		return as->addLink(EXECUTION_LINK, oset, TruthValue::TRUE_TV());
	}
}

bool Instantiator::walk_tree(Handle expr)
{
	Atom *a = TLB::getAtom(expr);
	Type t = a->getType();
	Node *n = dynamic_cast<Node *>(a);
	if (n)
	{
		if (VARIABLE_NODE != t)
		{
			oset.push_back(expr);
			return false;
		}

		// If we are here, we found a variable. Look it up.
		std::map<Handle,Handle>::const_iterator it = vmap->find(expr);
		if (vmap->end() != it)
		{
			Handle soln = it->second;
			oset.push_back(soln);
		}
		else
		{
			oset.push_back(expr);
		}
		return false;
	}

	// If we are here, then we have a link. Walk it.
	std::vector<Handle> save_oset = oset;
	oset.clear();

	// Walk the subtree, substituting values for variables.
	foreach_outgoing_handle(expr, &Instantiator::walk_tree, this);

	// Fire execution links, if found.
	did_exec = false;  // set flag on top-level only
	if (t == EXECUTION_LINK)
	{
		did_exec = true;
		Handle sh = execution_link();
		oset = save_oset;
		if (Handle::UNDEFINED != sh)
		{
			oset.push_back(sh);
		}
		return false;
	}

	// Now create a duplicate link, but with an outgoing set where
	// the variables have been substituted by their values.
	const TruthValue& tv = a->getTruthValue();
	Handle sh = as->addLink(t, oset, tv);

	oset = save_oset;
	oset.push_back(sh);

	return false;
}

/**
 * instantiate -- create a grounded expression from an ungrounded one.
 *
 * Given a handle to an ungrounded expression, and a set of groundings,
 * this will create a grounded expression.
 *
 * The set of groundings is to be passed in with the map 'vars', which
 * maps variable names to their groundings -- it maps variable names to
 * atoms that already exist in the atomspace.  This method will then go
 * through all of the variables in the expression, and substitute them
 * with their values, creating a new expression. The new expression is
 * added to the atomspace, and its handle is returned.
 */
Handle Instantiator::instantiate(Handle expr, std::map<Handle, Handle> &vars)
{
	if (Handle::UNDEFINED == expr)
	{
		logger().warn("%s: Asked to ground a null expression", __FUNCTION__);
		return Handle::UNDEFINED;
	}
	vmap = &vars;
	oset.clear();
	did_exec = false;
	walk_tree(expr);
	if ((false == did_exec) && (oset.size() != 1))
	{
		logger().warn("%s: failure to ground expression (found %d groundings)\n"
			"Ungrounded expr is %s\n",
			__FUNCTION__, oset.size(), TLB::getAtom(expr)->toString().c_str());
	}
	if (oset.size() >= 1)
		return oset[0];
	return Handle::UNDEFINED;
}

/* ================================================================= */

/**
 * class Implicator -- pattern matching callback for grounding implicands.
 *
 * This class is meant to be used with the pattern matcher. When the
 * pattern matcher calls the callback, it will do so with a particular
 * grounding of the search pattern. If this class is holding an ungrounded
 * implicand, and will create a grounded version of the implicand. If
 * the implcand is alrready grounded, then it's a no-op -- this class
 * alone will *NOT* change its truth value.  Use a derived class for
 * this. 
 *
 * The 'var_soln' argument in the callback contains the map from variables
 * to ground terms. 'class Instantiator' is used to perform the actual
 * grounding.  A list of grounded expressions is created in 'result_list'.
 */
class Implicator :
	public virtual PatternMatchCallback
{
	protected:
		Instantiator inst;
	public:
		AtomSpace *as;
		Handle implicand;
		std::vector<Handle> result_list;
		virtual bool solution(std::map<Handle, Handle> &pred_soln,
		                      std::map<Handle, Handle> &var_soln);
};

bool Implicator::solution(std::map<Handle, Handle> &pred_soln,
                          std::map<Handle, Handle> &var_soln)
{
	// PatternMatchEngine::print_solution(pred_soln,var_soln);
	inst.as = as;
	Handle h = inst.instantiate(implicand, var_soln);
	if (TLB::isValidHandle(h))
	{
		result_list.push_back(h);            
	}
	return false;
}

/* ================================================================= */
/**
 * Evaluate an ImplicationLink.
 *
 * Given an ImplicationLink, this method will "evaluate" it, matching
 * the predicate, and creating a grounded implicand, assuming the
 * predicate can be satisfied. Thus, for example, given the structure
 *
 *    ImplicationLink
 *       AndList
 *          EvaluationList
 *             PredicateNode "_obj"
 *             ListLink
 *                ConceptNode "make"
 *                VariableNode "$var0"
 *          EvaluationList
 *             PredicateNode "from"
 *             ListLink
 *                ConceptNode "make"
 *                VariableNode "$var1"
 *       EvaluationList
 *          PredicateNode "make_from"
 *          ListLink
 *             VariableNode "$var0"
 *             VariableNode "$var1"
 *
 * Then, if the atomspace also contains a parsed version of the English
 * sentence "Pottery is made from clay", that is, if it contains the
 * hypergraph
 *
 *    EvaluationList
 *       PredicateNode "_obj"
 *       ListLink
 *          ConceptNode "make"
 *          ConceptNode "pottery"
 *
 * and the hypergraph
 *
 *    EvaluationList
 *       PredicateNode "from"
 *       ListLink
 *          ConceptNode "make"
 *          ConceptNode "clay"
 *
 * Then, by pattern matching, the predicate part of the ImplicationLink
 * can be fulfilled, binding $var0 to "pottery" and $var1 to "clay".
 * These bindings are refered to as the 'groundings' or 'solutions'
 * to the variables. So, e.g. $var0 is 'grounded' by "pottery".
 *
 * Next, a grounded copy of the implicand is then created; that is,
 * the following hypergraph is created and added to the atomspace:
 *
 *    EvaluationList
 *       PredicateNode "make_from"
 *       ListLink
 *          ConceptNode "pottery"
 *          ConceptNode "clay"
 *
 * As the above example illustrates, this function expects that the
 * input handle is an implication link. It expects the implication link
 * to consist entirely of one disjunct (one AndList) and one (ungrounded)
 * implicand.  All variables are implicit, and are identified by being
 * VariableNodes.  These variables are interpreted as 'free variables'
 * having no binding.  The act of pattern-matching to the predicate of
 * the implication has an implicit 'for-all' flavour to it: the pattern
 * is matched to 'all' matches in the atomspace.
 *
 * When a pattern match is found, the variables can be understood as
 * being grounded by some explicit ground terms in the atomspace. This
 * grounding is then used to create a grounded version of the
 * (ungrounded) implicand. That is, the variables in the implicand are
 * substituted by their grounding values.  This method then returns a
 * list of all of the grounded implicands that were created.
 *
 * Note that this method can be used to create a simple forward-chainer:
 * One need only to take a set of implication links, and call this
 * method repeatedly on them, until one is exhausted.
 */

Handle PatternMatch::do_imply (Handle himplication,
                               PatternMatchCallback *pmc,
                               std::vector<Handle> *varlist)
{
	Atom * aimpl = TLB::getAtom(himplication);
	Link * limpl = dynamic_cast<Link *>(aimpl);

	// Must be non-empty.
	if (!limpl) return Handle::UNDEFINED;

	// Type must be as expected
	Type timpl = limpl->getType();
	if (IMPLICATION_LINK != timpl)
	{
		logger().warn("%s: expected ImplicationLink", __FUNCTION__);
		return Handle::UNDEFINED;
	}

	const std::vector<Handle>& oset = limpl->getOutgoingSet();
	if (2 != oset.size())
	{
		logger().warn("%s: ImplicationLink has wrong size", __FUNCTION__);
		return Handle::UNDEFINED;
	}

	Handle hclauses = oset[0];
	Handle implicand = oset[1];

	Atom * aclauses = TLB::getAtom(hclauses);
	Link * lclauses = dynamic_cast<Link *>(aclauses);

	// Must be non-empty.
	if (!lclauses) return Handle::UNDEFINED;

	// The predicate is either an AndList, or a single clause
	// If its an AndList, then its a list of clauses.
	// XXX Should an OrList be supported ??
	std::vector<Handle> affirm, negate;
	Type tclauses = lclauses->getType();
	if (AND_LINK == tclauses)
	{
		// Input is in conjunctive normal form, consisting of clauses,
		// or their negations. Split these into two distinct lists.
		// Any clause that is a NotLink is "negated"; strip off the
		// negation and put it into its own list.
		const std::vector<Handle>& cset = lclauses->getOutgoingSet();
		size_t clen = cset.size();
		for (size_t i=0; i<clen; i++)
		{
			Handle h = cset[i];
			Atom *a = TLB::getAtom(h);
			Type t = a->getType();
			if (NOT_LINK == t)
			{
				Link *l = static_cast<Link *>(a);
				h = l->getOutgoingHandle(0);
				negate.push_back(h);
			}
			else
			{
				affirm.push_back(h);
			}
		}
	}
	else
	{
		// There's just one single clause!
		affirm.push_back(hclauses);
	}


	// Extract a list of variables, if needed.
	// This is used only by the deprecated imply() function, as the
	// VariableScopeLink will include a list of variables up-front.
	FindVariables fv;
	if (NULL == varlist)
	{
		fv.find_vars(hclauses);
		varlist = &fv.varlist;
	}

	// Make sure that every clause contains at least one variable.
	// (The presence of constant clauses will mess up the current
	// pattern matcher.)
	bool bogus = pme.validate(*varlist, affirm);
	if (bogus)
	{
		logger().warn("%s: Constant clauses removed from pattern matching",
			__FUNCTION__);
	}
	bogus = pme.validate(*varlist, negate);
	if (bogus)
	{
		logger().warn("%s: Constant clauses removed from pattern negation",
			__FUNCTION__);
	}

	// Now perform the search.
	Implicator *impl = dynamic_cast<Implicator *>(pmc);
	impl->implicand = implicand;
	pme.match(pmc, *varlist, affirm, negate);

        
	// The result_list contains a list of the grounded expressions.
	// Turn it into a true list, and return it.
	Handle gl = atom_space->addLink(LIST_LINK, impl->result_list);

	return gl;
}

/* ================================================================= */
typedef std::pair<Atom *, const std::set<Type> > ATPair;

/**
 * Extract the variable type(s) from a TypedVariableLink
 *
 * The call is expecting htypelink to point to one of the two
 * following structures:
 *
 *    TypedVariableLink
 *       VariableNode "$some_var_name"
 *       VariableTypeNode  "ConceptNode"
 *
 * or
 *
 *    TypedVariableLink
 *       VariableNode "$some_var_name"
 *       ListLink
 *          VariableTypeNode  "ConceptNode"
 *          VariableTypeNode  "NumberNode"
 *          VariableTypeNode  "WordNode"
 *
 * In either case, the variable itself is appended to "vset",
 * and the list of allowed types are associated with the variable
 * via the map "typemap". 
 */
int PatternMatch::get_vartype(Handle htypelink,
                              Atom * atypelink,
                              std::vector<Handle> &vset,
                              VariableTypeMap &typemap)
{
	Link * ltvl = dynamic_cast<Link *>(atypelink);
	const std::vector<Handle>& oset = ltvl->getOutgoingSet();
	if (2 != oset.size())
	{
		logger().warn("%s: TypedVariableLink has wrong size",
		       __FUNCTION__);
		return 1;
	}

	Handle varname = oset[0];
	Handle vartype = oset[1];

	Atom *avar = TLB::getAtom(varname);

	// The vartype is either a single type name, or a list of typenames.
	Atom *atype = TLB::getAtom(vartype);
	Type t = atype->getType();
	if (VARIABLE_TYPE_NODE == t)
	{
		const Node *n = dynamic_cast<const Node *>(atype);
		const std::string &tn = n->getName();
		Type vt = classserver().getType(tn);

		if (NOTYPE == vt)
		{
			logger().warn("%s: VariableTypeNode specifies unknown type: %s\n",
			               __FUNCTION__, tn.c_str());
			return 4;
		}

		std::set<Type> ts;
		ts.insert(vt);
		typemap.insert(ATPair(avar,ts));
		vset.push_back(varname);
	}
	else if (LIST_LINK == t)
	{
		std::set<Type> ts;

		const Link *l = dynamic_cast<const Link *>(atype);
		const std::vector<Handle>& tset = l->getOutgoingSet();
		size_t tss = tset.size();
		for (size_t i=0; i<tss; i++)
		{
			Handle h = tset[i];
			Atom *a = TLB::getAtom(h);
			if (VARIABLE_TYPE_NODE != a->getType())
			{
				logger().warn("%s: TypedVariableLink has unexpected content:\n"
				              "Expected VariableTypeNode, got %s",
				              __FUNCTION__,
				              classserver().getTypeName(a->getType()).c_str());
				return 3;
			}
			const Node *n = dynamic_cast<const Node *>(a);
			const std::string &tn = n->getName();
			Type vt = classserver().getType(tn);
			if (NOTYPE == vt)
			{
				logger().warn("%s: VariableTypeNode specifies unknown type: %s\n",
				               __FUNCTION__, tn.c_str());
				return 5;
			}
			ts.insert(vt);
		}

		typemap.insert(ATPair(avar,ts));
		vset.push_back(varname);
	}
	else
	{
		logger().warn("%s: Unexpected contents in TypedVariableLink\n"
				        "Expected VariableTypeNode or ListLink, got %s",
		              __FUNCTION__,
		              classserver().getTypeName(t).c_str());
		return 2;
	}

	return 0;
}

/* ================================================================= */
/**
 * Evaluate an ImplicationLink embedded in a VariableScopeLink
 *
 * Given a VariableScopeLink containing variable declarations and an
 * ImplicationLink, this method will "evaluate" the implication, matching
 * the predicate, and creating a grounded implicand, assuming the
 * predicate can be satisfied. Thus, for example, given the structure
 *
 *    VariableScopeLink
 *       ListLink
 *          VariableNode "$var0"
 *          VariableNode "$var1"
 *       ImplicationLink
 *          AndList
 *             etc ...
 * Evaluation proceeds as decribed in the "do_imply()" function above.
 * The whole point of the VariableScopeLink is to do nothing more than 
 * to limit the range of the scope of the variables.
 */

Handle PatternMatch::do_varscope (Handle hvarscope,
                                  DefaultPatternMatchCB *pmc)
{
	Atom * ascope = TLB::getAtom(hvarscope);
	Link * lscope = dynamic_cast<Link *>(ascope);

	// Must be non-empty.
	if (!lscope) return Handle::UNDEFINED;

	// Type must be as expected
	Type tscope = lscope->getType();
	if (VARIABLE_SCOPE_LINK != tscope)
	{
		const std::string& tname = classserver().getTypeName(tscope);
		logger().warn("%s: expected VariableScopeLink, got %s",
			 __FUNCTION__, tname.c_str());
		return Handle::UNDEFINED;
	}

	const std::vector<Handle>& oset = lscope->getOutgoingSet();
	if (2 != oset.size())
	{
		logger().warn("%s: VariableScopeLink has wrong size", __FUNCTION__);
		return Handle::UNDEFINED;
	}

	Handle hdecls = oset[0];  // VariableNode declarations
	Handle himpl = oset[1];   // ImplicationLink

	Atom * adecls = TLB::getAtom(hdecls);

	// vset is the vector of variables.
	// typemap is the (possibly empty) list of restrictions on atom types.
	std::vector<Handle> vset;
	VariableTypeMap typemap;

	// Expecting the declaration list to be either a single
	// variable, or a list of variable declarations
	Type tdecls = adecls->getType();
	if ((VARIABLE_NODE == tdecls) ||
	    (dynamic_cast<Node *>(adecls))) // allow *any* node as a variable
	{
		vset.push_back(hdecls);
	}
	else if (TYPED_VARIABLE_LINK == tdecls)
	{
		if (get_vartype(hdecls, adecls, vset, typemap)) return Handle::UNDEFINED;
	}
	else if (LIST_LINK == tdecls)
	{
		// The list of variable declarations should be .. a list of
		// variables! Make sure its as expected.
		Link * ldecls = dynamic_cast<Link *>(adecls);
		const std::vector<Handle>& dset = ldecls->getOutgoingSet();
		size_t dlen = dset.size();
		for (size_t i=0; i<dlen; i++)
		{
			Handle h = dset[i];
			Atom *a = TLB::getAtom(h);
			Type t = a->getType();
			if (VARIABLE_NODE == t)
			{
				vset.push_back(h);
			}
			else if (TYPED_VARIABLE_LINK == t)
			{
				if (get_vartype(h, a, vset, typemap)) return Handle::UNDEFINED;
			}
			else
			{
				logger().warn("%s: expected a VariableNode or a TypedVariableLink",
				           __FUNCTION__);
				return Handle::UNDEFINED;
			}
		}
	}
  	else
	{
		logger().warn("%s: expected a ListLink holding variable declarations",
		     __FUNCTION__);
		return Handle::UNDEFINED;
	}

	pmc->set_type_restrictions(typemap);

	Handle gl = do_imply(himpl, pmc, &vset);

	return gl;
}

/* ================================================================= */

class DefaultImplicator:
	public virtual Implicator,
	public virtual DefaultPatternMatchCB
{};

/**
 * DEPRECATED: USE VAR_SCOPE INSTEAD!
 * Default evaluator of implication statements.  Does not consider
 * the truth value of any of the matched clauses; instead, looks
 * purely for a structural match.
 *
 * See the do_imply function for details.
 */
Handle PatternMatch::imply (Handle himplication)
{
	// Now perform the search.
	DefaultImplicator impl;
	impl.as = atom_space;
	return do_imply(himplication, &impl, NULL);
}

class CrispImplicator:
	public virtual Implicator,
	public virtual CrispLogicPMCB
{
	public:
		virtual bool solution(std::map<Handle, Handle> &pred_soln,
		                      std::map<Handle, Handle> &var_soln);
};

/**
 * The crisp implicator needs to tweak the truth value of the 
 * resulting implicand. In most cases, this is not (strictly) needed,
 * for example, if the implcand has ungrounded variables, then
 * a truth value can be assigned to it, and the implicand will obtain
 * that truth value upon grounding.
 *
 * HOWEVER, if the implicand is fully grounded, then it will be given
 * a truth value of (false, uncertain) to start out with, and, if a
 * solution is found, then the goal here is to change its truth value 
 * to (true, certain).  That is the whole point of this function:
 * to tweak (affirm) the truth value of existing clauses!
 */
bool CrispImplicator::solution(std::map<Handle, Handle> &pred_soln,
                          std::map<Handle, Handle> &var_soln)
{
	// PatternMatchEngine::print_solution(pred_soln,var_soln);
	inst.as = as;
        Handle h = inst.instantiate(implicand, var_soln);

	//Handle h = inst.instantiate(implicand, var_soln);        
	if (h != Handle::UNDEFINED)
	{
		result_list.push_back(h);

		// Set truth value to true+confident
		Atom *a = TLB::getAtom(h);
		SimpleTruthValue stv(1,0);
		stv.setConfidence(1);
		a->setTruthValue(stv);
	}
	return false;
}


/**
 * DEPRECATED: USE VAR_SCOPE INSTEAD!
 * Use the crisp-logic callback to evaluate boolean implication
 * statements; i.e. statements that have truth values assigned
 * their clauses, and statements that start with NotLink's.
 * These are evaluated using "crisp" logic: if a matched clause
 * is true, its accepted, if its false, its rejected. If the
 * clause begins with a NotLink, true and false are reversed.
 *
 * The NotLink is also interpreted as an "absence of a clause";
 * if the atomspace does NOT contain a NotLink clause, then the
 * match is considered postive, and the clause is accepted (and
 * it has a null or "invalid" grounding).
 *
 * See the do_imply function for details.
 */
Handle PatternMatch::crisp_logic_imply (Handle himplication)
{
	// Now perform the search.
	CrispImplicator impl;
	impl.as = atom_space;
	return do_imply(himplication, &impl, NULL);
}

/**
 * Evaluate an ImplicationLink embedded in a VarScopeLink
 *
 * Use the crisp-logic callback to evaluate boolean implication
 * statements; i.e. statements that have truth values assigned
 * their clauses, and statements that start with NotLink's.
 * These are evaluated using "crisp" logic: if a matched clause
 * is true, its accepted, if its false, its rejected. If the
 * clause begins with a NotLink, true and false are reversed.
 *
 * The NotLink is also interpreted as an "absence of a clause";
 * if the atomspace does NOT contain a NotLink clause, then the
 * match is considered postive, and the clause is accepted (and
 * it has a null or "invalid" grounding).
 *
 * See the do_varscope function documentation for details.
 */
Handle PatternMatch::varscope (Handle himplication)
{
	// Now perform the search.
	CrispImplicator impl;
	impl.as = atom_space;
	return do_varscope(himplication, &impl);
}

/* ===================== END OF FILE ===================== */
