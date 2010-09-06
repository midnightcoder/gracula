/*
 * opencog/embodiment/WorldWrapper/PAIWorldWrapper.h
 *
 * Copyright (C) 2002-2009 Novamente LLC
 * All Rights Reserved
 * Author(s): Nil Geisweiller
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

#ifndef _PAIWORLDWRAPPER_H
#define _PAIWORLDWRAPPER_H

#include "WorldWrapper.h"
#include <opencog/util/exceptions.h>
#include <opencog/embodiment/Control/PerceptionActionInterface/PAI.h>

#include <exception>

namespace WorldWrapper
{

class PAIWorldWrapper : public WorldWrapperBase
{
public:

    //ctor, dtor
    PAIWorldWrapper(PerceptionActionInterface::PAI& pai, opencog::RandGen& _rng);
    ~PAIWorldWrapper();

    /**
     * return true is the current action plan is finished
     * false otherwise or if there is no action plan
     */
    bool isPlanFinished() const;

    /**
     * return true if the plan has failed
     * false otherwise
     * pre-condition : the plan is finished
     */
    bool isPlanFailed() const;

    /**
     * Send a sequence of sequential_and actions [from, to)
     * returns true iff and action plan gets executed
     */
    bool sendSequential_and(sib_it from, sib_it to) throw (opencog::ComboException,
            opencog::AssertionException,
            std::bad_exception);

    /**
     * evaluate a perception
     */
    combo::vertex evalPerception(pre_it per,
                                 combo::variable_unifier& vu = combo::variable_unifier::DEFAULT_VU());

    /**
     * evaluate an indefinite object
     */
    combo::vertex evalIndefiniteObject(combo::indefinite_object io,
                                       combo::variable_unifier& vu = combo::variable_unifier::DEFAULT_VU());

private:
    //attributes
    PerceptionActionInterface::PAI& _pai;
    opencog::RandGen& rng;
    PerceptionActionInterface::ActionPlanID _planID;

    bool _hasPlanFailed; //sometimes it is possible to know in advance
    //whether the plan has failed without sending it
    //if _planHasFailed is true then we know for sure
    //that the plan has already failed
    //otherwise we don't know

    //methods

    /**
     * If pet is located at an invalid position, this method will return
     * a valid position
     * @param petLocation current pet location
     * @return a valid pet location
     */
    spatial::Point getValidPosition( const spatial::Point& location );

    /**
     * This method will try to makes path even smoother,
     * reducing the number of walks
     * @param actions walks processed by pathplanner
     * @param startPoint Current pet position
     * @param endPoint Goal position
     */
    void clearPlan( std::vector<spatial::Point>& actions,
                    const spatial::Point& startPoint,
                    const spatial::Point& endPoint );

    /**
     * Given a start and an end point, return the waypoints necessary to travel between them
     *
     * @param startPoint Start point
     * @param endPoint End point
     * @param actions Return vector actions
     */
    void getWaypoints( const spatial::Point& startPoint, const spatial::Point& endPoint, std::vector<spatial::Point>& actions );

    /**
     * Uses the current PathPlanner (HPA, Astar, TangentBUG, etc. )
     * to build the path plan
     * to achieve a given position
     * @param position Goal position
     * @return false if path planning has failed and true if successfull
     */
    bool buildGotoPlan( const spatial::Point& position, float customSpeed = 0 );

    /**
     * Create a walk planning action that will be sent to OPC
     * @param actions Calculated path plan
     * @param useExistingID If this method get called two or more times
     * and this var is true, the new walks will be added to the same walk plan
     * @param tuNudge nudge actions will be added to walk plan if it is
     *                not null
     */
    bool createWalkPlanAction( std::vector<spatial::Point>& actions,
                               bool useExistingID = false,
                               Handle toNudge = Handle::UNDEFINED,
                               float customSpeed = 0);


    //builds plans for actions relying on goto (goto_obj, follow, etc)
    bool build_goto_plan(Handle, bool useExistingID = false,
                         Handle nudgeHandle = Handle::UNDEFINED, Handle goBehind = Handle::UNDEFINED, float walkSpeed = 0);

    //synthesize a PetAction obj from a subtree
    PerceptionActionInterface::PetAction buildPetAction(sib_it from);

    //???
    std::string toCamelCase(std::string str);

    //type of a handle -> string
    std::string resolveType(combo::vertex);
    std::string resolveType(Handle);

    //convenience reference to string representing our pet ("self" in combo)
    std::string selfName();

    //convenience reference to string representing the owner ("owner" in combo)
    std::string ownerName();

    //determing angle an slobject is facing based on atomtable lookup
    double getAngleFacing(Handle) throw (opencog::ComboException,
                                         opencog::AssertionException,
                                         std::bad_exception);

    //convenience conversion function
    Handle toHandle(combo::definite_object);

    bool eval_percept_with_pai(pre_it it);

};
}

#endif
