/*
 * opencog/embodiment/AtomSpaceExtensions/AtomSpaceUtil.cc
 *
 * Copyright (C) 2002-2009 Novamente LLC
 * All Rights Reserved
 * Author(s): Welter Luigi
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


/**
 * Class with util methods for related to AtomSpace manipulation
 *
 * Author: Welter Luigi
 * Copyright(c), 2007
 */

#include <opencog/atomspace/HandleTemporalPairEntry.h>
#include <opencog/atomspace/Node.h>
#include <opencog/atomspace/SimpleTruthValue.h>
#include <opencog/atomspace/SpaceServer.h>
#include <opencog/atomspace/Temporal.h>
#include <opencog/atomspace/TemporalTable.h>
#include <opencog/atomspace/TLB.h>

#include <opencog/util/misc.h>
#include <opencog/util/Logger.h>
#include <opencog/util/oc_assert.h>

#include "AtomSpaceUtil.h"
#include "PredefinedProcedureNames.h"
#include "CompareAtomTreeTemplate.h"

#include <algorithm>
#include <cctype>
#include <stack>

#include <boost/algorithm/string.hpp>

using std::string;
using std::list;

#define IS_HOLDING_PREDICATE_NAME "isHolding"
#define IS_HOLDING_SOMETHING_PREDICATE_NAME "isHoldingSomething"

Handle AtomSpaceUtil::addNode(AtomSpace& atomSpace,
                              Type nodeType,
                              const std::string& nodeName,
                              bool permanent, bool renew_sti)
{
    Handle result = atomSpace.getHandle(nodeType, nodeName);
    if (result == Handle::UNDEFINED) {
        result = atomSpace.addNode(nodeType, nodeName);
        if (permanent) {
            atomSpace.setLTI(result, 1);
        }
    } else if (permanent) {
        if (atomSpace.getLTI(result) < 1) {
            atomSpace.setLTI(result, 1);
        }
    } else if (renew_sti) {
        result = atomSpace.addNode(nodeType, nodeName);
    }
    return result;
}

Handle AtomSpaceUtil::addLink(AtomSpace& atomSpace,
                              Type linkType,
                              const HandleSeq& outgoing,
                              bool permanent, bool renew_sti)
{
    Handle result = atomSpace.getHandle(linkType, outgoing);
    if (result == Handle::UNDEFINED) {
        result = atomSpace.addLink(linkType, outgoing);
        if (permanent) {
            atomSpace.setLTI(result, 1);
        }
    } else if (permanent) {
        if (atomSpace.getLTI(result) < 1) {
            atomSpace.setLTI(result, 1);
        }
    } else if (renew_sti) {
        result = atomSpace.addLink(linkType, outgoing);
    }
    return result;
}

// TODO: DEPRECATED
Handle AtomSpaceUtil::addRewardPredicate(AtomSpace& atomSpace,
        const char* petId,
        unsigned long timestamp)
{
    HandleSeq evalLinkOutgoing;

    string predicateName = petId;
    predicateName += ".+++";
    evalLinkOutgoing.push_back(addNode(atomSpace, PREDICATE_NODE, predicateName.c_str()));
    HandleSeq emptyOutgoing;
    evalLinkOutgoing.push_back(addLink(atomSpace, LIST_LINK, emptyOutgoing));

    Handle evalLink = addLink(atomSpace, EVALUATION_LINK, evalLinkOutgoing);

    atomSpace.addTimeInfo(evalLink, timestamp);

    return evalLink;
}

// TODO: DEPRECATED
Handle AtomSpaceUtil::addPunishmentPredicate(AtomSpace& atomSpace,
        const char* petId,
        unsigned long timestamp)
{
    HandleSeq evalLinkOutgoing;

    string predicateName = petId;
    predicateName += ".---";
    evalLinkOutgoing.push_back(addNode(atomSpace, PREDICATE_NODE, predicateName.c_str()));
    HandleSeq emptyOutgoing;
    evalLinkOutgoing.push_back(addLink(atomSpace, LIST_LINK, emptyOutgoing));

    Handle evalLink = addLink(atomSpace, EVALUATION_LINK, evalLinkOutgoing);

    atomSpace.addTimeInfo(evalLink, timestamp);

    return evalLink;
}

bool AtomSpaceUtil::isActionPredicatePresent(const AtomSpace& atomSpace,
        const char* actionPredicateName,
        Handle actionExecLink,
        unsigned long sinceTimestamp)
{

    //cout << "Looking for action predicate '" << actionPredicateName << "' after timestamp '" << sinceTimestamp << "' for action: " << TLB::getAtom(actionExecLink)->toString() << endl;
    bool result = false;
    HandleSeq evalListLinkOutgoing;
    evalListLinkOutgoing.push_back(actionExecLink);
    Handle evalListLink = atomSpace.getHandle(LIST_LINK, evalListLinkOutgoing);
    if (evalListLink != Handle::UNDEFINED) {
        //cout << "Found the ListLink with the ExecLink inside" << endl;
        Handle predicateNode = atomSpace.getHandle(PREDICATE_NODE,
                               actionPredicateName);
        if (predicateNode != Handle::UNDEFINED) {
            //cout << "Found the PredicateNode" << endl;
            HandleSeq evalLinkOutgoing;
            evalLinkOutgoing.push_back(predicateNode);
            evalLinkOutgoing.push_back(evalListLink);
            Handle evalLink = atomSpace.getHandle(EVALUATION_LINK,
                                                  evalLinkOutgoing);
            //cout << "evalLink = " << evalLink << endl;
            if (evalLink != Handle::UNDEFINED) {
                //cout << "Found the EvalLink with the PredicateNode and the ListLink in its " << endl;
                list<HandleTemporalPair> ocurrences;
                atomSpace.getTimeInfo(back_inserter(ocurrences),
                                      evalLink,
                                      Temporal(sinceTimestamp),
                                      TemporalTable::NEXT_AFTER_END_OF);
                //if (!ocurrences.empty()) {
                //    cout << "Got the following TimeServer entry: " << ocurrences.front().toString() << " for " << TLB::getAtom(evalLink)->toString() << endl;
                //} else {
                //    cout << "Got no TimeServer entry for " << TLB::getAtom(evalLink)->toString() << endl;
                //}
                result = !ocurrences.empty();
            }
        }
    }
    return result;
}

bool AtomSpaceUtil::getXYZOFromPositionEvalLink(const AtomSpace& atomspace,
        Handle evalLink,
        double &x,
        double &y,
        double &z,
        Handle &o)
{
    if (atomspace.getType(evalLink) != EVALUATION_LINK
            ||  atomspace.getArity(evalLink) != 2)
        return false;

    Handle eval_pred = atomspace.getOutgoing(evalLink, 0);
    Handle pred_args = atomspace.getOutgoing(evalLink, 1);

    if (atomspace.getName(eval_pred) != AGISIM_POSITION_PREDICATE_NAME
            ||  atomspace.getArity(pred_args) != 4)
        return false;

    Handle xh = atomspace.getOutgoing(pred_args, 1);
    Handle yh = atomspace.getOutgoing(pred_args, 2);
    Handle zh = atomspace.getOutgoing(pred_args, 3);
    o = atomspace.getOutgoing(pred_args, 0);

    x = atof(atomspace.getName(xh).c_str());
    y = atof(atomspace.getName(yh).c_str());
    z = atof(atomspace.getName(zh).c_str());

    //      printf("%.3f %.3f %.3f found for %s\n", x, y, z, atomspace.getName(o).c_str());

    return true;
}

Handle AtomSpaceUtil::getSpaceMapHandleAtTimestamp(const AtomSpace &atomSpace,
        unsigned long t)
{
    Temporal temporal(t);
    std::vector<HandleTemporalPair> temporalPairs;

    Handle spaceMapNode = atomSpace.getHandle(CONCEPT_NODE, SpaceServer::SPACE_MAP_NODE_NAME);

    Handle spaceMapHandle = Handle::UNDEFINED;

    if (spaceMapNode == Handle::UNDEFINED) {
        return spaceMapHandle;
    }

    const SpaceServer& spaceServer = atomSpace.getSpaceServer();

    // get temporal pair for the EXACT timestamp. Only spaceMapNodes are
    // considered
    atomSpace.getTimeInfo(back_inserter(temporalPairs),
                          spaceMapNode, temporal,
                          TemporalTable::EXACT);
    if (!temporalPairs.empty()) {

        // found at least one temporalPair. There should be at most one
        spaceMapHandle = atomSpace.getAtTimeLink(temporalPairs[0]);
        if (!spaceServer.containsMap(spaceMapHandle)) {
            spaceMapHandle = Handle::UNDEFINED;
        }
    }

    if (spaceMapHandle == Handle::UNDEFINED) {
        // found none temporalPair with EXACT timestamp. Now looking for
        // timestamps before the one used.
        temporalPairs.clear();
        atomSpace.getTimeInfo(back_inserter(temporalPairs),
                              spaceMapNode,
                              temporal,
                              TemporalTable::PREVIOUS_BEFORE_START_OF);
        while (!temporalPairs.empty()) {
            spaceMapHandle = atomSpace.getAtTimeLink(temporalPairs[0]);
            Temporal* nextTemporal = temporalPairs[0].getTemporal();
            if (spaceServer.containsMap(spaceMapHandle)) {
                break;
            } else {
                spaceMapHandle = Handle::UNDEFINED;
                temporalPairs.clear();
                atomSpace.getTimeInfo(back_inserter(temporalPairs),
                                      spaceMapNode,
                                      *nextTemporal,
                                      TemporalTable::PREVIOUS_BEFORE_START_OF);

            }
        }
    }

    return spaceMapHandle;
}

bool AtomSpaceUtil::getPredicateValueAtSpaceMap(const AtomSpace& atomSpace,
        const std::string predicate,
        const SpaceServer::SpaceMap& sm,
        Handle obj1, Handle obj2)
{
    if (!sm.containsObject(atomSpace.getName(obj1)) ||
            !sm.containsObject(atomSpace.getName(obj2)) ) {
        logger().error("AtomSpaceUtil - One or both objects were not present in Space Map");
        return false;
    }

    if (predicate == "near") {

        try {
            //const SpaceServer::ObjectMetadata&
            //    md1 = sm.getMetaData(atomSpace.getName(obj1));
            const spatial::EntityPtr& entity1 = sm.getEntity( atomSpace.getName(obj1) );
            //SpaceServer::SpaceMapPoint obj1Center( entity1.getPosition( ).x, entity1.getPosition( ).y );
            //const SpaceServer::ObjectMetadata&
            //    md2 = sm.getMetaData(atomSpace.getName(obj2));

            const spatial::EntityPtr& entity2 = sm.getEntity( atomSpace.getName(obj2) );
            //SpaceServer::SpaceMapPoint obj2Center( entity2.getPosition( ).x, entity2.getPosition( ).y );

            //std::cout << "MD1 X : " << md1.centerX << " Y : " << md1.centerY
            //    << " MD2 X : " << md2.centerX << " Y : " << md2.centerY
            //    << std::endl;
            double dist = ( entity1->getPosition( )
                            - entity2->getPosition( ) ).length( );
            //SpaceServer::SpaceMap::eucDist(obj1Center, obj2Center);

            double maxDistance = ( sm.xMax( ) - sm.xMin( ) ) / 50;
            double distStrength = 0;
            if ( dist < maxDistance ) {
                distStrength = std::max( 0.0, 1.0 - dist / maxDistance );
            } // if

            return ( distStrength > 0.5 );

        } catch (opencog::AssertionException& e) {
            return false;
        }

    } else if (predicate == "above") {
        // need 3D info
        return false;
    } else if (predicate == "below") {
        // need 3D info
        return false;
    } else if (predicate == "inside") {
        // need 3D info
        return false;
    } else return false;
}

bool AtomSpaceUtil::getPredicateValueAtTimestamp(const AtomSpace &atomSpace,
        const std::string& predicate,
        unsigned long timestamp,
        Handle obj1, Handle obj2)
{
    // get the SpaceMap to do the test
    Handle spaceMapHandle = getSpaceMapHandleAtTimestamp(atomSpace,
                            timestamp);
    if (spaceMapHandle == Handle::UNDEFINED) {
        return false;
    } else {
        const SpaceServer::SpaceMap&
        sm = atomSpace.getSpaceServer().getMap(spaceMapHandle);
        return getPredicateValueAtSpaceMap(atomSpace,
                                           predicate, sm, obj1, obj2);
    }
}

bool AtomSpaceUtil::getHasSaidValueAtTime(const AtomSpace &atomSpace,
        unsigned long timestamp,
        unsigned long delay,
        Handle from_h,
        Handle to_h,
        const std::string& message,
        bool include_to)
{
    OC_ASSERT(delay < timestamp,
                     "timestamp - delay must be positive");
    unsigned long tl = timestamp - delay;
    unsigned long tu = timestamp;
    Temporal temp(tl, tu);
    std::list<HandleTemporalPair> ret;
    atomSpace.getTimeInfo(back_inserter(ret), Handle::UNDEFINED, temp,
                          TemporalTable::STARTS_WITHIN);

    if (ret.empty()) {
        return false;
    } else {
        //check if all atoms of the structure to find are present
        Handle action_done_h = atomSpace.getHandle(PREDICATE_NODE,
                               ACTION_DONE_PREDICATE_NAME);

        if (action_done_h == Handle::UNDEFINED)
            return false;
        Handle say_h = atomSpace.getHandle(GROUNDED_SCHEMA_NODE,
                                           SAY_SCHEMA_NAME);

        if (say_h == Handle::UNDEFINED)
            return false;
        //create the sentence atom
        string atom_message_name;
        if (include_to) {
            OC_ASSERT(
                             dynamic_cast<Node*>(TLB::getAtom(to_h)),
                             "Handle to_h should be a 'Node'.");
            atom_message_name = string("to:") + atomSpace.getName(to_h)
                                + string(": ") + message;
        } else atom_message_name = message;
        Handle sentence_h = atomSpace.getHandle(SENTENCE_NODE,
                                                atom_message_name);

        if (sentence_h == Handle::UNDEFINED) {
            return false;
        }

        //define template
        atom_tree *say_template =
            makeVirtualAtom(EVALUATION_LINK,
                            makeVirtualAtom(action_done_h, NULL),
                            makeVirtualAtom(LIST_LINK,
                                            makeVirtualAtom(EXECUTION_LINK,
                                                            makeVirtualAtom(say_h, NULL),
                                                            makeVirtualAtom(LIST_LINK,
                                                                            makeVirtualAtom(from_h, NULL),
                                                                            makeVirtualAtom(sentence_h, NULL),
                                                                            NULL
                                                                           ),
                                                            NULL
                                                           ),
                                            NULL
                                           ),
                            NULL
                           );

        does_fit_template dft(*say_template, &atomSpace, true);
        //iterate over the atom list to see if such a message has been said
        for (std::list<HandleTemporalPair>::const_iterator ret_it = ret.begin();
                ret_it != ret.end(); ++ret_it) {
            if (dft(ret_it->getHandle()))
                return true;
        }
        return false;
    }
}


bool AtomSpaceUtil::isMovingBtwSpaceMap(const AtomSpace& atomSpace,
                                        const SpaceServer::SpaceMap& sm1,
                                        const SpaceServer::SpaceMap& sm2,
                                        Handle obj)
{
    const std::string& obj_str = atomSpace.getName(obj);
    bool insm1 = sm1.containsObject(atomSpace.getName(obj));
    bool insm2 = sm2.containsObject(atomSpace.getName(obj));
    //we consider that :
    //1)if the object appears or disappears from the spaceMaps it has moved.
    //2)if it is in neither those spaceMap it hasn't
    if (insm1 && insm2) {
        //check if has moved
        //const SpaceServer::ObjectMetadata& md1 = sm1.getMetaData(obj_str);
        //const SpaceServer::ObjectMetadata& md2 = sm2.getMetaData(obj_str);
        const spatial::EntityPtr& entity1 = sm1.getEntity( obj_str );
        const spatial::EntityPtr& entity2 = sm2.getEntity( obj_str );
        //return md1==md2;
        return ( *entity1 == entity2 );
    } else if (!insm1 && !insm2)
        return false; //case 2)
    else return true; //case 1)
}

bool AtomSpaceUtil::isMovingBtwSpaceMap(const AtomSpace& atomSpace,
                                        const SpaceServer::SpaceMap& sm,
                                        Handle obj)
{
    //TODO
    OC_ASSERT(false);
    return false;
}

float AtomSpaceUtil::getPredicateValue(const AtomSpace &atomSpace,
                                       std::string predicateName,
                                       Handle a,
                                       Handle b)
throw(opencog::NotFoundException)
{

    HandleSeq seq0;
    seq0.push_back(a);

    // used for binary predicates like near, inside, above and below
    if (b != Handle::UNDEFINED) {
        seq0.push_back(b);
    } // if

    // testing if there is a predicate already
    Handle predicateHandle = atomSpace.getHandle(PREDICATE_NODE,
                             predicateName);
    if (predicateHandle == Handle::UNDEFINED) {
        throw opencog::NotFoundException( TRACE_INFO,
                                          ( std::string( "AtomSpaceUtil - Predicate not found: " ) + predicateName ).c_str( ) );
    } // if

    // testing if there is a list link already
    Handle listLinkHandle = atomSpace.getHandle(LIST_LINK, seq0);
    if (listLinkHandle == Handle::UNDEFINED) {
        throw opencog::NotFoundException( TRACE_INFO, ( "AtomSpaceUtil - List link not found. predicateName[" + predicateName + "]" ).c_str( ) );
    } // if

    HandleSeq seq;
    seq.push_back(predicateHandle);
    seq.push_back(listLinkHandle);

    Handle evalLinkHandle = atomSpace.getHandle(EVALUATION_LINK, seq);
    if (evalLinkHandle == Handle::UNDEFINED) {
        throw opencog::NotFoundException( TRACE_INFO, ( "AtomSpaceUtil - There is no evaluation link for predicate: " + predicateName ).c_str( ) );
    } // if
    return atomSpace.getTV(evalLinkHandle).getMean();
}


bool AtomSpaceUtil::isPredicateTrue(const AtomSpace &atomSpace,
                                    std::string predicateName,
                                    Handle a, Handle b)
{
    try {
        return ( getPredicateValue( atomSpace, predicateName, a, b ) > 0.5 );
    } catch ( opencog::NotFoundException& ex ) {
        return false;
    } // catch
}

bool AtomSpaceUtil::isPetOwner( const AtomSpace& atomSpace,
                                Handle avatar, Handle pet )
{
    HandleSeq seq0;
    seq0.push_back(avatar);
    seq0.push_back(pet);

    // testing if there is a predicate already
    Handle predicateHandle = atomSpace.getHandle(PREDICATE_NODE,
                             OWNERSHIP_PREDICATE_NAME );
    if (predicateHandle == Handle::UNDEFINED) {
        logger().fine(
                     "IsFriendly - Found no \"owns\" predicate.");
        return false;
    } // if

    // testing if there is a list link already
    Handle listLinkHandle = atomSpace.getHandle(LIST_LINK, seq0);
    if (listLinkHandle == Handle::UNDEFINED) {
        logger().fine(
                     "IsFriendly - Obj %s and %s have no ListLink.",
                     atomSpace.getName(avatar).c_str(),
                     atomSpace.getName(pet).c_str());
        return false;
    } // if

    HandleSeq seq;
    seq.push_back(predicateHandle);
    seq.push_back(listLinkHandle);

    Handle evalLinkHandle = atomSpace.getHandle(EVALUATION_LINK, seq);
    if (evalLinkHandle == Handle::UNDEFINED) {
        logger().warn("IsFriendly - Found no EvalLink.");
        return false;
    }

    return true;

}

bool AtomSpaceUtil::getSizeInfo(AtomSpace& atomSpace,
                                Handle object,
                                double& length, double& width, double &height)
{

    Handle sizePredicate = atomSpace.getHandle(PREDICATE_NODE,
                           SIZE_PREDICATE_NAME);
    if (sizePredicate == Handle::UNDEFINED) {
        logger().fine(
                     "AtomSpaceUtil - No size predicate found.");
        return false;
    }

#ifdef USE_GET_HANDLE_SET
    HandleSeq seq;
    seq.push_back(sizePredicate);
    seq.push_back(Handle::UNDEFINED);

    std::vector<Handle> allHandles;
    atomSpace.getHandleSet(back_inserter(allHandles),
                           seq, NULL, NULL, 2, EVALUATION_LINK, false);

    foreach(Handle evalLink, allHandles) {
        // getting data from evalLink
        Handle listLink = atomSpace.getOutgoing(evalLink, 1);
        if (atomSpace.getType(listLink) == LIST_LINK
                && atomSpace.getArity(listLink) == 4) {
            // ensure that the size predicate relats to the object
            if (atomSpace.getOutgoing(listLink, 0) == object) {
//TODO: elvys remove logs comment
                length = atof(atomSpace.getName(atomSpace.getOutgoing(listLink, 1)).c_str());
                width = atof(atomSpace.getName(atomSpace.getOutgoing(listLink, 2)).c_str());
                height = atof(atomSpace.getName(atomSpace.getOutgoing(listLink, 3)).c_str());
//                logger().info("AtomSpaceUtil - Obj %s (width: %.2lf length: %.2lf height: %.2lf)",
//                            atomSpace.getName(object).c_str(), width, length, height);
                return true;
            }
        }
    }
#else
    HandleSeq incomingSet = atomSpace.getIncoming(sizePredicate);
    foreach(Handle incomingHandle, incomingSet) {
        Link* incomingLink = (Link*) TLB::getAtom(incomingHandle);
        if (incomingLink->getType() == EVALUATION_LINK &&  incomingLink->getArity() == 2 && 
                incomingLink->getOutgoingHandle(0) == sizePredicate) {
            Handle targetHandle = incomingLink->getOutgoingHandle(1);
            Atom* targetAtom = TLB::getAtom(targetHandle);
            if (targetAtom->getType() == LIST_LINK) {
                Link* listLink = (Link*) targetAtom;
                if (listLink->getArity() == 4 && listLink->getOutgoingHandle(0) == object) {
                    length = atof(atomSpace.getName(listLink->getOutgoingHandle(1)).c_str());
                    width = atof(atomSpace.getName(listLink->getOutgoingHandle(2)).c_str());
                    height = atof(atomSpace.getName(listLink->getOutgoingHandle(3)).c_str());
                    return true;
                }
            }
        }
    } 
#endif 

//TODO: elvys remove logs comment
//    logger().fine("AtomSpaceUtil - No size pred for obj %s found.",
//                    atomSpace.getName(object).c_str());
    return false;
}

Handle AtomSpaceUtil::addGenericPropertyPred(AtomSpace& atomSpace,
        std::string predicateName,
        const HandleSeq& ll_out,
        const TruthValue &tv, bool permanent, const Temporal &t)
{
    // truth criterion as defined in
    // https://extranet.vettalabs.com:8443/bin/view/Petaverse/PetaverseCombo
    bool predBool = true;
    if (tv.getMean() >= 0.5) {
        predBool = false;
    }

    Handle ph = atomSpace.getHandle(PREDICATE_NODE, predicateName);
    // if predicate handle not defined and TV equals < 0.5 just return
    if (ph == Handle::UNDEFINED && predBool) {
        logger().fine(
                     "AtomSpaceUtil - %s not added (no pred handle and TV less than 0.5).",
                     predicateName.c_str());
        return Handle::UNDEFINED;
    } else {
        ph = AtomSpaceUtil::addNode(atomSpace,
                                    PREDICATE_NODE,
                                    predicateName, true);
    }

    Handle ll = atomSpace.getHandle(LIST_LINK, ll_out);
    // if list link handle not defined and TV equals < 0.5 just return
    if (ll == Handle::UNDEFINED && predBool) {
        logger().fine(
                     "AtomSpaceUtil - %s not added (no ListLink and TV less than 0.5)",
                     predicateName.c_str());
        return Handle::UNDEFINED;
    } else {
        ll = atomSpace.addLink(LIST_LINK, ll_out);
    }

    HandleSeq hs2;
    hs2.push_back(ph);
    hs2.push_back(ll);
    Handle el = atomSpace.getHandle(EVALUATION_LINK, hs2);

    // if evaluation link handle not defined and TV equals < 0.5 just return
    if (el == Handle::UNDEFINED && predBool) {
        logger().fine(
                     "AtomSpaceUtil - %s not added (no EvalLink and TV less than 0.5).",
                     predicateName.c_str());
        return Handle::UNDEFINED;
    } else {
        el = atomSpace.addLink(EVALUATION_LINK, hs2);
        logger().fine(
                     "AtomSpaceUtil - %s added with TV %f.",
                     predicateName.c_str(), tv.getMean());
    }

    atomSpace.setTV(el, tv);

    Handle result;
    // if not undefined temporal then  a time information should be inserted
    // inserted into AtomSpace.
    if (t != UNDEFINED_TEMPORAL) {
        result = atomSpace.addTimeInfo(el, t);
    } else {
        result = el;
    }
    if (permanent) {
        atomSpace.setLTI(result, 1);
    }
    return result;
}

Handle AtomSpaceUtil::getMostRecentEvaluationLink(const AtomSpace& atomSpace,
        const std::string& predicateNodeName )
{

    std::vector<HandleTemporalPair> timestamps;
    getAllEvaluationLinks( atomSpace, timestamps, predicateNodeName );

    if ( timestamps.size() == 0 ) {
        logger().debug(
                     "AtomSpaceUtil - Found no entries for PredicateNode '%s' in TimeServer.",
                     predicateNodeName.c_str());
        return Handle::UNDEFINED;
    } // if

    int mostRecentIndex = 0;
    Temporal *mostRecent = timestamps[mostRecentIndex].getTemporal();
    for (unsigned int i = 1; i < timestamps.size(); i++) {
        if (*(timestamps[i].getTemporal()) > *mostRecent) {
            mostRecent = timestamps[i].getTemporal();
            mostRecentIndex = i;
        } // if
    } // for

    Handle selectedHandle = timestamps[mostRecentIndex].getHandle();
    if ( selectedHandle == Handle::UNDEFINED) {
        logger().debug(
                     "AtomSpaceUtil - Timeserver returned NULL handle for PredicateNode '%s'",
                     predicateNodeName.c_str());
        return Handle::UNDEFINED;
    } // if

    return selectedHandle;
}

float AtomSpaceUtil::getCurrentPetFeelingLevel( const AtomSpace& atomSpace,
        const std::string& petId,
        const std::string& feelingName )
{
    std::string predicateName = petId + "." + feelingName;

    Handle selectedEvalLink = getMostRecentEvaluationLink( atomSpace,
                              predicateName );

    if (selectedEvalLink == Handle::UNDEFINED) {
        logger().fine(
                     "AtomSpaceUtil - Found no EvaluationLink for PredicateNode '%s'.",
                     predicateName.c_str());
        return -1;
    } // if

    Handle primaryListLink = atomSpace.getOutgoing(selectedEvalLink, 1);
    if (primaryListLink == Handle::UNDEFINED) {
        logger().warn(
                     "AtomSpaceUtil - Found no signals for PredicateNode '%s'. Null primary listLink.",
                     predicateName.c_str());
        return -1;
    } // if

    int i = 0;
    do {
        Handle secondaryListLink = atomSpace.getOutgoing(primaryListLink, i);
        if (secondaryListLink == Handle::UNDEFINED) {
            logger().warn(
                         "AtomSpaceUtil - Found no signals for PredicateNode '%s'. Null secondary listLink.",
                         predicateName.c_str());
            return -1;
        }
        Handle paramName = atomSpace.getOutgoing(secondaryListLink, 0);
        if (paramName == Handle::UNDEFINED) {
            logger().warn(
                         "AtomSpaceUtil - Found no signals for PredicateNode '%s'. Null paramName.",
                         predicateName.c_str());
            return -1;
        } // if

        if (atomSpace.getName(paramName) == "level") {
            Handle paramValue = atomSpace.getOutgoing(secondaryListLink, 1);
            if (paramValue == Handle::UNDEFINED) {
                logger().warn(
                             "AtomSpaceUtil - Found no signals for PredicateNode '%s'. Null paramValue.",
                             predicateName.c_str());
                return -1;
            } // if

            return atof( atomSpace.getName(paramValue).c_str( ) );
        } // if

        i++;

        if (i >= atomSpace.getArity(primaryListLink)) {
            logger().warn(
                         "AtomSpaceUtil - Found no signals for PredicateNode '%s'. Invalid listLink.",
                         predicateName.c_str());
            return -1;
        } // if

    } while (true);

    return -1;
}

void AtomSpaceUtil::getAllEvaluationLinks(const AtomSpace& atomSpace,
        std::vector<HandleTemporalPair>& timestamps,
        const std::string& predicateNodeName,
        const Temporal& temporal,
        TemporalTable::TemporalRelationship criterion,
        bool needSort)
{

    logger().fine(
                 "AtomSpaceUtil - getAllEvaluationLinks - Searching for PredicateNode '%s'.",
                 predicateNodeName.c_str());

    std::vector<Handle> handles;
    atomSpace.getHandleSet(back_inserter(handles),
                           predicateNodeName.c_str(),
                           PREDICATE_NODE,
                           EVALUATION_LINK, true);

    if (handles.empty()) {
        logger().debug(
                     "AtomSpaceUtil - getAllEvaluationLinks - Found no EvaluationLink for PredicateNode '%s'",
                     predicateNodeName.c_str());
        return;
    } // if

    for ( unsigned int i = 0; i < handles.size(); ++i ) {
        atomSpace.getTimeInfo(back_inserter(timestamps),
                              handles[i], temporal, criterion);
    } // for

    if (needSort) {
        std::sort(timestamps.begin(),
                  timestamps.end(),
                  HandleTemporalPairEntry::SortComparison());
    }

    logger().fine(
                 "AtomSpaceUtil - getAllEvaluationLinks - end.");
}

Handle AtomSpaceUtil::setPredicateValue( AtomSpace& atomSpace,
        std::string predicateName,
        const TruthValue &tv,
        Handle object1,
        Handle object2 )
{
    HandleSeq listLink;
    listLink.push_back( object1 );
    if ( object2 != Handle::UNDEFINED ) {
        listLink.push_back( object2 );
    }
    Handle listLinkHandle = AtomSpaceUtil::addLink(atomSpace,
                            LIST_LINK,
                            listLink);
    Handle predicateHandle = AtomSpaceUtil::addNode(atomSpace,
                             PREDICATE_NODE,
                             predicateName, true);
    HandleSeq evalLink;
    evalLink.push_back( predicateHandle );
    evalLink.push_back( listLinkHandle );
    Handle evalLinkHandle = AtomSpaceUtil::addLink(atomSpace,
                            EVALUATION_LINK,
                            evalLink, true);
    atomSpace.setTV( evalLinkHandle, tv );
    return evalLinkHandle;
}


Handle AtomSpaceUtil::addPropertyPredicate(AtomSpace& atomSpace,
        std::string predicateName,
        Handle object,
        const TruthValue &tv,
        bool permanent,
        const Temporal &t)
{
    HandleSeq ll_out;
    ll_out.push_back(object);
    return addGenericPropertyPred(atomSpace, predicateName,
                                  ll_out, tv, permanent, t);
}

Handle AtomSpaceUtil::addPropertyPredicate(AtomSpace& atomSpace,
        std::string predicateName,
        Handle a,
        Handle b,
        const TruthValue& tv,
        const Temporal& t)
{

    HandleSeq ll_out;
    ll_out.push_back(a);
    ll_out.push_back(b);
    return addGenericPropertyPred(atomSpace,
                                  predicateName,
                                  ll_out, tv, false, t);
}

void AtomSpaceUtil::setupHoldingObject( AtomSpace& atomSpace,
                                        const std::string& holderId,
                                        const std::string& objectId,
                                        long unsigned currentTimestamp )
{

    Handle holderHandle = getAgentHandle( atomSpace, holderId );
    if ( holderHandle == Handle::UNDEFINED ) {
        return;
    } // if

    Handle objectHandle = Handle::UNDEFINED;
    if ( objectId != "" ) {
        objectHandle = atomSpace.getHandle( OBJECT_NODE, objectId );
        if ( objectHandle != Handle::UNDEFINED ) {
            logger().debug(
                          "AtomSpaceUtil - Object is a OBJECT_NODE" );
        } else {
            objectHandle = atomSpace.getHandle( ACCESSORY_NODE, objectId );
            if ( objectHandle != Handle::UNDEFINED ) {
                logger().debug(
                              "AtomSpaceUtil - Object is a ACCESSORY_NODE" );
            } else {
                logger().error(
                              "AtomSpaceUtil - Object cannot be identified" );
                return;
            } // if
        } // if
    } // if

    if (objectHandle == Handle::UNDEFINED) {
        // drop operation
        logger().debug(
                     "AtomSpaceUtil - The object's handle is undefined. Holder dropped the object");
        Handle isHoldingOldAtTimeLink = getMostRecentIsHoldingAtTimeLink(atomSpace, holderId);
        if (isHoldingOldAtTimeLink != Handle::UNDEFINED) {
            // Reconfigure the timestamp of the last grabbed object
            Handle isHoldingEvalLink = getTimedHandle(atomSpace,
                                       isHoldingOldAtTimeLink);
            if ( isHoldingEvalLink != Handle::UNDEFINED ) {
                Temporal t = getTemporal(atomSpace, isHoldingOldAtTimeLink);
                // TODO: What if it was not holding anything anymore (this happens if it gets 2 or more consecutive drop operations)
                // Shouldn't the old isHoldingatTimeLink be updated?
                //remove the old atTimeLink
                atomSpace.removeAtom(isHoldingOldAtTimeLink);
                //add the new one
                long unsigned tl = t.getLowerBound();
                Temporal new_temp(tl, std::max(currentTimestamp - 20, tl));
                logger().debug(
                             "AtomSpaceUtil - setupHoldingObject: new time = '%s'",
                             new_temp.toString().c_str());
                atomSpace.addTimeInfo(isHoldingEvalLink, new_temp); // Now, it can be forgotten.
            } // if
        } // if
        AtomSpaceUtil::setPredicateValue( atomSpace,
                                          IS_HOLDING_SOMETHING_PREDICATE_NAME,
                                          SimpleTruthValue( 0.0, 1.0 ),
                                          holderHandle );
    } else {
        // grab: it is now holding an object
        // TODO: What if it is already holding another thing? Shouldn't the old isHoldingatTimeLink be updated?
        logger().debug(
                     "AtomSpaceUtil - Now '%s' is holding '%s' at '%ul'",
                     atomSpace.getName(holderHandle).c_str(),
                     atomSpace.getName(objectHandle).c_str(),
                     currentTimestamp);
        AtomSpaceUtil::setPredicateValue( atomSpace,
                                          IS_HOLDING_SOMETHING_PREDICATE_NAME,
                                          SimpleTruthValue( 1.0, 1.0 ),
                                          holderHandle );
        Handle isHoldingAtTimeLink =
            AtomSpaceUtil::addPropertyPredicate( atomSpace,
                                                 IS_HOLDING_PREDICATE_NAME,
                                                 holderHandle,
                                                 objectHandle,
                                                 SimpleTruthValue( 1.0, 1.0 ),
                                                 Temporal(currentTimestamp) );
        atomSpace.setLTI(isHoldingAtTimeLink, 1); // Now, it cannot be forgotten (until the agent drop the object)
    } // else
}

Handle AtomSpaceUtil::getLatestHoldingObjectHandle(const AtomSpace& atomSpace,
        const std::string& holderId )
{

    Handle holderHandle = getAgentHandle( atomSpace, holderId );
    if ( holderHandle == Handle::UNDEFINED ) {
        return Handle::UNDEFINED;
    } // if

    Handle isHoldingLink = getMostRecentIsHoldingLink( atomSpace, holderId );
    if ( isHoldingLink != Handle::UNDEFINED ) {
        Handle listLink = atomSpace.getOutgoing(isHoldingLink, 1);
        if ( listLink != Handle::UNDEFINED ) {
            Handle objectHandle = atomSpace.getOutgoing(listLink, 1);
            if ( objectHandle != Handle::UNDEFINED ) {
                return objectHandle;
            } // if
            logger().error(
                          "AtomSpaceUtil - There is no object on list link" );
        } // if
        logger().error(
                      "AtomSpaceUtil - There is no listlink on isHoldingLink" );
    } // if

    logger().debug(
                  "AtomSpaceUtil - There is no isHoldingLink for %s",
                  holderId.c_str( ) );
    return Handle::UNDEFINED;
}

bool AtomSpaceUtil::isObjectBeingHolded( const AtomSpace& atomSpace,
        const std::string& objectId )
{
    return ( getObjectHolderHandle( atomSpace, objectId ) != Handle::UNDEFINED );
}

Handle AtomSpaceUtil::getObjectHolderHandle( const AtomSpace& atomSpace,
        const std::string& objectId )
{
    // TODO: try to optimize this method. it is using twice the getHandleSet for
    // isHolding (below and through getLatestHoldingObjectHandle)
    std::vector<Handle> handles;
    atomSpace.getHandleSet( back_inserter(handles),
                            OBJECT_NODE,
                            objectId, true );

    if ( handles.size( ) != 1 ) {
        logger().debug(
                     "AtomSpaceUtil - No agent is holding object[%s]",
                     objectId.c_str( ) );
        return Handle::UNDEFINED;
    } // if
    Handle holdedObjectHandle = handles[0];

    handles.clear( );
    atomSpace.getHandleSet( back_inserter(handles),
                            "isHolding",
                            PREDICATE_NODE,
                            EVALUATION_LINK, true );

    std::vector<HandleTemporalPair> timestamps;
    for ( unsigned int i = 0; i < handles.size(); ++i ) {
        Handle listLink = atomSpace.getOutgoing(handles[i], 1);
        if ( listLink != Handle::UNDEFINED ) {
            if ( atomSpace.getOutgoing(listLink, 1) == holdedObjectHandle ) {
                // skip other holded objects which aren't that we're seeking
                continue;
            } // if

            Handle holderHandle = atomSpace.getOutgoing(listLink, 0 );
            if ( !AtomSpaceUtil::isPredicateTrue( atomSpace,
                                                  IS_HOLDING_SOMETHING_PREDICATE_NAME,
                                                  holderHandle ) ) {
                // skip links which the holder isn't holding something now
                continue;
            } // if

            if ( getLatestHoldingObjectHandle(atomSpace,
                                              atomSpace.getName(holderHandle))
                    != holdedObjectHandle ) {
                // skip objects that are holding by the same holder of the target object
                // but arent't the target
                continue;
            } // if

            atomSpace.getTimeInfo( back_inserter( timestamps ), handles[i]);


        } // if
    } // for

    // get the most recent eval link
    if ( timestamps.size( ) > 0 ) {
        int mostRecentIndex = 0;
        Temporal *mostRecent = timestamps[mostRecentIndex].getTemporal();
        for (unsigned int i = 1; i < timestamps.size(); i++) {
            if (*(timestamps[i].getTemporal()) > *mostRecent) {
                mostRecent = timestamps[i].getTemporal();
                mostRecentIndex = i;
            } // if
        } // for

        Handle listLink = atomSpace.getOutgoing(timestamps[mostRecentIndex].getHandle( ), 1);
        return atomSpace.getOutgoing(listLink, 0);
    } // if

    return Handle::UNDEFINED;
}

std::string AtomSpaceUtil::getObjectHolderId( const AtomSpace& atomSpace,
        const std::string& objectId )
{
    Handle objectHandle = getObjectHolderHandle( atomSpace, objectId );
    if ( objectHandle != Handle::UNDEFINED ) {
        return atomSpace.getName( objectHandle );
    } // if
    return "";
}


Handle AtomSpaceUtil::getMostRecentIsHoldingAtTimeLink(const AtomSpace& atomSpace,
        const std::string& holderId )
{
    Handle holderHandle = getAgentHandle( atomSpace, holderId );
    if ( holderHandle == Handle::UNDEFINED ) {
        return Handle::UNDEFINED;
    } // if

    std::vector<Handle> handles;
    atomSpace.getHandleSet( back_inserter(handles),
                            IS_HOLDING_PREDICATE_NAME,
                            PREDICATE_NODE, EVALUATION_LINK, true );

    std::vector<HandleTemporalPair> timestamps;
    for ( unsigned int i = 0; i < handles.size(); ++i ) {
        Handle listLink = atomSpace.getOutgoing(handles[i], 1);
        if ( listLink != Handle::UNDEFINED ) {
            // get only holder's eval-links
            if ( atomSpace.getOutgoing(listLink, 0) == holderHandle ) {
                atomSpace.getTimeInfo( back_inserter( timestamps ),
                                       handles[i]);
            } // if
        } // if
    } // for

    // get the most recent eval link
    if ( timestamps.size( ) > 0 ) {
        int mostRecentIndex = 0;
        Temporal *mostRecent = timestamps[mostRecentIndex].getTemporal();
        for (unsigned int i = 1; i < timestamps.size(); i++) {
            if (*(timestamps[i].getTemporal()) > *mostRecent) {
                mostRecent = timestamps[i].getTemporal();
                mostRecentIndex = i;
            } // if
        } // for

        logger().debug(
                      "AtomSpaceUtil - The most recent holded object %ul",
                      timestamps[mostRecentIndex].getTemporal( )->getUpperBound( ) );

        return atomSpace.getAtTimeLink(timestamps[mostRecentIndex]);
    } // if
    return Handle::UNDEFINED;
}

Handle AtomSpaceUtil::getMostRecentIsHoldingLink(const AtomSpace& atomSpace,
        const std::string& holderId )
{
    Handle h = getMostRecentIsHoldingAtTimeLink(atomSpace, holderId);
    if (h != Handle::UNDEFINED)
        return atomSpace.getOutgoing(h, 1);
    else return Handle::UNDEFINED;
}

std::string AtomSpaceUtil::getHoldingObjectId(const AtomSpace& atomSpace,
        const std::string& holderId )
{
    Handle holderHandle = getAgentHandle( atomSpace, holderId );
    if ( holderHandle == Handle::UNDEFINED ) {
        return "";
    } // if

    if ( !AtomSpaceUtil::isPredicateTrue( atomSpace,
                                          IS_HOLDING_SOMETHING_PREDICATE_NAME,
                                          holderHandle ) ) {
        return "";
    } // if

    Handle objectHandle = getLatestHoldingObjectHandle( atomSpace, holderId );
    if ( objectHandle == Handle::UNDEFINED ) {
        return "";
    } // if

    return atomSpace.getName(objectHandle);
}

bool AtomSpaceUtil::isHoldingSomething(const AtomSpace& atomSpace,
                                       const std::string& holderId)
{
    Handle holderHandle = getAgentHandle(atomSpace, holderId);
    if (holderHandle == Handle::UNDEFINED) {
        return false;
    }

    return AtomSpaceUtil::isPredicateTrue(atomSpace,
                                          IS_HOLDING_SOMETHING_PREDICATE_NAME,
                                          holderHandle);
}

Handle AtomSpaceUtil::getHoldingObjectHandleAtTime(const AtomSpace& atomSpace,
        const std::string& holderId,
        unsigned long time)
{
    Handle isHoldingLink = getIsHoldingLinkAtTime(atomSpace,
                           holderId,
                           time);
    if ( isHoldingLink != Handle::UNDEFINED ) {
        Handle listLink = atomSpace.getOutgoing(isHoldingLink, 1);
        if ( listLink != Handle::UNDEFINED ) {
            Handle objectHandle = atomSpace.getOutgoing(listLink, 1);
            if ( objectHandle != Handle::UNDEFINED ) {
                return objectHandle;
            } // if
            logger().error(
                          "AtomSpaceUtil - There is no object on list link" );
        } // if
        logger().error(
                      "AtomSpaceUtil - There is no listlink on isHoldingLink" );
    } // if

    logger().debug(
                  "AtomSpaceUtil - There is no isHoldingLink for %s",
                  holderId.c_str( ) );
    return Handle::UNDEFINED;
}

Handle AtomSpaceUtil::getIsHoldingLinkAtTime(const AtomSpace& atomSpace,
        const std::string& holderId,
        unsigned long time)
{
    Handle holderHandle = getAgentHandle( atomSpace, holderId );
    if ( holderHandle == Handle::UNDEFINED ) {
        return Handle::UNDEFINED;
    } // if


    std::vector<Handle> handles;
    atomSpace.getHandleSet(back_inserter(handles), IS_HOLDING_PREDICATE_NAME,
                           PREDICATE_NODE, EVALUATION_LINK, true);

    std::vector<HandleTemporalPair> timestamps;
    for (std::vector<Handle>::const_iterator h_i = handles.begin();
            h_i != handles.end(); ++h_i) {
        Handle listLink = atomSpace.getOutgoing(*h_i, 1);
        OC_ASSERT(listLink != Handle::UNDEFINED,
                "ListLink must be defined");
        OC_ASSERT(atomSpace.getArity(listLink) == 2,
                "IsHolding predicate must have 2 arguments");
        // get only holder's eval-links occuring within 'time'

        if (atomSpace.getOutgoing(listLink, 0) == holderHandle) {
            logger().debug(
                         "AtomSpaceUtil - before '%d' timestamps for isHolding pred for '%s'.",
                         timestamps.size(), holderId.c_str());

            atomSpace.getTimeInfo(back_inserter(timestamps), *h_i,
                                  Temporal(time), TemporalTable::INCLUDES);

            logger().debug(
                         "AtomSpaceUtil - after '%d' timestamps for isHolding pred for '%s'.",
                         timestamps.size(), holderId.c_str());

        } // if
    } // for


    if (!timestamps.empty()) {
        unsigned int ts = timestamps.size();
        //there should be only one element because someone can only carry
        //one object at a time
        if (ts != 1) {
            std::string s("It is assumed that someone can only carry 1 object at a time and it seems that ");
            s += holderId;
            s += std::string(" carries ") + boost::lexical_cast<std::string>(ts) + std::string(" objects ");
            s += std::string("at time ") + boost::lexical_cast<std::string>(time);
            s += std::string(" listed as ");
            for (std::vector<HandleTemporalPair>::iterator tsi = timestamps.begin();
                    tsi != timestamps.end(); ++tsi) {
                s +=  std::string("'") + tsi->toString() + std::string("' ");
            }
            OC_ASSERT(false, s.c_str());
        }
        return timestamps[0].getHandle();
    } else return Handle::UNDEFINED;
}

std::string AtomSpaceUtil::getHoldingObjectIdAtTime(const AtomSpace& as,
        const std::string& holderId,
        unsigned long time)
{
    Handle holderHandle = getAgentHandle( as, holderId );
    if ( holderHandle == Handle::UNDEFINED ) {
        return "";
    } // if

    Handle objectHandle = getHoldingObjectHandleAtTime(as, holderId, time);
    if ( objectHandle == Handle::UNDEFINED ) {
        return "";
    } // if

    return as.getName(objectHandle);
}

std::string AtomSpaceUtil::getObjectName( const AtomSpace& atomSpace, 
                                          Handle object )
{
    std::string name("");

    if ( object != Handle::UNDEFINED ) {
        HandleSeq objectName(2);
        objectName[0] = Handle::UNDEFINED;
        objectName[1] = object;
        Type types[] = { WORD_NODE, atomSpace.getType( object ) };
        HandleSeq wrLinks;
        atomSpace.getHandleSet( back_inserter(wrLinks), objectName,
                                &types[0], NULL, 2, WR_LINK, false );
        if ( wrLinks.size( ) > 0 ) {
            name = atomSpace.getName( atomSpace.getOutgoing( wrLinks[0], 0 ) );
        } // if
    } // if

    return name;
}

std::string AtomSpaceUtil::getObjIdFromName( const AtomSpace& atomSpace,
        const std::string& objName )
{
    std::string result;
    Handle objIdHandle = Handle::UNDEFINED;
    Handle objNameHandle = atomSpace.getHandle(WORD_NODE, objName);
    if (objNameHandle != Handle::UNDEFINED) {
        HandleSeq outgoing;
        outgoing.push_back(objNameHandle);
        outgoing.push_back(Handle::UNDEFINED);
        HandleSeq wrLinks;
        atomSpace.getHandleSet(back_inserter(wrLinks), outgoing,
                               NULL, NULL, 2, WR_LINK, false);
        if (!wrLinks.empty()) {
            objIdHandle = atomSpace.getOutgoing(wrLinks[0], 1);
            // TODO: check for multiple answers...
        } else {
            logger().debug(
                          "AtomSpaceUtil::getObjIdFromName: Found avatar name '%s' but no avatar id associated to this name.",
                          objName.c_str());
        }
    } else {
        logger().debug(
                      "AtomSpaceUtil::getObjIdFromName: didn't find avatar name '%s'",
                      objName.c_str());
    }
    if (objIdHandle == Handle::UNDEFINED) {
        // try all letters in lowercase
        std::string lcObjName = objName;
        std::transform(lcObjName.begin(), lcObjName.end(),
                       lcObjName.begin(), (int(*)(int)) std::tolower);
        objNameHandle = atomSpace.getHandle(WORD_NODE, lcObjName);
        if (objNameHandle != Handle::UNDEFINED) {
            HandleSeq outgoing;
            outgoing.push_back(objNameHandle);
            outgoing.push_back(Handle::UNDEFINED);
            HandleSeq wrLinks;
            atomSpace.getHandleSet(back_inserter(wrLinks), outgoing,
                                   NULL, NULL, 2, WR_LINK, false);
            if (!wrLinks.empty()) {
                objIdHandle = atomSpace.getOutgoing(wrLinks[0], 1);
                // TODO: check for multiple answers...
            } else {
                logger().debug(
                              "AtomSpaceUtil::getObjIdFromName: Found avatar name '%s' but no avatar id associated to this name.",
                              lcObjName.c_str());
            }
        } else {
            logger().debug(
                          "AtomSpaceUtil::getObjIdFromName: didn't find avatar name '%s'",
                          lcObjName.c_str());
        }
    }
    if (objIdHandle == Handle::UNDEFINED) {
        // try only the first letter in uppercase, the remaining ones in lowercase
        std::string firstCapObjName = objName;
        std::transform(firstCapObjName.begin(), firstCapObjName.end(),
                       firstCapObjName.begin(), (int(*)(int)) std::tolower);
        firstCapObjName[0] = toupper(firstCapObjName[0]);
        objNameHandle = atomSpace.getHandle(WORD_NODE, firstCapObjName);
        if (objNameHandle != Handle::UNDEFINED) {
            HandleSeq outgoing;
            outgoing.push_back(objNameHandle);
            outgoing.push_back(Handle::UNDEFINED);
            HandleSeq wrLinks;
            atomSpace.getHandleSet(back_inserter(wrLinks), outgoing,
                                   NULL, NULL, 2, WR_LINK, false);
            if (!wrLinks.empty()) {
                objIdHandle = atomSpace.getOutgoing(wrLinks[0], 1);
                // TODO: check for multiple answers...
            } else {
                logger().debug(
                              "AtomSpaceUtil::getObjIdFromName: Found avatar name '%s' but no avatar id associated to this name.",
                              firstCapObjName.c_str());
            }
        } else {
            logger().debug(
                          "AtomSpaceUtil::getObjIdFromName: didn't find avatar name '%s'",
                          firstCapObjName.c_str());
        }
    }
    if (objIdHandle != Handle::UNDEFINED) {
        result = atomSpace.getName(objIdHandle);
    }
    logger().debug(
                  "AtomSpaceUtil::getObjIdFromName: returning '%s'",
                  result.c_str());
    return result;
}

Handle AtomSpaceUtil::getMostRecentPetSchemaExecLink(const AtomSpace& atomSpace,
        unsigned long timestamp,
        bool schemaSuccessful)
{

    logger().debug(
                 "AtomSpaceUtil - getMostRecentPetSchemaExecLink");
    std::vector<HandleTemporalPair> timestamps;
    Temporal t(timestamp);

    if (schemaSuccessful) {
        AtomSpaceUtil::getAllEvaluationLinks(atomSpace,
                                             timestamps,
                                             "SchemaDone",
                                             t, TemporalTable::STARTS_AFTER,
                                             true);
    } else {
        AtomSpaceUtil::getAllEvaluationLinks(atomSpace,
                                             timestamps,
                                             "SchemaFailure",
                                             t, TemporalTable::STARTS_AFTER,
                                             true);
    }

    if (timestamps.empty()) {
        return Handle::UNDEFINED;
    }

    for (int i = timestamps.size() - 1; i > 0; --i) {

        Handle evalLink = timestamps[i].getHandle();
        if (evalLink != Handle::UNDEFINED) {
            Handle listLink = atomSpace.getOutgoing(evalLink, 1);
            if (listLink != Handle::UNDEFINED) {
                Handle execLink = atomSpace.getOutgoing(listLink, 0);
                if (execLink != Handle::UNDEFINED
                        && atomSpace.getType(execLink) == EXECUTION_LINK) {
                    return execLink;
                }
            }
        }
    }
    return Handle::UNDEFINED;
}

std::string AtomSpaceUtil::convertPetExecLinkParametersToString(const AtomSpace& atomSpace,
        Handle execLink)
{

    if (execLink != Handle::UNDEFINED) {
        Handle listLink = atomSpace.getOutgoing(execLink, 1);
        if (listLink == Handle::UNDEFINED) {
            return "";
        } // if

        std::stringstream parameters;
        for (int i = 0; i < atomSpace.getArity(listLink); i++ ) {
            Handle schemaParam = atomSpace.getOutgoing(listLink, i);

            if (schemaParam == Handle::UNDEFINED) {
                logger().error(
                             "AtomSpaceUtil - Found no param for schema");
                return "";
            } // if

            if (i > 0) {
                parameters << ", ";
            } // if

            if (atomSpace.getType(schemaParam) == LIST_LINK) {
                // rotation or vector
                parameters << atomSpace.getName(atomSpace.getOutgoing(schemaParam, 0));
                parameters << ", ";
                parameters << atomSpace.getName(atomSpace.getOutgoing(schemaParam, 1));
                parameters << ", ";
                parameters << atomSpace.getName(atomSpace.getOutgoing(schemaParam, 2));
            } else {
                // entity
                parameters << atomSpace.getName(schemaParam);
            }
        } // for
        return parameters.str( );
    } // if
    return "";
}

Handle AtomSpaceUtil::getMostRecentAgentActionLink(const AtomSpace& atomSpace,
        const std::string& agentId,
        const Temporal& temporal,
        TemporalTable::TemporalRelationship criterion)
{


    Handle latestActionDoneLink = Handle::UNDEFINED;
    std::string agentType = "unknown";

    // reference:
    // https://extranet.vettalabs.com:8443/bin/view/Petaverse/PerceptionActionInterface

    // get eval links for all agents actions done
    std::vector<HandleTemporalPair> timestamps;
    AtomSpaceUtil::getAllEvaluationLinks( atomSpace, timestamps,
                                          ACTION_DONE_PREDICATE_NAME,
                                          temporal, criterion );

    Temporal previousTemporal(0);

    // filter eval links by agent name and most recent temporal
    unsigned int i;
    for ( i = 0; i < timestamps.size( ); ++i ) {
        Temporal& temporal = *( timestamps[i].getTemporal( ) );

        Handle evalLink = timestamps[i].getHandle( );

        if ( evalLink == Handle::UNDEFINED) {
            logger().error(
                         "AtomSpaceUtil - AtomSpace returned undefined handle for evaluation link (agent action done predicate)" );
            continue;
        } // if

        Handle agentActionLink = atomSpace.getOutgoing(evalLink, 1);

        if ( agentActionLink == Handle::UNDEFINED) {
            logger().error(
                         "AtomSpaceUtil - Found no agent action for actionDone predicate." );
            continue;
        } // if

        Handle agentIdNode = atomSpace.getOutgoing(agentActionLink, 0);
        if (agentIdNode == Handle::UNDEFINED ) {
            logger().error(
                         "AtomSpaceUtil - Found no agent name for actionDone predicate" );
            continue;
        } // if

        int inspectedAgentTypeCode = atomSpace.getType(agentIdNode);
        if ( !atomSpace.isNode( inspectedAgentTypeCode ) ) {
            logger().fine(
                         "AtomSpaceUtil - Skipping handles that isn't nodes. Inspected handle type: %d",
                         inspectedAgentTypeCode );
            continue;
        } // if

        const std::string& inspectedAgentId = atomSpace.getName( agentIdNode );

        if ( inspectedAgentId != agentId ) {
            logger().fine(
                         "AtomSpaceUtil - Inspected agent id is [%s; type=%d], but required is %s",
                         inspectedAgentId.c_str( ),
                         inspectedAgentTypeCode,
                         agentId.c_str( ) );
            // it is the wrong agent, then skip it
            continue;
        } // if

        Handle agentActionNode = atomSpace.getOutgoing(agentActionLink, 1);
        if (agentActionNode == Handle::UNDEFINED) {
            logger().error(
                         "AtomSpaceUtil - Found no agent action name for actionDone predicate" );
            continue;
        } // if

        logger().fine(
                     "AtomSpaceUtil - Previous temporal[%lu %lu], Inspected temporal[%lu %lu]",
                     previousTemporal.getA(),
                     previousTemporal.getB(),
                     temporal.getA(),
                     temporal.getB() );
        if ( temporal > previousTemporal ) {

            latestActionDoneLink = agentActionLink;
            previousTemporal = temporal;

            if ( inspectedAgentTypeCode == AVATAR_NODE ) {
                agentType = "an avatar";
            } else if ( inspectedAgentTypeCode == PET_NODE ) {
                agentType = "a pet";
            } else if ( inspectedAgentTypeCode == HUMANOID_NODE ) {
                agentType = "an humanoid";
            } else {
                logger().error(
                             "AtomSpaceUtil - Invalid agentIdNode type: %i",
                             inspectedAgentTypeCode );
                continue;
            } // else

        } // if

    } // for

    logger().debug(
                 "AtomSpaceUtil::getMostRecentAgentActionLink - Agent %s is %s",
                 agentId.c_str( ),
                 agentType.c_str( ) );

    return latestActionDoneLink;
}

Handle AtomSpaceUtil::getMostRecentAgentActionLink( AtomSpace& atomSpace,
        const std::string& agentId,
        const std::string& actionName,
        const Temporal& temporal,
        TemporalTable::TemporalRelationship criterion )
{

    Handle agentHandle = getAgentHandle( atomSpace, agentId );
    if ( agentHandle == Handle::UNDEFINED ) {
        logger().debug(
                     "AtomSpaceUtil - Found no agent identified by %s",
                     agentId.c_str( ) );
        return Handle::UNDEFINED;
    } // if

    Handle predicateNodeHandle = atomSpace.getHandle(PREDICATE_NODE,
                                 ACTION_DONE_PREDICATE_NAME);
    if ( predicateNodeHandle == Handle::UNDEFINED ) {
        logger().debug(
                     "AtomSpaceUtil - Found no predicate node named %s",
                     ACTION_DONE_PREDICATE_NAME )
        ;
        return Handle::UNDEFINED;
    } // if

    Handle actionNodeHandle = atomSpace.getHandle( NODE, actionName );
    if ( actionNodeHandle == Handle::UNDEFINED ) {
        logger().debug(
                     "AtomSpaceUtil - Found no NODE for action named %s",
                     actionName.c_str( ) );
        return Handle::UNDEFINED;
    } // if

#ifdef USE_GET_HANDLE_SET
    // retrieve all list_links which describes the given action
    HandleSeq listLinkDescription;
    listLinkDescription.push_back( agentHandle );
    listLinkDescription.push_back( actionNodeHandle );
    listLinkDescription.push_back( Handle::UNDEFINED ); // action parameters at a link list

    std::vector<Handle> handles;
    atomSpace.getHandleSet( back_inserter(handles),
                            listLinkDescription,
                            NULL, NULL, 3, LIST_LINK, false );

    if ( handles.size( ) == 0 ) {
        logger().debug(
                     "AtomSpaceUtil - the given agent wasn't executed group_command yet" );
        // there is no group command for the given agent
        return Handle::UNDEFINED;
    } // if
#else
    std::vector<Handle> handles;
    HandleSeq incomingSet = atomSpace.getIncoming(agentHandle);
    foreach(Handle incomingHandle, incomingSet) {
        Link* incomingLink = (Link*) TLB::getAtom(incomingHandle);
        if (incomingLink->getType() == LIST_LINK &&  incomingLink->getArity() == 3 && 
                incomingLink->getOutgoingHandle(0) == agentHandle && 
                incomingLink->getOutgoingHandle(1) == actionNodeHandle) {
            handles.push_back(incomingHandle);
        }
    }
#endif


    std::vector<HandleTemporalPair> timestamps;

    // filter and remove handles which isn't linked by an evaluation_link that has, as the first outgoing,
    // the predicate node handle ACTION_DONE_PREDICATE_NAME
    //std::vector<Handle> filteredHandles;
    unsigned int i;
    for ( i = 0; i < handles.size( ); ++i ) {
        HandleSeq incomingLinks = atomSpace.getIncoming(handles[i]);
        logger().debug(
                     "AtomSpaceUtil - %d incoming links were identified",
                     incomingLinks.size( ) );
        unsigned int j;
        for ( j = 0; j < incomingLinks.size( ); ++j ) {
            logger().debug(
                         "AtomSpaceUtil - %d) type[%s] EVAL_LINK[%lu], outgoing0[%lu] predicateNode[%lu]",
                         j,
                         classserver().getTypeName( atomSpace.getType( incomingLinks[j] ) ).c_str( ),
                         classserver().getTypeName( EVALUATION_LINK ).c_str( ),
                         atomSpace.getOutgoing( incomingLinks[j], 0 ).value(),
                         predicateNodeHandle.value(),
                         atomSpace.getOutgoing( incomingLinks[j], 1 ).value() );
            if ( atomSpace.getType( incomingLinks[j] ) == EVALUATION_LINK &&
                    atomSpace.getOutgoing( incomingLinks[j], 0 ) == predicateNodeHandle ) {
                atomSpace.getTimeInfo( back_inserter(timestamps),
                                       incomingLinks[j], temporal, criterion );

                //filteredHandles.push_back( incomingLinks[j] );
            } // if
        } // for
    } // for

    logger().debug(
                 "AtomSpaceUtil - %d handles (was)were identified after filter",
                 timestamps.size( ) );

    Temporal previousTemporal(0);
    Handle mostRecentGroupCommand = Handle::UNDEFINED;
    // filter eval links by agent name and most recent temporal
    for ( i = 0; i < timestamps.size( ); ++i ) {
        Temporal& temporal = *( timestamps[i].getTemporal( ) );
        if ( temporal > previousTemporal ) {
            mostRecentGroupCommand = timestamps[i].getHandle( );
            previousTemporal = temporal;
        } // if
    } // for

    logger().debug(
                 "AtomSpaceUtil - handle %s",
                 ( mostRecentGroupCommand != Handle::UNDEFINED ? "found" : "not found" ) );
    return mostRecentGroupCommand;
}


Handle AtomSpaceUtil::getMostRecentAgentActionLinkWithinTime( const AtomSpace& atomSpace,
        const std::string& agentId,
        unsigned long t1,
        unsigned long t2 )
{
    Temporal t(t1, t2);
    return getMostRecentAgentActionLink(atomSpace,
                                        agentId,
                                        t, TemporalTable::ENDS_WITHIN);
}

Handle AtomSpaceUtil::getMostRecentAgentActionLinkAfterTime( const AtomSpace& atomSpace,
        const std::string& agentId,
        unsigned long timestamp )
{
    Temporal t(timestamp);
    return getMostRecentAgentActionLink(atomSpace,
                                        agentId,
                                        t, TemporalTable::ENDS_AFTER);
}

std::string AtomSpaceUtil::convertAgentActionParametersToString( const AtomSpace& atomSpace,
        Handle agentActionLink )
{
    if ( agentActionLink != Handle::UNDEFINED ) {
        try {
            Handle predicateListLink = atomSpace.getOutgoing( agentActionLink,
                                       1 );
            if ( predicateListLink == Handle::UNDEFINED ) {
                logger().error(
                              "AtomSpaceUtil - There is no description for this action" );
                return "";
            } // if

            if ( atomSpace.getArity( predicateListLink ) <= 2 ) {
                logger().error(
                              "AtomSpaceUtil - There is no parameters on the given action" );
                return "";
            } // if

            Handle actionParametersLink = atomSpace.getOutgoing( predicateListLink, 2 );
            if ( actionParametersLink == Handle::UNDEFINED ) {
                logger().error(
                              "AtomSpaceUtil - There is no parameters on the given action" );
                return "";
            } // if

            int i;
            std::stringstream parameters;
            for ( i = 0; i < atomSpace.getArity(actionParametersLink); ++i ) {
                Handle actionParam = atomSpace.getOutgoing(actionParametersLink, i);
                if ( actionParam == Handle::UNDEFINED) {
                    logger().error(
                                 "AtomSpaceUtil - Found no param for action" );
                    return "";
                } // if

                if ( i > 0 ) {
                    parameters << "; ";
                } // if

                Type t = atomSpace.getType( actionParam );
                if (  (t == CONCEPT_NODE) //boolean or string
                        || (t == NUMBER_NODE) // int or float
                   ) {
                    parameters << atomSpace.getName( actionParam );
                } else if (t == LIST_LINK) { // rotation or vector
                    parameters << atomSpace.getName( atomSpace.getOutgoing( actionParam, 0 ) );
                    parameters << "; ";
                    parameters << atomSpace.getName( atomSpace.getOutgoing( actionParam, 1 ) );
                    parameters << "; ";
                    parameters << atomSpace.getName( atomSpace.getOutgoing( actionParam, 2 ) );
                } else {
                    // entity
                    parameters << atomSpace.getName( actionParam );
                }
            } // for
            return parameters.str( );
        } catch ( opencog::IndexErrorException& ex ) {
            logger().error("AtomSpaceUtil - Invalid outgoing: %s", ex.getMessage( ) );
        } // catch
    } // if
    return "";
}

Handle AtomSpaceUtil::getRuleImplicationLink(const AtomSpace& atomSpace,
        const std::string& rule)
{

    Handle rulePhraseNode = atomSpace.getHandle(PHRASE_NODE, rule);
    if (rulePhraseNode == Handle::UNDEFINED) {
        logger().error(
                     "AtomSpaceUtil - Found no PhraseNode for rule '%s'.",
                     rule.c_str());
        return Handle::UNDEFINED;
    }

    std::vector<Handle> ruleReferenceLink;
    atomSpace.getHandleSet(back_inserter(ruleReferenceLink),
                           rulePhraseNode, REFERENCE_LINK, false);
    if (ruleReferenceLink.size() != 1) {
        logger().error(
                     "AtomSpaceUtil - There should be exactly one ReferenceLink to rule '%s', found '%d'.",
                     rule.c_str(),
                     ruleReferenceLink.size());
        return Handle::UNDEFINED;
    }

    // handle to ImplicationLink
    Handle implicationLink = atomSpace.getOutgoing(ruleReferenceLink[0], 1);
    if (atomSpace.getType(implicationLink) != IMPLICATION_LINK) {
        logger().error(
                     "AtomSpaceUtil - Outgoing atom index [1] should be an ImplicationLink. Got '%s'.",
                     classserver().getTypeName(atomSpace.getType(implicationLink)).c_str());
        return Handle::UNDEFINED;
    }

    return implicationLink;
}

float AtomSpaceUtil::getRuleImplicationLinkStrength(const AtomSpace& atomSpace,
        const std::string& rule,
        const std::string& agentModeName )
{

    Handle implicationLink = AtomSpaceUtil::getRuleImplicationLink(atomSpace,
                             rule);
    if (implicationLink == Handle::UNDEFINED) {
        logger().error(
                     "AtomSpaceUtil - Found no ImplicationLink for rule '%s'.",
                     rule.c_str());
        return (-1.0f);
    }
    Handle agentModeNode = atomSpace.getHandle( CONCEPT_NODE, agentModeName );
    if ( agentModeNode == Handle::UNDEFINED ) {
        logger().error(
                     "AtomSpaceUtil - Found no Handle for the given agent mode '%s'.",
                     agentModeName.c_str());
        return (-1.0f);
    } // if

    // strength is given by link TruthValue
    return (atomSpace.getTV(implicationLink, VersionHandle( CONTEXTUAL,
                            agentModeNode ) ).toFloat());
}

spatial::math::Vector3 AtomSpaceUtil::getMostRecentObjectVelocity( const AtomSpace& atomSpace, const std::string& objectId, unsigned long afterTimestamp )
{
    // look for a velocity predicate at 2 RuleEngine cycles before the current cycle
    std::vector<HandleTemporalPair> timestamps;
    getAllEvaluationLinks( atomSpace, timestamps,
                           AGISIM_VELOCITY_PREDICATE_NAME,
                           Temporal( afterTimestamp ),
                           TemporalTable::ENDS_AFTER, true );

    // inspect velocities to find a target candidate velocity
    int i;
    for (i = timestamps.size() - 1; i > 0; --i) {
        Handle evalLink = timestamps[i].getHandle( );
        if ( evalLink != Handle::UNDEFINED
                && atomSpace.getOutgoing(evalLink, 1) != Handle::UNDEFINED ) {
            Handle listLink = atomSpace.getOutgoing(evalLink, 1);
            Handle currentHandle = atomSpace.getOutgoing(listLink, 0);
            if ( currentHandle == Handle::UNDEFINED
                    || atomSpace.getName(currentHandle) != objectId ) {
                continue;
            } // if
            // a velocity node was found to the given target
            return spatial::math::Vector3( atof( atomSpace.getName(atomSpace.getOutgoing(listLink, 1)).c_str() ),  // x
                                           atof( atomSpace.getName(atomSpace.getOutgoing(listLink, 2)).c_str() ),  // y
                                           atof( atomSpace.getName(atomSpace.getOutgoing(listLink, 3)).c_str() )); // z
        } // if
    } // for
    return spatial::math::Vector3( 0, 0, 0 );
}

Handle AtomSpaceUtil::getObjectHandle( const AtomSpace& atomSpace,
                                       const std::string& objectId )
{
    // concept node objects
    if (objectId == "food_bowl" || objectId == "water_bowl"
            || objectId == "pet_home") {
        // TODO: The concept nodes may not be present inside AtomSpace
        // if pet/agent do not perceive
        // any of these objects in the world.
        // For now, whenever it perceives any of them,
        // the concept node is created
        // and its LTI is set to 1 so that it is not forgotten
        // by STI decayment. This may not be the best approach though.
        //OC_ASSERT(as.getHandle(CONCEPT_NODE, objectId) != Handle::UNDEFINED);
        return atomSpace.getHandle(CONCEPT_NODE, objectId);
    } else { //Now let's deal with the default case
        HandleSeq tmp;
        atomSpace.getHandleSet(std::back_inserter(tmp),
                               ACCESSORY_NODE, objectId);
        if (tmp.empty()) { //it is not an accessory, let's try a structure
            atomSpace.getHandleSet(std::back_inserter(tmp),
                                   STRUCTURE_NODE, objectId);
        }

        //assume that structure and accessories have distinct id
        OC_ASSERT(tmp.size() <= 1);

        return tmp.empty() ? Handle::UNDEFINED : tmp.front();
    }
}

Handle AtomSpaceUtil::getAgentHandle( const AtomSpace& atomSpace,
                                      const std::string& agentId )
{
    Handle agentHandle = atomSpace.getHandle( PET_NODE, agentId );
    if ( agentHandle != Handle::UNDEFINED ) {
        logger().debug(
                      "AtomSpaceUtil - Agent is a pet" );
    } else {
        agentHandle = atomSpace.getHandle( AVATAR_NODE, agentId );
        if ( agentHandle != Handle::UNDEFINED ) {
            logger().debug(
                          "AtomSpaceUtil - Agent is an avatar" );
        } else {
            agentHandle = atomSpace.getHandle( HUMANOID_NODE, agentId );
            if ( agentHandle != Handle::UNDEFINED ) {
                logger().debug(
                              "AtomSpaceUtil - Agent is an humanoid" );
            } // if
        } // if
    } // if
    return agentHandle;
}

Temporal AtomSpaceUtil::getTemporal(AtomSpace& as, Handle atTimeLink)
{
    OC_ASSERT(atTimeLink != Handle::UNDEFINED,
            "No HandleTemporalPair correspond to Handle::UNDEFINED");
    OC_ASSERT(as.getType(atTimeLink) == AT_TIME_LINK,
            "The Atom %s must be an atTimeLink",
            TLB::getAtom(atTimeLink)->toString().c_str());
    Handle timeNode = as.getOutgoing(atTimeLink, 0);
    OC_ASSERT(as.getType(timeNode) == TIME_NODE,
            "The Atom %s must be a TimeNode",
            TLB::getAtom(timeNode)->toString().c_str());

    return Temporal::getFromTimeNodeName(as.getName(timeNode).c_str());
}

Handle AtomSpaceUtil::getTimedHandle(AtomSpace& as, Handle atTimeLink)
{
    OC_ASSERT(atTimeLink != Handle::UNDEFINED,
            "No HandleTemporalPair correspond to Handle::UNDEFINED");
    OC_ASSERT(as.getType(atTimeLink) == AT_TIME_LINK,
            "The Atom %s must be an atTimeLink", TLB::getAtom(atTimeLink)->toString().c_str());

    return as.getOutgoing(atTimeLink, 1);
}

// Initialize static variables that stores the LatestLinks for each type of information
std::map<Handle, Handle> AtomSpaceUtil::latestAgentActionDone;
std::map<Handle, Handle> AtomSpaceUtil::latestPhysiologicalFeeling;
std::map<Handle, Handle> AtomSpaceUtil::latestAvatarSayActionDone;
std::map<Handle, Handle> AtomSpaceUtil::latestAvatarActionDone;
std::map<Handle, Handle> AtomSpaceUtil::latestPetActionPredicate;
std::map<Handle, std::map<Handle, Handle> > AtomSpaceUtil::latestSpatialPredicate;
std::map<Handle, Handle> AtomSpaceUtil::latestSchemaPredicate;
boost::unordered_map<std::string, HandleSeq> AtomSpaceUtil::frameElementsCache;
Handle AtomSpaceUtil::latestIsExemplarAvatar = Handle::UNDEFINED;

void AtomSpaceUtil::updateGenericLatestInfoMap(std::map<Handle, Handle>& infoMap,
        AtomSpace& as,
        Handle atTimeLink,
        Handle key)
{
    std::map<Handle, Handle>::iterator itr = infoMap.find(key);
    if (itr != infoMap.end()) {
        as.removeAtom(itr->second);
    }
    HandleSeq hs;
    hs.push_back(atTimeLink);
    Handle latestLink = addLink(as, LATEST_LINK, hs, true);
    infoMap[key] = latestLink;
}

void AtomSpaceUtil::updateLatestAgentActionDone(AtomSpace& as,
        Handle atTimeLink,
        Handle agentNode)
{
    updateGenericLatestInfoMap(latestAgentActionDone,
                               as, atTimeLink, agentNode);
}

void AtomSpaceUtil::updateLatestPhysiologicalFeeling(AtomSpace& as,
        Handle atTimeLink,
        Handle predicateNode)
{
    updateGenericLatestInfoMap(latestPhysiologicalFeeling,
                               as, atTimeLink, predicateNode);
}

void AtomSpaceUtil::updateLatestAvatarSayActionDone(AtomSpace& as,
        Handle atTimeLink,
        Handle avatarNode)
{
    updateGenericLatestInfoMap(latestAvatarSayActionDone,
                               as, atTimeLink, avatarNode);
}

void AtomSpaceUtil::updateLatestAvatarActionDone(AtomSpace& as,
        Handle atTimeLink,
        Handle avatarNode)
{
    updateGenericLatestInfoMap(latestAvatarActionDone,
                               as, atTimeLink, avatarNode);
}

void AtomSpaceUtil::updateLatestPetActionPredicate(AtomSpace& as,
        Handle atTimeLink,
        Handle predicateNode)
{
    updateGenericLatestInfoMap(latestPetActionPredicate,
                               as, atTimeLink, predicateNode);
}

void AtomSpaceUtil::updateLatestSpatialPredicate(AtomSpace& as,
        Handle atTimeLink,
        Handle predicateNode,
        Handle objectNode)
{
    std::map<Handle, Handle> infoMap;
    std::map<Handle, std::map<Handle, Handle> >::iterator itr = latestSpatialPredicate.find(predicateNode);
    if (itr != latestSpatialPredicate.end()) {
        infoMap = itr->second;
    }
    updateGenericLatestInfoMap(infoMap, as, atTimeLink, objectNode);
    latestSpatialPredicate[predicateNode] = infoMap;
}

void AtomSpaceUtil::updateLatestSchemaPredicate(AtomSpace& as,
        Handle atTimeLink,
        Handle predicateNode)
{
    updateGenericLatestInfoMap(latestSchemaPredicate,
                               as, atTimeLink, predicateNode);
}

void AtomSpaceUtil::updateGenericLatestSingleInfo(Handle& latestSingleInfoHandle,
        AtomSpace& as,
        Handle atTimeLink)
{
    if (latestSingleInfoHandle != Handle::UNDEFINED) {
        as.removeAtom(latestSingleInfoHandle);
    }
    HandleSeq hs;
    hs.push_back(atTimeLink);
    latestSingleInfoHandle = addLink(as, LATEST_LINK, hs, true);
}

void AtomSpaceUtil::updateLatestIsExemplarAvatar(AtomSpace& as,
        Handle atTimeLink)
{
    updateGenericLatestSingleInfo(latestIsExemplarAvatar, as, atTimeLink);
}


Handle AtomSpaceUtil::getFrameElements( AtomSpace& atomSpace, const std::string& frameName, HandleSeq& frameElementsHandles ) 
{
    Handle frameNode = atomSpace.getHandle( DEFINED_FRAME_NODE, frameName );
    if ( frameNode != Handle::UNDEFINED ) {


        boost::unordered_map<std::string, HandleSeq>::const_iterator it =
            frameElementsCache.find( frameName );
        
        if ( it != frameElementsCache.end( ) ) {
            frameElementsHandles = it->second;
            return frameNode;
        } // if
        


#ifdef USE_GET_HANDLE_SET
        HandleSeq frameElementLink;
        frameElementLink.push_back(frameNode);
        frameElementLink.push_back(Handle::UNDEFINED);
        HandleSeq frameElementLinkHandles;
        atomSpace.getHandleSet(back_inserter(frameElementLinkHandles),
                               frameElementLink, NULL, NULL, 2, FRAME_ELEMENT_LINK, false);
        
        unsigned int i;
        for( i = 0; i < frameElementLinkHandles.size( ); ++i ) {
            frameElementsHandles.push_back( atomSpace.getOutgoing( frameElementLinkHandles[i], 1 ) );
        } // for


        // now get the parents of this frame
        HandleSeq parentLink;            
        parentLink.push_back( frameNode );
        parentLink.push_back( Handle::UNDEFINED );
        HandleSeq parentFrames;
        atomSpace.getHandleSet(back_inserter(parentFrames),
                               parentLink, NULL, NULL, 2, INHERITANCE_LINK, false);            
        for(unsigned int i = 0; i < parentFrames.size( ); ++i ) {
            Handle parentFrameHandle = atomSpace.getOutgoing( parentFrames[i], 1 );
            if ( atomSpace.getType( parentFrameHandle ) == DEFINED_FRAME_NODE ) {
                getFrameElements( atomSpace, atomSpace.getName( parentFrameHandle ), frameElementsHandles );
            } // if
        } // for
#else
        HandleSeq parentFrames;
        HandleSeq incomingSet = atomSpace.getIncoming(frameNode);
        foreach(Handle incomingHandle, incomingSet) {
            Link* incomingLink = (Link*) TLB::getAtom(incomingHandle);
            if (incomingLink->getType() == FRAME_ELEMENT_LINK &&  
                incomingLink->getArity() == 2 && 
                incomingLink->getOutgoingHandle(0) == frameNode) {
                frameElementsHandles.push_back(incomingLink->getOutgoingHandle(1));
            } else if (incomingLink->getType() == INHERITANCE_LINK &&  
                incomingLink->getArity() == 2 && 
                incomingLink->getOutgoingHandle(0) == frameNode) {
                parentFrames.push_back(incomingLink->getOutgoingHandle(1));
            }
        }
        foreach(Handle parentFrameHandle, parentFrames) {
            if ( atomSpace.getType( parentFrameHandle ) == DEFINED_FRAME_NODE ) {
                getFrameElements( atomSpace, atomSpace.getName( parentFrameHandle ), frameElementsHandles );
            } // if
        } // for
#endif

        frameElementsCache[frameName] = frameElementsHandles;

        return frameNode;
    } // if
    
    return Handle::UNDEFINED;
}

Handle AtomSpaceUtil::setPredicateFrameFromHandles( AtomSpace& atomSpace, const std::string& frameName, 
                                                    const std::string& frameInstanceName, 
                                                    const std::map<std::string, Handle>& frameElementsValuesHandles, 
                                                    const TruthValue& truthValue, bool permanent )
{
    Handle frameNode = atomSpace.getHandle( DEFINED_FRAME_NODE, frameName );

    if ( frameNode != Handle::UNDEFINED ) {

        // get all parents of this frame to copy its elements to this one
        std::map<std::string, Handle> frameElements;

        // get the frame elements
        HandleSeq frameElementsHandles;
        getFrameElements( atomSpace, atomSpace.getName( frameNode ), frameElementsHandles );
        unsigned int i;
        for( i = 0; i < frameElementsHandles.size( ); ++i ) {
            std::vector<string> elementNameParts;
            boost::algorithm::split( elementNameParts,
                                     atomSpace.getName( frameElementsHandles[i] ),
                                     boost::algorithm::is_any_of(":") );
            
            OC_ASSERT(elementNameParts.size( ) == 2,
                      "The name of a Frame element must be #FrameName:FrameElementName, but '%' was given",
                      atomSpace.getName( frameElementsHandles[i] ).c_str( ) );
            
            // only add a new element if it wasn't yet defined (overloading)
            if ( frameElements.find( elementNameParts[1] ) == frameElements.end( ) ) {
                frameElements.insert( std::map<std::string, Handle>::value_type(
                elementNameParts[1], frameElementsHandles[i] ) );
            } // if
        } // for
                

        // check if there are one element value for each Frame element
        if ( frameElements.size( ) > 0 ) {

            Handle frameInstance = addNode(atomSpace,
                                                 PREDICATE_NODE,
                                                 frameInstanceName, true);

            HandleSeq frameInstanceInheritance;
            frameInstanceInheritance.push_back( frameInstance );
            frameInstanceInheritance.push_back( frameNode );
            Handle frameInheritanceLink = addLink( atomSpace, INHERITANCE_LINK, frameInstanceInheritance, true );
            atomSpace.setTV( frameInheritanceLink, TruthValue::FALSE_TV() );
            
            std::map<std::string, Handle>::const_iterator it;
            for( it = frameElements.begin( ); it != frameElements.end( ); ++it ) {
                
                // the element exists, so get its handle and check it
                Handle frameElementHandle = it->second;
                if ( frameElementHandle == Handle::UNDEFINED ) {
                    logger().error( "AtomSpaceUtil::%s - Invalid Undefined frame element node for frame '%s'",
                                    __FUNCTION__, frameName.c_str( ) );
                    return Handle::UNDEFINED;
                } // if

                // then add a new eval link to a new element value if necessary
                std::map<std::string, Handle>::const_iterator itValue =
                    frameElementsValuesHandles.find( it->first );

                // new prepare the name of the element for using at the Frame instance
                std::stringstream frameElementInstanceName;
                frameElementInstanceName << frameInstanceName << "_" << it->first;

                Handle frameElementInstance = 
                    addNode( atomSpace, PREDICATE_NODE, 
                             frameElementInstanceName.str( ), true );

                // Remove any old value
                HandleSeq incomingSet = atomSpace.getIncoming(frameElementInstance);
                foreach(Handle incomingHandle, incomingSet) {
                    Link* incomingLink = (Link*) TLB::getAtom(incomingHandle);
                    if (incomingLink->getType() == EVALUATION_LINK &&  
                        incomingLink->getArity() == 2 && 
                        incomingLink->getOutgoingHandle(0) == frameElementInstance) {
                        atomSpace.removeAtom(incomingHandle);
                    } 
                }

                if ( itValue != frameElementsValuesHandles.end( ) && itValue->second != Handle::UNDEFINED ) {                
                    HandleSeq frameElementInheritance;
                    frameElementInheritance.push_back( frameElementInstance );
                    frameElementInheritance.push_back( frameElementHandle );
    
                    Handle frameElementInheritanceLink = addLink( atomSpace, INHERITANCE_LINK, frameElementInheritance, true );
                    if ( permanent ) {
                        atomSpace.setLTI( frameElementInheritanceLink, 1 );
                    } // if
                    atomSpace.setTV( frameElementInheritanceLink, truthValue );

                    HandleSeq predicateFrameElement;
                    predicateFrameElement.push_back( frameInstance );
                    predicateFrameElement.push_back( frameElementInstance );
                                
                    Handle frameElementLink = addLink( atomSpace, FRAME_ELEMENT_LINK, predicateFrameElement, true );
                    if ( permanent ) {
                        atomSpace.setLTI( frameElementLink, 1 );
                    } // if
                    atomSpace.setTV( frameElementLink, truthValue );

                    // set a new value to the frame element
                    HandleSeq predicateFrameValue(2);
                    predicateFrameValue[0] = frameElementInstance;
                    predicateFrameValue[1] = itValue->second;
                    Handle frameElementEvalLink = addLink( atomSpace, EVALUATION_LINK, predicateFrameValue, true );                    
                    if ( permanent ) {
                        atomSpace.setLTI( frameElementEvalLink, 1 );                
                    } // if
                    atomSpace.setTV( frameElementEvalLink, truthValue );
                } else {
                    // Remove any other atoms for representing this element, since it's not
                    // present anymore
                    HandleSeq incomingSet = atomSpace.getIncoming(frameElementInstance);
                    foreach(Handle incomingHandle, incomingSet) {
                        Link* incomingLink = (Link*) TLB::getAtom(incomingHandle);
                        if (incomingLink->getArity() == 2 && 
                           ((incomingLink->getType() == INHERITANCE_LINK &&   
                             incomingLink->getOutgoingHandle(0) == frameElementInstance) || 
                            (incomingLink->getType() == FRAME_ELEMENT_LINK &&  
                             incomingLink->getOutgoingHandle(1) == frameElementInstance))) {  
                            atomSpace.removeAtom(incomingHandle);
                        } 
                    }
                    atomSpace.removeAtom(frameElementInstance);
                }

            } // for

            if ( frameElementsValuesHandles.size( ) == 0 ) {
                std::stringstream msg;
                msg << "AtomSpaceUtil::%s - You created a Frame with Handle::UNDEFINED ";
                msg << "in all of its elements, what is probably a useless Frame. Frame name '%s' ";
                msg << " Frame instance name '%s'. # of Frame elements '%d' ";
                msg << " # of Values given '%d' ";
                logger().debug( msg.str( ).c_str( ), __FUNCTION__, frameName.c_str( ), frameInstanceName.c_str( ),
                                frameElements.size( ), frameElementsValuesHandles.size( ) );
            } // if

            atomSpace.setTV( frameInheritanceLink, truthValue );
            if ( permanent ) {
                atomSpace.setLTI( frameInheritanceLink, 1 );
            } // if
            return frameInstance;

        } else {            
            logger().debug( "AtomSpaceUtil::%s - There are no configured frame elements for frame '%s'", 
                            __FUNCTION__, frameName.c_str( ) );
        } // else
        
    } else {
        logger().debug( "AtomSpaceUtil::%s - There is no Registered Frame named '%s'", 
                        __FUNCTION__, frameName.c_str( ) );
    } // else

    return Handle::UNDEFINED;
}

std::map<std::string, Handle> AtomSpaceUtil::getFrameInstanceElementsValues( AtomSpace& atomSpace, Handle frameInstancePredicateNode ) 
{

    std::map<std::string, Handle> elementsValues;
    // first check if this is really a frame instance
    Type type = atomSpace.getType( frameInstancePredicateNode );
    const std::string& instanceName = atomSpace.getName( frameInstancePredicateNode );
    if ( atomSpace.getType( frameInstancePredicateNode ) != PREDICATE_NODE ) {
        logger().error(
            "AtomSpaceUtil::%s - The given handle isn't a PREDICATE_NODE: '%d'.", 
            __FUNCTION__, type );
        return elementsValues;
    } // if
    
    std::string frameName;
    { // check the inheritance
        HandleSeq inheritanceLink;
        inheritanceLink.push_back( frameInstancePredicateNode );
        inheritanceLink.push_back( Handle::UNDEFINED );

        Type inheritanceLinkTypes[] = { PREDICATE_NODE, DEFINED_FRAME_NODE };
        HandleSeq inheritanceLinks;
        atomSpace.getHandleSet( back_inserter( inheritanceLinks ),
                                inheritanceLink,
                                &inheritanceLinkTypes[0], NULL, 2, INHERITANCE_LINK, false );        
        if ( inheritanceLinks.size( ) > 0 ) {
            // ok it is a frame instance
            if ( inheritanceLinks.size( ) > 1 ) {
                logger().error(
                   "AtomSpaceUtil::%s - The given handle represents more than one instance of Frame, what is unacceptable. Only the first occurrence will be considered.", __FUNCTION__ );                
            } // if
            frameName = atomSpace.getName( atomSpace.getOutgoing( inheritanceLinks[0], 1 ) );
        } else {
            logger().debug(
                "AtomSpaceUtil::%s - The given handle isn't a Frame instance. It doesn't inherits from a DEFINED_FRAME_NODE: %s", __FUNCTION__, instanceName.c_str( ) );
            return elementsValues;
        } // else
    } // end block

    // get the Frame type elements handles
    HandleSeq frameElementsHandles;
    Handle frame = getFrameElements( atomSpace, frameName, frameElementsHandles );
    // get the predicate nodes that represents the frame instance elements

    unsigned int j;
    //    for( j = 0; j < frameElements.size( ); ++j ) {
    for( j = 0; j < frameElementsHandles.size( ); ++j ) {
        // well, everything is ok with the Frame element, so
        // put it into the result map;
        std::vector<string> elementNameParts;
        boost::algorithm::split( elementNameParts,
                                 atomSpace.getName( frameElementsHandles[j] ),
                                 boost::algorithm::is_any_of(":") );
        std::stringstream elementName;
        elementName << instanceName << "_" << elementNameParts[1];

        Handle elementPredicate = atomSpace.getHandle( PREDICATE_NODE, elementName.str( ) );
        if ( elementPredicate == Handle::UNDEFINED ) {
            continue;
        } // if

        

        HandleSeq elementValue;
        elementValue.push_back( elementPredicate );
        elementValue.push_back( Handle::UNDEFINED );
        
        HandleSeq elementValues;
        atomSpace.getHandleSet( back_inserter( elementValues ),
                                elementValue, NULL, NULL, 2, EVALUATION_LINK, false );

        if ( elementValues.size( ) > 0 ) {
            if ( elementValues.size( ) != 1 ) {
                logger().error( "AtomSpaceUtil::%s - An invalid number of values was defined for the element: %s -> %d", __FUNCTION__, elementName.str( ).c_str( ), elementValues.size( ) );
            } // if
            elementsValues[elementNameParts[1]] = atomSpace.getOutgoing( elementValues[0], 1 );
        } // if
    } // for
    return elementsValues;
}


HandleSeq AtomSpaceUtil::retrieveFrameInstancesUsingAnElementValue( AtomSpace& atomSpace, const std::string& frameName, Handle aElementValue ) 
{

    HandleSeq instances;

    HandleSeq frameElementsHandles;
    Handle frame = getFrameElements( atomSpace, frameName, frameElementsHandles );

    if ( frame == Handle::UNDEFINED ) {
        logger().error(
                     "AtomSpaceUtil::%s - Invalid given frame name '%s'.",
                     __FUNCTION__, frameName.c_str());
        return instances;
    } // if

    // get the predicate node associated with the element value
    HandleSeq evalLink;
    evalLink.push_back( Handle::UNDEFINED );
    evalLink.push_back( aElementValue );
    HandleSeq evalLinks;
    Type evalLinkTypes[] = {PREDICATE_NODE, atomSpace.getType( aElementValue ) };
    atomSpace.getHandleSet( back_inserter( evalLinks ),
                            evalLink,
                            &evalLinkTypes[0], NULL, 2, EVALUATION_LINK, false );

    // now check if the predicate nodes belongs to a specific frame instance    
    unsigned int i;
    for( i = 0; i < evalLinks.size( ); ++i ) {
        // test against all the frame elements
        unsigned int j;
        for( j = 0; j < frameElementsHandles.size( ); ++j ) {            
            HandleSeq inheritanceElements(2);
            inheritanceElements[0] = atomSpace.getOutgoing( evalLinks[i], 0 );
            inheritanceElements[1] = frameElementsHandles[j];
            Handle inheritance = 
                atomSpace.getHandle(INHERITANCE_LINK, inheritanceElements );
            if ( inheritance != Handle::UNDEFINED ) {
                // ok, it is part of a frame instance, now get the frame
                // predicate

                // get all the predicate nodes that is part of a FrameElementLink
                HandleSeq frameElementLink;
                frameElementLink.push_back( Handle::UNDEFINED );
                frameElementLink.push_back( atomSpace.getOutgoing( evalLinks[i], 0 ) );
                
                HandleSeq frameElementLinks;
                Type frameElementLinkTypes[] = { PREDICATE_NODE, PREDICATE_NODE };
                atomSpace.getHandleSet( back_inserter( frameElementLinks ),
                                        frameElementLink,
                                        &frameElementLinkTypes[0], NULL, 2,
                                        FRAME_ELEMENT_LINK, false );
                // finally check if the upper predicate node inherits from
                // the correct frame node
                unsigned int k;
                for( k = 0; k < frameElementLinks.size( ); ++k ) {
                    inheritanceElements[0] = atomSpace.getOutgoing( frameElementLinks[k], 0 );
                    inheritanceElements[1] = frame;

                    inheritance = 
                        atomSpace.getHandle(INHERITANCE_LINK, inheritanceElements );
                    if ( inheritance != Handle::UNDEFINED ) {
                        // Ah, we found a predicate node that identifies a Frame instance
                        instances.push_back( atomSpace.getOutgoing( frameElementLinks[k], 0) );
                    } // if
                                                      
                } // for

            } // if

        } // for                                   
    } // for

    return instances;
    
}

void AtomSpaceUtil::deleteFrameInstance( AtomSpace& atomSpace, Handle frameInstance ) 
{

    // first check if this is really a frame instance
    if ( atomSpace.getType( frameInstance ) != PREDICATE_NODE ) {
        logger().error(
                       "AtomSpaceUtil::%s - The given handle isn't a PREDICATE_NODE: '%d'.", 
                       __FUNCTION__, atomSpace.getType( frameInstance ) );
        return;
    } // if

    std::string frameName;
    { // check the inheritance
#ifdef USE_GET_HANDLE_SET
        HandleSeq inheritanceLink;
        inheritanceLink.push_back( frameInstance );
        inheritanceLink.push_back( Handle::UNDEFINED );

        Type inheritanceLinkTypes[] = { PREDICATE_NODE, DEFINED_FRAME_NODE };
        HandleSeq inheritanceLinks;
        atomSpace.getHandleSet( back_inserter( inheritanceLinks ),
                                inheritanceLink,
                                &inheritanceLinkTypes[0], NULL, 2, INHERITANCE_LINK, false );

        if ( inheritanceLinks.size( ) > 0 ) {
            // ok it is a frame instance
            if ( inheritanceLinks.size( ) > 1 ) {
                logger().error(
                   "AtomSpaceUtil::%s - The given handle represents more than one instance of Frame, what is unacceptable. Only the first occurrence will be considered.", __FUNCTION__ );
            } // if
            frameName = atomSpace.getName( atomSpace.getOutgoing( inheritanceLinks[0], 1 ) );
            atomSpace.removeAtom( inheritanceLinks[0] );
        } else {
            logger().debug(
                "AtomSpaceUtil::%s - The given handle isn't a Frame instance. It doesn't inherits from a DEFINED_FRAME_NODE.", __FUNCTION__ );
            return;
        } // else
#else 
        bool found = false;
        HandleSeq incomingSet = atomSpace.getIncoming(frameInstance);
        foreach(Handle incomingHandle, incomingSet) { 
            Link* incomingLink = (Link*) TLB::getAtom(incomingHandle);
            if (incomingLink->getType() == INHERITANCE_LINK) {
                if (incomingLink->getArity() == 2 && incomingLink->getOutgoingHandle(0) == frameInstance) {
                    Handle targetHandle = incomingLink->getOutgoingHandle(1);
                    Atom* targetAtom = TLB::getAtom(targetHandle);
                    if (targetAtom->getType() == DEFINED_FRAME_NODE) {
                        found = true;
                        atomSpace.removeAtom(incomingHandle);
                        break; // discard aditional inheritance, if any
                    }
                }
            }
        } 
        if (!found) {
            logger().debug(
                "AtomSpaceUtil::%s - The given handle isn't a Frame instance. It doesn't inherit from a DEFINED_FRAME_NODE.", __FUNCTION__ );
            return;
        }
#endif
        
    } // end block

    std::string instanceName = atomSpace.getName( frameInstance );

#ifdef USE_GET_HANDLE_SET
    HandleSeq frameElement;
    frameElement.push_back( frameInstance );
    frameElement.push_back( Handle::UNDEFINED );
    Type frameElementTypes[] = { PREDICATE_NODE, PREDICATE_NODE };
    HandleSeq frameElements;
    atomSpace.getHandleSet( back_inserter( frameElements ),
                            frameElement, &frameElementTypes[0], NULL, 2, FRAME_ELEMENT_LINK, false );
#else
    HandleSeq frameElements;
    HandleSeq incomingSet = atomSpace.getIncoming(frameInstance);
    foreach(Handle incomingHandle, incomingSet) {
        Link* incomingLink = (Link*) TLB::getAtom(incomingHandle);
        if (incomingLink->getType() == FRAME_ELEMENT_LINK) {
            if (incomingLink->getArity() == 2 && incomingLink->getOutgoingHandle(0) == frameInstance) {
                Handle targetHandle = incomingLink->getOutgoingHandle(1);
                Atom* targetAtom = TLB::getAtom(targetHandle);
                if (targetAtom->getType() == PREDICATE_NODE) {
                    frameElements.push_back(incomingHandle);
                }
            }
        }
    }
#endif
    unsigned int j;
    for( j = 0; j < frameElements.size( ); ++j ) {
        Handle elementPredicate = atomSpace.getOutgoing( frameElements[j], 1 );
            
        // check if the elements are part of the same frame instance
#ifdef USE_GET_HANDLE_SET
        HandleSeq inheritance;
        inheritance.push_back( elementPredicate );
        inheritance.push_back( Handle::UNDEFINED );
        
        Type inheritanceTypes[] = { PREDICATE_NODE, DEFINED_FRAME_ELEMENT_NODE };
        HandleSeq inheritances;
        atomSpace.getHandleSet( back_inserter( inheritances ),
                                inheritance, &inheritanceTypes[0], NULL, 2, INHERITANCE_LINK, false );
        if ( inheritances.size( ) != 1 ) {
            logger().error( "AtomSpaceUtil::%s - Invalid frame instance. It has a predicate node linked by a InheritanceLink that isn't a DefinedFrameElementNode", __FUNCTION__ );
            return;
        } // if
#else
        Handle inheritanceLink = Handle::UNDEFINED;
        HandleSeq elemIncomingSet = atomSpace.getIncoming(elementPredicate);
        foreach (Handle elemIncomingHandle, elemIncomingSet) {
            Link* elemIncomingLink = (Link*) TLB::getAtom(elemIncomingHandle);
            if (elemIncomingLink->getType() == INHERITANCE_LINK) {
                if (elemIncomingLink->getArity() == 2 && elemIncomingLink->getOutgoingHandle(0) == elementPredicate) {
                    Handle targetHandle = elemIncomingLink->getOutgoingHandle(1);
                    Atom* targetAtom = TLB::getAtom(targetHandle);
                    if (targetAtom->getType() == DEFINED_FRAME_ELEMENT_NODE) {
                        inheritanceLink = elemIncomingHandle;
                        break;
                    }
                }
            }
        }
        if (inheritanceLink == Handle::UNDEFINED) {
            logger().error( "AtomSpaceUtil::%s - Invalid frame instance. It has a predicate node linked by a InheritanceLink that isn't a DefinedFrameElementNode", __FUNCTION__ );
            return;
        }
#endif

        // check if the elements are part of the same frame instance
#ifdef USE_GET_HANDLE_SET
        HandleSeq evaluation;
        evaluation.push_back( elementPredicate );
        evaluation.push_back( Handle::UNDEFINED );
        
        HandleSeq values;
        atomSpace.getHandleSet( back_inserter( values ),
                                evaluation, NULL, NULL, 2, EVALUATION_LINK, false );
        if ( values.size( ) != 1 ) {
            logger().error( "AtomSpaceUtil::%s - Invalid number of element value. It should be just one but was %d.", 
                            __FUNCTION__, values.size( ) );
            return;
        } // if
#else
        Handle valueHandle = Handle::UNDEFINED;
        foreach (Handle elemIncomingHandle, elemIncomingSet) {
            Link* elemIncomingLink = (Link*) TLB::getAtom(elemIncomingHandle);
            if (elemIncomingLink->getType() == EVALUATION_LINK) {
                if (elemIncomingLink->getArity() == 2 && elemIncomingLink->getOutgoingHandle(0) == elementPredicate) {
                    valueHandle = elemIncomingHandle;
                    break;
                }
            }
        }
        if (valueHandle == Handle::UNDEFINED) {
            logger().error( "AtomSpaceUtil::%s - Did not find element value", __FUNCTION__);
            return;
        }
#endif
      
        // first, remove the evaluation link
        HandleSeq elementPair(2);
        elementPair[0] = elementPredicate;
#ifdef USE_GET_HANDLE_SET
        elementPair[1] = atomSpace.getOutgoing( values[0], 1 );
#else
        elementPair[1] = atomSpace.getOutgoing( valueHandle, 1 );
#endif

        Handle link = atomSpace.getHandle( EVALUATION_LINK, elementPair );
        atomSpace.removeAtom( link );

        // second, remove the frame element link       
        elementPair[0] = frameInstance;
        elementPair[1] = elementPredicate;
        link = atomSpace.getHandle( FRAME_ELEMENT_LINK, elementPair );
        atomSpace.removeAtom( link );


        // then, remove the inheritance link
#ifdef USE_GET_HANDLE_SET
        atomSpace.removeAtom( inheritances[0] );
#else
        atomSpace.removeAtom( inheritanceLink );
#endif

        // finaly, remove the element predicate node
        atomSpace.removeAtom( elementPredicate );

        
    } // for

}
