/*
 * Copyright (C) 2002-2007 Novamente LLC
 * Copyright (C) 2008 by Singularity Institute for Artificial Intelligence
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
 */

#include <algorithm>
#include <opencog/util/platform.h>
#include "../../PLN.h"

#include "../Rule.h"
#include "../Rules.h"
#include "../../AtomSpaceWrapper.h"
#include "../../PLNatom.h"
#include "../../BackInferenceTreeNode.h"

namespace haxx
{
    /// \todo This data must persist even if the BITNodeRoot is deleted.
    extern std::map<pHandle,std::vector<pHandle> > inferred_from;
    extern std::map<pHandle,opencog::pln::Rule*> inferred_with;
}

namespace opencog { namespace pln {

Btr<std::set<BoundVertex > >
CustomCrispUnificationRule::attemptDirectProduction(meta outh, bool fresh)
{
    if (asw->isSubType(_v2h(*outh->begin()), FORALL_LINK) ||
        asw->isSubType(_v2h(*outh->begin()), FW_VARIABLE_NODE))
        return Btr<std::set<BoundVertex > >();

    NMPrinter printer(NMP_HANDLE|NMP_TYPE_NAME|NMP_NODE_TYPE_NAME);
    printer.print(vtree(outh->begin()));

    cprintf(3,"FindMatchingUniversal...\n");
    Btr<ModifiedBoundVTree> i = FindMatchingUniversal(outh, hForAllLink, asw);
    cprintf(3,"FindMatchingUniversal OK!\n");

    if (!i) return Btr<std::set<BoundVertex > >();

    Btr<std::set<BoundVertex > > ret(new std::set<BoundVertex >);
    
    MPs ret1;
    typedef std::pair<pHandle,vtree> phvt;
    DeclareBtr(bindingsT, pre_binds);

    foreach(phvt vp, *i->bindings)
    {
        // Never bind a VariableNode to ATOM. That cannot be used in a real Atom.
        // This check can't just be done on outh (the target), because that would stop ATOM in
        // the target from matching to something _valid_ in the ForAllLink. Though it doesn't actually
        // support that anyway, see the following.
        //! @todo Note that if target contains ATOM, the algo won't fill in something from
        //! the ForAllLink that was more specific.
        //! This would be useful for FC.
        if (std::find(vp.second.begin(), vp.second.end(), Vertex((pHandle)ATOM))
            != vp.second.end()) {
            return Btr<std::set<BoundVertex > >();
        }

        (*pre_binds)[vp.first] = make_real(vp.second);
        printer.print(vp.first);
        cprintf(0,"=");
        printer.print((*pre_binds)[vp.first]);
    }

    BBvtree rootAtom(new BoundVTree(*i, pre_binds));
//    bind_Bvtree(rootAtom, *i->bindings);
//    pHandle topologicalStub = asw->addAtom(*rootAtom, TruthValue::TRIVIAL_TV(), false);
//
//    pHandle ret_h = asw->addLink(asw->getType(topologicalStub),
//                                 asw->getOutgoing(topologicalStub),
//                                 asw->getTV(i->original_handle), fresh);
    bind_Bvtree(rootAtom, *i->bindings);

    //pHandle ret_h = asw->addAtom(*rootAtom, asw->getTV(i->original_handle), fresh);
    pHandle ret_h = asw->addAtom(*rootAtom, asw->getTV(i->original_handle), false);
    
    ret->insert(BoundVertex(ret_h, pre_binds));
//    ret->insert(BoundVertex(ret_h, NULL));

/*  haxx::bitnoderoot->inferred_with[ret_h] = (Rule*)(int)this;
    if (haxx::bitnoderoot->inferred_from[ret_h].empty()) //comes here often, we want the link only once
        haxx::bitnoderoot->inferred_from[ret_h].push_back(hForAllLink);
*/
    haxx::inferred_with[ret_h] = (Rule*)this;
    if (haxx::inferred_from[ret_h].empty()) //comes here often, we want the link only once
        haxx::inferred_from[ret_h].push_back(hForAllLink);

    std::cout << name << " produced " << ret_h << std::endl;

    // Necessary since this is not the only way of producing the Atom.
    //reviseVersionedTVs(asw->fakeToRealHandle(_v2h(ret_h)).first);

    return ret;
}
}} // namespace opencog { namespace pln {
