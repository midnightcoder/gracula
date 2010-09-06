/*
 * DefaultPatternMatchCB.h
 *
 * Copyright (C) 2009 Linas Vepstas <linasvepstas@gmail.com>
 * All Rights Reserved
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
 *
 * Created by Linas Vepstas January 2009
 */

#ifndef _OPENCOG_DEFAULT_PATTERN_MATCH_H
#define _OPENCOG_DEFAULT_PATTERN_MATCH_H

#include <set>
#include <opencog/atomspace/types.h>
#include <opencog/atomspace/Link.h>
#include <opencog/atomspace/Node.h>
#include <opencog/query/PatternMatchCallback.h>

namespace opencog {

typedef std::map<Atom *, const std::set<Type> > VariableTypeMap;

/**
 * Callback mixin class, used to provide a default node and link
 * matching behaviour. This class is still a pure virtual class,
 * since it does not implement the solution method.
 *
 * The *only* thing it provides is node and link matching; it does
 * not consider any truth values in establishing a match.
 */
class DefaultPatternMatchCB :
	public virtual PatternMatchCallback
{
	public:
		DefaultPatternMatchCB(void)
		{
			type_restrictions = NULL;
		}

		/**
		 * Called when a node in the template pattern needs to 
		 * be compared to a possibly matching node in the atomspace.
		 * The first argument is a node from the pattern, and the
		 * second is a possible solution (grounding) node from the
		 * atomspace.
		 *
		 * Return false if the nodes match, else return
		 * true. (i.e. return true if mis-match).
		 *
		 * By default, the nodes must be identical.
		 */
		virtual bool node_match(Node *npat, Node *nsoln)
		{
			// If equality, then a match.
			if (npat == nsoln) return false;
			return true;
		}

		/**
		 * Called when a variable in the template pattern
		 * needs to be compared to a possible grounding
		 * node in the atomspace. The first argument
		 * is a variable from the pattern, and the second
		 * is a possible grounding node from the atomspace.
		 * Return false if the nodes match, else return
		 * true. (i.e. return true if mis-match).
		 */
		virtual bool variable_match(Node *npat, Node *nsoln)
		{
			Type pattype = npat->getType();

			// If the ungrounded term is not of type VariableNode, then just
			// accept the match. This allows any kind of node types to be 
			// explicitly bound as variables.  However, the type VariableNode
			// gets special handling, below.
			if (pattype != VARIABLE_NODE) return false;

			// If the solution is variable too, reject it out-of-hand,
			// even if its some variable in some utterly unrelated thing.
			Type soltype = nsoln->getType();
			if (soltype == VARIABLE_NODE) return true;

			// If the ungrounded term is a variable, then see if there
			// are any restrictions on the variable type. 
			// If no restrictions, we are good to go.
			if (NULL == type_restrictions) return false;

			// If we are here, there's a restriction on the grounding type.
			// Validate the node type, if needed.
			VariableTypeMap::const_iterator it = type_restrictions->find(npat);
			if (it == type_restrictions->end()) return false;

			// Is the ground-atom type in our list of allowed types?
			const std::set<Type> &tset = it->second;
			std::set<Type>::const_iterator allow = tset.find(soltype);
			if (allow != tset.end()) return false;
			return true;
		}

		/**
		 * Called when a link in the template pattern
		 * needs to be compared to a possibly matching
		 * link in the atomspace. The first argument
		 * is a link from the pattern, and the second
		 * is a possible solution link from the atomspace.
		 * Return false if the links match, else return
		 * true. (i.e. return true if mis-match).
		 *
		 * By default, the link arity and the 
		 * link types must match.
		 */
		virtual bool link_match(Link *lpat, Link *lsoln)
		{
			if (lpat == lsoln) return false;

			if (lpat->getArity() != lsoln->getArity()) return true;
			Type pattype = lpat->getType();
			Type soltype = lsoln->getType();

			// If types differ, no match,
			if (pattype != soltype) return true;
			return false;
		}

		/**
		 * Called to perform the actual search. This makes some default
		 * assumptions about the kind of things that might be matched,
		 * in order to drive a reasonably-fast search.
		 */
		virtual void perform_search(PatternMatchEngine *,
		                            const std::vector<Handle> &vars,
		                            const std::vector<Handle> &clauses,
		                            const std::vector<Handle> &negations);

		/**
		 * Indicate a set of restrictions on the types of the ground atoms.
		 * The typemap contains a map from variables to a set of types
		 * that the groundings for the variable are allowed to have.
		 */
		void set_type_restrictions(VariableTypeMap &tm)
		{
			type_restrictions = &tm;
		}
	private:
		Handle root;
		Handle starter_pred;
		Handle find_starter(Handle);
		bool loop_candidate(Handle);
		PatternMatchEngine *pme;
		VariableTypeMap *type_restrictions;
};

} // namespace opencog

#endif // _OPENCOG_DEFAULT_PATTERN_MATCH_H
