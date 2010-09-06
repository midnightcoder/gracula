/*
 * opencog/embodiment/WorldWrapper/PAIWorldWrapper.cc
 *
 * Copyright (C) 2002-2009 Novamente LLC
 * All Rights Reserved
 * Author(s): Nil Geisweiller, Moshe Looks
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


#include "PAIWorldWrapper.h"
#include "WorldWrapperUtil.h"
#include <opencog/spatial/TangentBug.h>
#include <opencog/embodiment/Control/PerceptionActionInterface/PVPXmlConstants.h>
#include <opencog/spatial/AStarController.h>
#include <opencog/atomspace/Node.h>
#include <boost/bind.hpp>
#include <opencog/embodiment/Control/MessagingSystem/NetworkElement.h>
#include <opencog/util/RandGen.h>
#include <opencog/embodiment/AtomSpaceExtensions/PredefinedProcedureNames.h>
#include <opencog/spatial/HPASearch.h>
#include <opencog/embodiment/PetComboVocabulary/PetComboVocabulary.h>

#include <boost/scoped_ptr.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

#include <sstream>
#include <limits>


// The percentage bellow is related to the diagonal of the SpaceMap
#define STEP_SIZE_PERCENTAGE 2.0

#define MAIN_LOGGER_ACTION_PLAN_FAILED logger().debug("PAIWorldWrapper - No action plan has been sent");
#define MAIN_LOGGER_ACTION_PLAN_SUCCEEDED logger().debug("PAIWorldWrapper - The action plan has been successfully sent");

namespace WorldWrapper
{

using namespace PetCombo;
//  using namespace Spatial;
using namespace opencog;
using namespace PerceptionActionInterface;

/**
 * public methods
 */

//ctor, stor
PAIWorldWrapper::PAIWorldWrapper(PAI& pai, opencog::RandGen& _rng)
        : _pai(pai), rng(_rng), _hasPlanFailed(false) { }

PAIWorldWrapper::~PAIWorldWrapper() { }


bool PAIWorldWrapper::isPlanFinished() const
{
    return _pai.isPlanFinished(_planID);
}

bool PAIWorldWrapper::isPlanFailed() const
{
    return _hasPlanFailed || _pai.hasPlanFailed(_planID);
}

bool PAIWorldWrapper::sendSequential_and(sib_it from, sib_it to)
throw (opencog::ComboException, opencog::AssertionException, std::bad_exception)
{

    //DEBUG log
    if (logger().isDebugEnabled()) {
        string actionPlanStr = "{";
        for (sib_it sib = from; sib != to;) {
            stringstream ss;
            ss << combo_tree(sib);
            actionPlanStr += ss.str();
            ++sib;
            if (sib != to)
                actionPlanStr += ", ";
        }
        actionPlanStr += "}";
        logger().debug(
                     "PAIWorldWrapper - Attempt to send the following sequence of actions: %s",
                     actionPlanStr.c_str());
    }
    //~DEBUG log

    _hasPlanFailed = false;
    _planID = _pai.createActionPlan();

    const AtomSpace& as = _pai.getAtomSpace();
    const SpaceServer::SpaceMap& sm = as.getSpaceServer().getLatestMap();
    //treat the case when the action is a compound
    if (WorldWrapperUtil::is_builtin_compound_action(*from)) {
        OC_ASSERT(++sib_it(from) == to); //there is only one compound action
        pre_it it = from;
        builtin_action ba = get_builtin_action(*it);
        pet_builtin_action_enum bae = get_enum(ba);
        switch (bae) {
        case id::goto_obj: {
            logger().debug(
                         "PAIWorldWrapper - Handling goto_obj. # of parameters = %d", it.number_of_children() );
            OC_ASSERT(it.number_of_children() == 2);
            OC_ASSERT(is_definite_object(*it.begin()));
            OC_ASSERT(is_contin(*++it.begin()));


            std::string target = get_definite_object( *it.begin() );
            float walkSpeed = get_contin( *++it.begin() );
            logger().debug(
                         "PAIWorldWrapper - goto_obj(%s, %f)", target.c_str( ), walkSpeed );
            if ( target == "custom_path" ) {
                std::string customWaypoints =
                    _pai.getPetInterface( ).getCurrentModeHandler( ).getPropertyValue( "customPath" );

                std::vector<spatial::Point> actionPlan;

                std::vector<std::string> strWayPoints;
                boost::algorithm::split( strWayPoints, customWaypoints, boost::algorithm::is_any_of(";") );

                spatial::Point wayPoint;
                unsigned int i;
                for ( i = 0; i < strWayPoints.size( ); ++i ) {
                    std::istringstream parser( strWayPoints[i] );
                    parser >> wayPoint.first;
                    parser >> wayPoint.second;

                    if ( i > 0 ) {
                        // TODO: use the pathfinding algorithm to go to this wayPoint
                        actionPlan.push_back( wayPoint );
                    } else {
                        std::istringstream nextParser( strWayPoints[i+1] );
                        spatial::Point nextWaypoint;
                        nextParser >> nextWaypoint.first;
                        nextParser >> nextWaypoint.second;

                        getWaypoints( wayPoint, nextWaypoint, actionPlan );
                        ++i;
                    } // else
                } // for

                // register seeking object
                _pai.getPetInterface( ).setLatestGotoTarget( std::pair<std::string, spatial::Point>( target, wayPoint ) );

                if ( _hasPlanFailed || !createWalkPlanAction( actionPlan, false, Handle::UNDEFINED, walkSpeed ) ) {
                    if (_hasPlanFailed) {
                        logger().error("PAIWorldWrapper - Failed to create a goto plan to the goal.");
                    } else {
                        logger().info(
                                     "PAIWorldWrapper - No goto plan needed since the goal was already near enough.");
                    } // else
                } // if

                std::vector<std::string> arguments;
                _pai.getPetInterface( ).getCurrentModeHandler( ).handleCommand( "followCustomPathDone", arguments );
            } else {

                if ( target == "custom_object" ) {
                    target = _pai.getPetInterface( ).getCurrentModeHandler( ).getPropertyValue( "customObject" );
                } // if
                Handle targetHandle = toHandle( target );
                try {
                    sm.getEntity( as.getName( targetHandle ) );

                    if (!build_goto_plan(targetHandle, false, Handle::UNDEFINED, Handle::UNDEFINED, walkSpeed )) {
                        if (_hasPlanFailed) {
                            logger().error("PAIWorldWrapper - Failed to create a goto plan to the goal.");
                        } else {
                            logger().info("PAIWorldWrapper - No goto plan needed since the goal was already near enough.");
                            std::vector<std::string> arguments;
                            _pai.getPetInterface( ).getCurrentModeHandler( ).handleCommand( "gotoDone", arguments );

                        } // else
                        MAIN_LOGGER_ACTION_PLAN_FAILED; //macro see top of the file
                        return false;
                    } // if

                } catch ( NotFoundException& ex ) {
                    logger().error("PAIWorldWrapper - Goto: target not found.");
                    _hasPlanFailed = true;
                    MAIN_LOGGER_ACTION_PLAN_FAILED; //macro see top of the file
                    return false;
                } // catch

                std::vector<std::string> arguments;
                _pai.getPetInterface( ).getCurrentModeHandler( ).handleCommand( "gotoDone", arguments );
            } // else

        }
        break;

        case id::gonear_obj: {
            try {
                OC_ASSERT(it.number_of_children() == 2);
                OC_ASSERT(is_definite_object(*it.begin()));
                OC_ASSERT(is_contin(*++it.begin()));

                std::string target = get_definite_object( *it.begin() );
                float walkSpeed = get_contin( *++it.begin() );

                if ( target == "custom_object" ) {
                    target = _pai.getPetInterface( ).getCurrentModeHandler( ).getPropertyValue( "customObject" );
                } // if

                const spatial::EntityPtr& entity = sm.getEntity( target );

                // 3,0% of the maximum world horizontal distance
                float distanceFromGoal = fabs( sm.xMax( ) - sm.xMin( ) ) * 0.025;

                // get the object's direction vector (where it's face is pointing)
                spatial::math::Vector3 direction( entity->getDirection( ) );

                spatial::Point position = sm.getNearFreePointAtDistance(
                                              spatial::Point( entity->getPosition( ).x, entity->getPosition( ).y ),
                                              distanceFromGoal,
                                              spatial::Point( direction.x, direction.y )
                                          );

                logger().debug("PAIWorldWrapper - gonear_obj(%s) calculated position(%f, %f) target orientation(%s) target location(%s) distance from target(%f)", target.c_str( ), position.first, position.second, entity->getOrientation( ).toString( ).c_str( ), entity->getPosition( ).toString( ).c_str( ), distanceFromGoal );

                if ( !buildGotoPlan( position, walkSpeed ) ) {
                    if ( _hasPlanFailed ) {
                        logger().error(
                                     "PAIWorldWrapper - Failed to create a goto plan to the goal.");
                    } else {
                        logger().info(
                                     "PAIWorldWrapper - No goto plan needed since the goal was already near enough.");

                    } // else
                    MAIN_LOGGER_ACTION_PLAN_FAILED; //macro see top of the file
                    return false;
                } // if
            } catch ( NotFoundException& ex ) {
                // Pet will stay at it's current position until it's owner isn't at a valid position
                logger().error(
                             "PAIWorldWrapper - Goto: target not found.");
                _hasPlanFailed = true;
                MAIN_LOGGER_ACTION_PLAN_FAILED; //macro see top of the file
                return false;
            } // catch

        }
        break;

        case id::gobehind_obj: {

            OC_ASSERT(it.number_of_children() == 2);
            OC_ASSERT(is_definite_object(*it.begin()));
            OC_ASSERT(is_contin(*++it.begin()));

            std::string target = get_definite_object( *it.begin() );
            float walkSpeed = get_contin( *++it.begin() );

            if ( target == "custom_object" ) {
                target = _pai.getPetInterface( ).getCurrentModeHandler( ).getPropertyValue( "customObject" );
            } // if

            if (!WorldWrapperUtil::inSpaceMap(sm, as, selfName(), ownerName(),  target ) ) { //can't find the target
                logger().error(
                             "PAIWorldWrapper - gobehind_obj: target[%s] not found.",
                             target.c_str( ) );
                _hasPlanFailed = true;
                MAIN_LOGGER_ACTION_PLAN_FAILED; //macro see top of the file
                return false;
            } // if

            Handle targetHandle = toHandle( target );
            //const std::string& targetId = as.getName( targetHandle );

            //const SpaceServer::ObjectMetadata& md = sm.getMetaData( targetId );
            const spatial::EntityPtr& entity = sm.getEntity( target );

            //SpaceServer::SpaceMapPoint goalLocation =
            //  WorldWrapperUtil::getLocation(sm, as, targetHandle );

            // get the object's direction vector (where it's face is pointing)
            //spatial::math::Vector2 correctDirection( std::cos(md.yaw), std::sin(md.yaw) );
            spatial::math::Vector3 direction( entity->getDirection( ) );

            spatial::Point goalPoint = sm.findFree( spatial::Point( entity->getPosition( ).x, entity->getPosition( ).y ), spatial::Point( direction.x, direction.y ) );

            //spatial::Point startPoint( md.centerX, md.centerY );

            // register seeking object
            _pai.getPetInterface( ).setLatestGotoTarget(
                std::pair<std::string, spatial::Point>( target, spatial::Point( entity->getPosition( ).x, entity->getPosition( ).y ) ) );

            if ( !buildGotoPlan( goalPoint, walkSpeed ) ) {
                if (_hasPlanFailed) {
                    logger().error(
                                 "PAIWorldWrapper - Failed to create a gobehind_obj plan to the goal.");
                } else {
                    logger().info(
                                 "PAIWorldWrapper - No gobehind_obj plan needed since the goal was already near enough.");
                } // if
                MAIN_LOGGER_ACTION_PLAN_FAILED; //macro see top of the file
                return false;
            } // if

        }
        break;

        case id::heel:
            OC_ASSERT(it.number_of_children() == 0);
            //first goto the owner, if we can find it
            if (!WorldWrapperUtil::inSpaceMap(sm, as,
                                              selfName(), ownerName(),
                                              combo::vertex("owner"))) {
                logger().error(
                             "PAIWorldWrapper - Heel: owner not found.");

                _hasPlanFailed = true;
                MAIN_LOGGER_ACTION_PLAN_FAILED; //macro see top of the file
                return false;
            }
            if (!build_goto_plan(WorldWrapperUtil::ownerHandle(as, ownerName())))
                _planID = _pai.createActionPlan();
            //then add a heel action at the end
            _pai.addAction(_planID, PetAction(ActionType::HEEL()));
            break;
        case id::nudge_to:
            OC_ASSERT(it.number_of_children() == 2);
            OC_ASSERT(is_definite_object(*it.begin()));
            OC_ASSERT(is_definite_object(*++it.begin()));
            //make sure we can find the obj to nudge and its destination
            if (!WorldWrapperUtil::inSpaceMap(sm, as, selfName(), ownerName(),
                                              *it.begin()) ||
                    !WorldWrapperUtil::inSpaceMap(sm, as, selfName(), ownerName(),
                                                  *++it.begin())) {
                logger().error(
                             "PAIWorldWrapper - Nudge: obj or destination not found.");

                _hasPlanFailed = true;
                MAIN_LOGGER_ACTION_PLAN_FAILED; //macro see top of the file
                return false;
            }
            //first goto the obj that we want to nudge
            {
                bool useExistingPlan = build_goto_plan(toHandle(get_definite_object(*it.begin())));
                //now build a nudging plan adding to it
                if (!build_goto_plan(toHandle(get_definite_object(*++it.begin())),
                                     useExistingPlan,
                                     toHandle(get_definite_object(*it.begin())))
                        && !useExistingPlan) {
                    MAIN_LOGGER_ACTION_PLAN_FAILED; //macro see top of the file
                    return false;
                }
            }
            break;
        case id::go_behind: {
            OC_ASSERT(it.number_of_children() == 3);
            OC_ASSERT(is_definite_object(*it.begin()));
            OC_ASSERT(is_definite_object(*++it.begin()));
            OC_ASSERT(is_contin(*++(++it.begin())));

            //and just goto it, if we can find the args on the map
            if (!WorldWrapperUtil::inSpaceMap(sm, as, selfName(), ownerName(),
                                              *it.begin()) ||
                    !WorldWrapperUtil::inSpaceMap(sm, as, selfName(), ownerName(),
                                                  *++it.begin())) {
                logger().error(
                             "PAIWorldWrapper - Go behind: args not found.");

                _hasPlanFailed = true;
                MAIN_LOGGER_ACTION_PLAN_FAILED; //macro see top of the file
                return false;
            }
            float walkSpeed = get_contin( *++(++it.begin()) );

            if (!build_goto_plan(toHandle(get_definite_object(*it.begin())),
                                 false, Handle::UNDEFINED, toHandle(get_definite_object(*++it.begin()))), walkSpeed ) {
                MAIN_LOGGER_ACTION_PLAN_FAILED; //macro see top of the file
                return false;
            }
        }
        break;
        case id::follow: {
            OC_ASSERT(it.number_of_children() == 3);
            OC_ASSERT(is_definite_object(*it.begin()));
            OC_ASSERT(is_contin(*++it.begin()));
            OC_ASSERT(is_contin(*++(++it.begin())));
            {
                Handle obj = toHandle(get_definite_object(*it.begin()));
                OC_ASSERT(obj != Handle::UNDEFINED);
                //first goto the obj to follow, if we can find it
                if (!WorldWrapperUtil::inSpaceMap(sm, as, selfName(), ownerName(),
                                                  *it.begin())) {
                    logger().error(
                                 "PAIWorldWrapper - Follow: obj not found.");

                    _hasPlanFailed = true;
                    MAIN_LOGGER_ACTION_PLAN_FAILED; //macro see top of the file
                    return false;
                }
                float walkSpeed = get_contin( *++it.begin() );
                float duration = get_contin( *++(++it.begin( ) ) );

                if ( duration == 0 ) {
                    duration = _pai.getPetInterface( ).computeFollowingDuration( );
                } // if

                if (!build_goto_plan(obj, false, Handle::UNDEFINED, Handle::UNDEFINED, walkSpeed ) )
                    _planID = _pai.createActionPlan();
                //then add a follow action at the end
                PetAction action(ActionType::FOLLOW());
                action.addParameter(ActionParameter("id",
                                                    ActionParamType::ENTITY(),
                                                    Entity(get_definite_object(*it.begin()),
                                                           resolveType(*it.begin()))));
                action.addParameter(ActionParameter("duration", ActionParamType::FLOAT(), lexical_cast<string>(duration)) );
                _pai.addAction(_planID, action);
            }
        }
        break;
        default:
            std::stringstream stream (std::stringstream::out);
            stream << "Unrecognized compound schema '"  << combo_tree(it) << "'" << std::endl;
            throw opencog::ComboException(TRACE_INFO, "PAIWorldWrapper - %s.", stream.str().c_str());
        }
        //try { //TODO
        if (_pai.isActionPlanEmpty(_planID)) {
            _hasPlanFailed = true;
            MAIN_LOGGER_ACTION_PLAN_FAILED; //macro see top of the file
            return false;
        } else {
            _pai.sendActionPlan(_planID);
            MAIN_LOGGER_ACTION_PLAN_SUCCEEDED; //macro see top of the file
            return true;
        }
        //} catch (...) {
        //throw ActionPlanSendingFailure(_planID);
        //}

    } else { //non-compound action sequence
        while (from != to) {
            try {
                PetAction action = buildPetAction(from);
                _pai.addAction(_planID, action);
                ++from;
            } catch( const opencog::StandardException& ex ) {
                std::stringstream ss (stringstream::out);
                ss << combo_tree(from);
                ss << " " << const_cast<opencog::StandardException*>( &ex )->getMessage( );
                logger().error( "PAIWorldWrapper::%s - Failed to build PetAction '%s'",
                                __FUNCTION__, ss.str().c_str( )  );
                from = to;
            } catch (...) {
                //log error
                std::stringstream ss (stringstream::out);
                ss << combo_tree(from);
                logger().error(
                             "PAIWorldWrapper - Failed to build PetAction '%s'.",
                             ss.str().c_str());
                //~log error

                from = to; //so no more actions are built for that action plan, as according to and_seq semantics
            }
        }
        //TODO : add expection
        //try {
        if (_pai.isActionPlanEmpty(_planID)) {
            _hasPlanFailed = true;
            MAIN_LOGGER_ACTION_PLAN_FAILED; //macro see top of the file
            return false;
        } else {
            _pai.sendActionPlan(_planID);
            MAIN_LOGGER_ACTION_PLAN_SUCCEEDED; //macro see top of the file
            return true;
        }
        //} catch (...) {
        //throw ActionPlanSendingFailure(_planID);
        //}
    }
}

combo::vertex PAIWorldWrapper::evalPerception(pre_it it, combo::variable_unifier& vu)
{
    Handle smh = _pai.getAtomSpace().getSpaceServer().getLatestMapHandle();
    unsigned int current_time = _pai.getLatestSimWorldTimestamp();
    OC_ASSERT(smh != Handle::UNDEFINED,
                     "A SpaceMap must exists");
    combo::vertex v = WorldWrapperUtil::evalPerception(rng, smh, current_time,
                      _pai.getAtomSpace(),
                      selfName(), ownerName(),
                      it, false, vu);

    //DEBUG log
    if (logger().isDebugEnabled()) {
        stringstream p_ss, r_ss;
        p_ss << combo_tree(it);
        r_ss << v;
        logger().debug(
                     "PAIWorldWrapper - Perception %s has been evaluation to %s",
                     p_ss.str().c_str(), r_ss.str().c_str());
    }
    //~DEBUG log

    return v;
}

combo::vertex PAIWorldWrapper::evalIndefiniteObject(indefinite_object io,
        combo::variable_unifier& vu)
{
    Handle smh = _pai.getAtomSpace().getSpaceServer().getLatestMapHandle();
    unsigned int current_time = _pai.getLatestSimWorldTimestamp();
    OC_ASSERT(smh != Handle::UNDEFINED,
                     "A SpaceMap must exists");

    combo::vertex v = WorldWrapperUtil::evalIndefiniteObject(rng, smh, current_time,
                      _pai.getAtomSpace(),
                      selfName(), ownerName(),
                      io, false, vu);

    //DEBUG log
    if (logger().isDebugEnabled()) {
        stringstream io_ss, r_ss;
        io_ss << io;
        r_ss << v;
        logger().debug(
                     "PAIWorldWrapper - Indefinition object %s has been evaluation to %s",
                     io_ss.str().c_str(), r_ss.str().c_str());
    }
    //~DEBUG log

    return v;
}


/**
 * private methods
 */
void PAIWorldWrapper::clearPlan( std::vector<spatial::Point>& actions,
                                 const spatial::Point& startPoint,
                                 const spatial::Point& endPoint )
{
    const SpaceServer::SpaceMap& sm = _pai.getAtomSpace().getSpaceServer().getLatestMap();

    double closestDist = SpaceServer::SpaceMap::eucDist( startPoint,
                         endPoint );
    std::vector<spatial::Point>::iterator eraseFrom = actions.begin( );
    std::vector<spatial::Point>::iterator it;

    for ( it = actions.begin(); it != actions.end(); ++it ) {
        double d = SpaceServer::SpaceMap::eucDist(*it, endPoint);
        if (d < closestDist) {
            eraseFrom = next(it);

            // at some point the following "take the nearest point"
            // heuristic can be
            // made more subtle by adding a weight to the length of the path -
            // e.g. if the path generated by tb is (x1,x2,...,xN) and N is large,
            // and the point xN is only slightly closer to the goal than x2, then
            // maybe we want to just go to x2 and not all the way to xN
            if (d <= sm.radius() * 1.1) {
                break;
            } // if
            closestDist = d;
        } // if
    } // for
    actions.erase( eraseFrom, actions.end( ) );
}

spatial::Point PAIWorldWrapper::getValidPosition( const spatial::Point& location )
{
    const SpaceServer::SpaceMap& spaceMap = _pai.getAtomSpace().getSpaceServer().getLatestMap();

    //make sure that the object location is valid
    spatial::Point correctedLocation = location;
    if ( spaceMap.illegal( location ) ) {
        logger().warn(
                     "PAIWorldWrapper - Position (%.2f, %.2f) is invalid (off the grid, near/inside an obstacle).",
                     location.first, location.second);

        correctedLocation = spaceMap.getNearestFreePoint( location );

        logger().warn(
                     "PAIWorldWrapper - Changed position to the nearest valid point (%.2f, %.2f).",
                     correctedLocation.first, correctedLocation.second);
    } // if

    return correctedLocation;
}

void PAIWorldWrapper::getWaypoints( const spatial::Point& startPoint, const spatial::Point& endPoint, std::vector<spatial::Point>& actions )
{
    const std::string pathFindingAlgorithm =
        opencog::config().get("NAVIGATION_ALGORITHM");

    const SpaceServer::SpaceMap& sm = _pai.getAtomSpace().getSpaceServer().getLatestMap();

    try {
        spatial::Point begin = startPoint;
        spatial::Point end = endPoint;

        spatial::Point correctedAgentLocation = getValidPosition( begin );
        spatial::Point correctedEndLocation = getValidPosition( end );

        if ( correctedAgentLocation != begin ) {
            begin = correctedAgentLocation;
            actions.push_back( begin );
        } // if

        if ( correctedEndLocation != end ) {
            end = correctedEndLocation;
        } // if

        if ( pathFindingAlgorithm == "astar") {
            spatial::LSMap2DSearchNode petNode = sm.snap(spatial::Point(begin.first, begin.second));
            spatial::LSMap2DSearchNode goalNode = sm.snap(spatial::Point(end.first, end.second));

            spatial::AStarController AStar;
            SpaceServer::SpaceMap *map = const_cast<SpaceServer::SpaceMap*>(&sm);
            AStar.setMap( map );
            AStar.setStartAndGoalStates(petNode, goalNode);

            //finally, run AStar
            _hasPlanFailed = (AStar.findPath() != spatial::AStarSearch<spatial::LSMap2DSearchNode>::SEARCH_STATE_SUCCEEDED);
            actions = AStar.getShortestCalculatedPath( );

            logger().debug(
                         "PAIWorldWrapper - AStar result %s.", !_hasPlanFailed ? "true" : "false");
        } else if ( pathFindingAlgorithm == "hpa" ) {

            SpaceServer::SpaceMap *map = const_cast<SpaceServer::SpaceMap*>(&sm);
            unsigned int maximumClusters = opencog::config().get_int("HPA_MAXIMUM_CLUSTERS");
            spatial::HPASearch search( map, 1, maximumClusters );

            _hasPlanFailed = !search.processPath( spatial::math::Vector2( begin.first, begin.second ), spatial::math::Vector2( end.first, end.second ) );
            std::vector<spatial::math::Vector2> pathPoints = search.getProcessedPath( 1 );
            if ( !_hasPlanFailed ) {
                foreach( spatial::math::Vector2 pathPoint, pathPoints ) {
                    actions.push_back( spatial::Point( pathPoint.x, pathPoint.y ) );
                } // foreach
            } // if

            logger().debug(
                         "PAIWorldWrapper - HPASearch result %s.", !_hasPlanFailed ? "true" : "false");

        } else {
            spatial::TangentBug::CalculatedPath calculatedPath;
            spatial::TangentBug tb(sm, calculatedPath, rng);

            //place the pet and the goal on the map
            tb.place_pet(begin.first, begin.second);
            tb.place_goal(end.first, end.second);

            //finally, run tangent bug
            _hasPlanFailed = !tb.seek_goal();
            if ( !_hasPlanFailed ) {
                spatial::TangentBug::CalculatedPath::iterator it;
                for ( it = calculatedPath.begin( ); it != calculatedPath.end( ); ++it ) {
                    actions.push_back( boost::get<0>( *it ) );
                } // for
            } // if
            logger().debug("PAIWorldWrapper - TangetBug result %s.",
                         !_hasPlanFailed ? "true" : "false");
        } // else
    } catch ( opencog::RuntimeException& e ) {
        _hasPlanFailed = true;
    } catch ( opencog::AssertionException& e) {
        _hasPlanFailed = true;
    } // catch
}

bool PAIWorldWrapper::buildGotoPlan( const spatial::Point& position, float customSpeed )
{

    const AtomSpace& as = _pai.getAtomSpace();
    const SpaceServer::SpaceMap& sm = as.getSpaceServer().getLatestMap();
    std::vector<spatial::Point> actions;

    spatial::Point startPoint = WorldWrapperUtil::getLocation(sm, as,
                                WorldWrapperUtil::selfHandle( as, selfName( ) ) );
    spatial::Point endPoint = position;

    getWaypoints( startPoint, endPoint, actions );

    if ( !_hasPlanFailed ) {
        clearPlan( actions, startPoint, endPoint );
        return createWalkPlanAction( actions, false, Handle::UNDEFINED, customSpeed );
    } // if

    // plan has failed
    return false;
}

bool PAIWorldWrapper::createWalkPlanAction( std::vector<spatial::Point>& actions, bool useExistingId, Handle toNudge, float customSpeed )
{

    if ( actions.empty( ) ) {
        // we're done. No need to create any walk sequency
        logger().debug("PAIWorldWrapper - Zero actions from AStar.");
        return false;
    }

    // --------------------------------------------------------------------
    // transform to a sequence of walk commands
    // --------------------------------------------------------------------

    if (!useExistingId ) {
        _planID = _pai.createActionPlan( );
    } // if

    foreach(const spatial::Point& it_action, actions ) {
        PetAction action;

        if (toNudge != Handle::UNDEFINED) {
            action = PetAction(ActionType::NUDGE_TO());
            action.addParameter(ActionParameter("moveableObj",
                                                ActionParamType::ENTITY(),
                                                Entity(_pai.getAtomSpace().getName(toNudge),
                                                       resolveType(toNudge))));
            action.addParameter(ActionParameter("target",
                                                ActionParamType::VECTOR(),
                                                Vector(it_action.first,
                                                       it_action.second,
                                                       0.0)));
        } else {
            action = PetAction(ActionType::WALK());
            action.addParameter(ActionParameter("target",
                                                ActionParamType::VECTOR(),
                                                Vector(it_action.first,
                                                       it_action.second,
                                                       0.0)));

            float speed = ( customSpeed != 0 ) ? customSpeed : _pai.getPetInterface().computeWalkingSpeed();
            logger().debug("PAIWorldWrapper::createWalkPlanAction customSpeed[%f] finalSpeed[%f]", customSpeed, speed );
            action.addParameter(ActionParameter("speed", ActionParamType::FLOAT(), lexical_cast<string>( speed) ) );

        } // else
        _pai.addAction( _planID, action );
    } // foreach

    return true;
}

bool PAIWorldWrapper::build_goto_plan(Handle goalHandle,
                                      bool useExistingID,
                                      Handle toNudge,
                                      Handle goBehind, float walkSpeed )
{

    const AtomSpace& atomSpace = _pai.getAtomSpace();
    const SpaceServer::SpaceMap& spaceMap = atomSpace.getSpaceServer().getLatestMap();
    std::string goalName = atomSpace.getName(goalHandle);

    OC_ASSERT(goalHandle != Handle::UNDEFINED);
    OC_ASSERT(spaceMap.containsObject(goalName));

    spatial::Point startPoint;
    spatial::Point endPoint;
    spatial::Point targetCenterPosition;

    try {
        startPoint = WorldWrapperUtil::getLocation(spaceMap, atomSpace,
                     WorldWrapperUtil::selfHandle(atomSpace, selfName()));
        targetCenterPosition = WorldWrapperUtil::getLocation(spaceMap, atomSpace, goalHandle );
        if ( goBehind != Handle::UNDEFINED ) {
            endPoint = spaceMap.behindPoint(WorldWrapperUtil::getLocation(spaceMap, atomSpace, goBehind), goalName);
        } else {
            endPoint = spaceMap.nearbyPoint(startPoint, goalName);
            if (spaceMap.gridIllegal(spaceMap.snap(endPoint))) {
                logger().error(
                             "PAIWorldWrapper - nearby point selected and invalid point.");
            }
        } // else
    } catch ( opencog::AssertionException& e ) {
        logger().error(
                     "PAIWorldWrapper - Unable to get pet or goal location.");
        _hasPlanFailed = true;
        return false;
    } // catch

    logger().fine(
                 "PAIWorldWrapper - Pet position: (%.2f, %.2f). Goal position: (%.2f, %.2f) - %s.",
                 startPoint.first, startPoint.second,  endPoint.first, endPoint.second, goalName.c_str());
    // register seeking object
    _pai.getPetInterface( ).setLatestGotoTarget(
        std::pair<std::string, spatial::Point>( goalName, targetCenterPosition ) );

    return buildGotoPlan( endPoint, walkSpeed );
}

PetAction PAIWorldWrapper::buildPetAction(sib_it from)
{
    unsigned int current_time = _pai.getLatestSimWorldTimestamp();
    AtomSpace& as = _pai.getAtomSpace();
    const SpaceServer::SpaceMap& sm = as.getSpaceServer().getLatestMap();
    static const std::map<pet_builtin_action_enum, ActionType> actions2types =
        boost::assign::map_list_of
        (id::bark, ActionType::BARK())
        (id::bare_teeth_at, ActionType::BARE_TEETH())
        (id::bark_at, ActionType::BARK())
        (id::chew, ActionType::CHEW())
        (id::dream, ActionType::DREAM())
        (id::drink, ActionType::DRINK())
        (id::eat, ActionType::EAT())
        (id::grab, ActionType::GRAB())
        (id::growl_at, ActionType::GROWL())
        (id::jump_up, ActionType::JUMP_UP())
        (id::jump_towards, ActionType::JUMP_TOWARD())
        //(id::move_left_ear, ActionType::LEFT_EAR_?()) each ear movement has its own command
        //(id::move_right_ear, ActionType::RIGHT_EAR_?())
        (id::lick_at, ActionType::LICK())
        (id::move_head, ActionType::MOVE_HEAD())
        (id::random_step, ActionType::WALK())
        (id::rotate_left, ActionType::TURN())
        (id::rotate_right, ActionType::TURN())
        // (id::scratch_self, ActionType::SCRATCH_SELF_*()) // each body part has itw own  scratch command
        (id::scratch_other, ActionType::SCRATCH_OTHER())
        (id::scratch_ground_back_legs, ActionType::SCRATCH_GROUND_BACK_LEGS())
        (id::sniff_at, ActionType::SNIFF_AT())
        (id::sniff_avatar_part, ActionType::SNIFF_AVATAR_PART())
        (id::sniff_pet_part, ActionType::SNIFF_PET_PART())
        (id::step_backward, ActionType::WALK())
        (id::step_forward, ActionType::WALK())
        (id::step_towards, ActionType::WALK())
        (id::tail_flex, ActionType::TAIL_FLEX())
        (id::turn_to_face, ActionType::TURN())
        (id::wag, ActionType::WAG())
        (id::bite, ActionType::BITE())
        (id::pet, ActionType::PET())
        (id::kick, ActionType::KICK())
        (id::group_command, ActionType::GROUP_COMMAND())
        (id::receive_latest_group_commands, ActionType::RECEIVE_LATEST_GROUP_COMMANDS())
        (id::sit, ActionType::SIT())
        (id::look_at, ActionType::LOOK_AT())
        (id::say, ActionType::SAY())
        (id::whine_at, ActionType::WHINE());

    OC_ASSERT(WorldWrapperUtil::is_builtin_atomic_action(*from));
    builtin_action ba = get_builtin_action(*from);
    pet_builtin_action_enum bae = get_enum(ba);

    stringstream ss;
    ss << *from;

    logger().debug("PAIWorldWrapper::%s - Trying to build pet action '%s' for builtin_action_enum %d'", 
                   __FUNCTION__, ss.str().c_str( ), bae);

    /****
         this switch statement deals with
         special cases - e.g. step_forward needs to get translated into a walk
         command based on the pet's current position

         also, commands like scratch needs to have the body-part codes translated

         the full list of such schema is:

         bare_teeth_at(obj)
         bark_at(obj)
         chew(obj)
         dream(obj)
         drink(drinkable_obj)
         eat(edible_obj)
         grab(pickupable_obj)
         growl_at(obj)
         jump_towards(obj)
         move_left_ear(TWITCH|PERK|BACK)
         lick_at(obj)
         move_head(angle, angle)
         random_step
         move_right_ear(TWITCH|PERK|BACK)
         rotate_left
         rotate_right
         scratch_other(obj)
         scratch_self(NOSE|RIGHT_EAR|LEFT_EAR|NECK|RIGHT_SHOULDER|LEFT_SHOULDER)
         sniff_at(obj)
         sniff_avatar_part(avatar,RIGHT_FOOT|LEFT_FOOT|RIGHT_HAND|LEFT_HAND|CROTCH|BUTT)
         sniff_pet_part(pet,NOSE|NECK|BUTT)
         step_backward
         step_forward
         step_towards(obj,TOWARDS|AWAY)
         tail_flex(position)
         turn_to_face(obj)
         bite(obj)
         whine_at(obj)
    ****/
    PetAction action;
    if (actions2types.find(bae) != actions2types.end()) {
        action = actions2types.find(bae)->second;
    } else {
        logger().fine(
                     "PAIWorldWrapper - No action type was found to build pet action at actions2types" );
    } // else

    double theta = 0;
    switch (bae) {
        //we're going to do all of the "do x to id y" commands together
    case id::bare_teeth_at:     // bare_teeth_at(obj)
    case id::bark_at:           // bark_at(obj)
    case id::chew:              // chew(obj)
    case id::dream:             // dream(obj)
    case id::drink:             // drink(drinkable_obj)
    case id::eat:               // eat(edible_obj)
    case id::grab:              // grab(pickupable_obj)
    case id::growl_at:          // growl_at(obj)
    case id::lick_at:           // lick_at(obj)
    case id::scratch_other:     // scratch_other(obj)
    case id::sniff_at:          // sniff_at(obj)
    case id::whine_at:          // whine_at(obj)
    case id::bite:              // bite(obj)
    case id::pet:               // pet(obj)
    case id::kick:              // kick(obj)
        action.addParameter(ActionParameter("target",
                                            ActionParamType::ENTITY(),
                                            Entity(get_definite_object(*from.begin()),
                                                   resolveType(*from.begin()))));
        break;

    case id::look_at: {
        if ( from.number_of_children( ) != 1 ) {
            throw opencog::InvalidParamException(TRACE_INFO,
                                                 "PAIWorldWrapper - Invalid number of arguments for look_at %d", from.number_of_children( )  );
        } // if

        sib_it arguments = from.begin( );

        // get target name. can be an agent name
        std::string targetName;
        if ( is_indefinite_object( *arguments ) ) {
            combo::variable_unifier vu;
            targetName = get_definite_object( evalIndefiniteObject( get_indefinite_object( *arguments ), vu ) );
        } else {
            targetName = get_definite_object( *arguments );
        } // else

        if ( targetName == "custom_object" ) {
            targetName = _pai.getPetInterface( ).getCurrentModeHandler( ).getPropertyValue( "customObject" );
        } // else

        action.addParameter( ActionParameter( "target", ActionParamType::ENTITY( ),
                                              Entity( targetName, resolveType(targetName) ) ) );
    }
    break;

    case id::say: {
        if ( from.number_of_children( ) != 2 ) {
            throw opencog::InvalidParamException(TRACE_INFO,
                                                 "PAIWorldWrapper - Invalid number of arguments for say %d", from.number_of_children( )  );
        } // if
        sib_it arguments = from.begin( );
        
        std::string message = "";
        if ( !is_definite_object(*arguments) ) {
            logger().error( "PAIWorldWrapper::%s - The first 'say' argument must be a definite_object which defines the string message" );            
        } else {
            message = get_definite_object(*arguments);
        } // else

        if ( message == "custom_message" ) {
            message = _pai.getPetInterface( ).getCurrentModeHandler( ).getPropertyValue( "customMessage" );
        } // if

        // once the say action was executed set the has_something_to_say predicate to false
        // to avoid repetitions
        AtomSpaceUtil::setPredicateValue( as, "has_something_to_say", TruthValue::FALSE_TV( ),
                                          AtomSpaceUtil::getAgentHandle( as, _pai.getPetInterface( ).getPetId( ) ) );
                        
        action.addParameter( ActionParameter( "message", ActionParamType::STRING( ), message ) );

        ++arguments;

        std::string targetName;
        if ( is_indefinite_object( *arguments ) ) {
            combo::variable_unifier vu;
            targetName = get_definite_object( evalIndefiniteObject( get_indefinite_object( *arguments ), vu ) );
        } else {
            targetName = get_definite_object( *arguments );
        } // else
        
        if ( targetName == "custom_object" ) {
            targetName = _pai.getPetInterface( ).getCurrentModeHandler( ).getPropertyValue( "customObject" );
        } // else
        
        action.addParameter( ActionParameter( "target", ActionParamType::STRING( ), targetName ) );
    }
    break;

    case id::group_command: {    // group_command(string, string [, arg_list] )
        logger().debug(
                     "PAIWorldWrapper - Building PetAction group_command. number_of_children: %d",
                     from.number_of_children( ) );

        if ( from.number_of_children( ) < 2 ) {
            throw opencog::InvalidParamException(TRACE_INFO,
                                                 "PAIWorldWrapper - Invalid number of arguments for group_command" );
        } // if

        sib_it arguments = from.begin( );
        combo::variable_unifier vu;
        // get target name. can be an agent name or all_agents
        std::string targetName;

        if ( is_indefinite_object( *arguments ) ) {
            targetName = get_definite_object( evalIndefiniteObject( get_indefinite_object( *arguments ), vu ) );
        } else {
            targetName = get_definite_object( *arguments );
        } // else

        ++arguments;

        std::string commandName = get_definite_object( *arguments );

        std::vector<std::string> actionDoneParameters;
        actionDoneParameters.push_back( commandName );

        //std::vector<std::string> parameters;
        std::stringstream parameters;
        for (++arguments; arguments != from.end(); arguments++) {
            // retrieve a string containing a given parameter
            std::string parameter;

            if ( is_indefinite_object( *arguments ) ) {
                parameter = get_definite_object( evalIndefiniteObject( get_indefinite_object( *arguments ), vu ) );
            } else {
                parameter = get_definite_object( *arguments );
            } // else

            if ( parameter == "randbool" ) {
                parameter = ((rng.randint() % 2) == 0) ? "0" : "1";
            } else if ( parameter == "rand" ) {
                std::stringstream rand;
                rand << rng.randdouble( );
                parameter = rand.str( );
            } // if


            //parameters.push_back( parameter );
            parameters << parameter;
            sib_it nextArgument = arguments;
            ++nextArgument;
            actionDoneParameters.push_back( parameter );
            if ( nextArgument != from.end( ) ) {
                parameters << ";";
            } // if
        } // for


        action.addParameter( ActionParameter( "target", ActionParamType::STRING( ), targetName ) );
        action.addParameter( ActionParameter( "command", ActionParamType::STRING( ), commandName ) );
        action.addParameter( ActionParameter( "parameters", ActionParamType::STRING( ), parameters.str() ) );

        _pai.getPetInterface( ).getCurrentModeHandler( ).handleCommand( "groupCommandDone", actionDoneParameters );

    }
    break;

    case id::receive_latest_group_commands: {
        OC_ASSERT(from.number_of_children() == 0);

        logger().debug(
                     "PAIWorldWrapper - receiving latest_group_commands from all agents" );

        //_pai.getPetInterface( ).getCurrentModeHandler( ).handleCommand( "receivedGroupCommand", commandArguments );

        // retrieve all agents
        std::vector<Handle> agentsHandles;
        as.getHandleSet( back_inserter(agentsHandles), AVATAR_NODE, false );
        as.getHandleSet( back_inserter(agentsHandles), HUMANOID_NODE, false );
        as.getHandleSet( back_inserter(agentsHandles), PET_NODE, false );


        Handle selfHandle = WorldWrapperUtil::selfHandle(  as, selfName( ) );
        unsigned int i;
        for ( i = 0; i < agentsHandles.size( ); ++i ) {
            if ( agentsHandles[i] == Handle::UNDEFINED || agentsHandles[i] == selfHandle ) {
                // ignore non agents entities
                // ignore self
                continue;
            } // if
            std::string agentId = as.getName( agentsHandles[i] );
            logger().debug(
                         "PAIWorldWrapper - verifying agent[%s]", agentId.c_str( ) );

            unsigned long delay_past = 10 * PerceptionActionInterface::PAIUtils::getTimeFactor();
            unsigned long t_past = ( delay_past < current_time ? current_time - delay_past : 0 );

            Handle agentActionLink = AtomSpaceUtil::getMostRecentAgentActionLink( as, agentId, "group_command", Temporal( t_past, current_time ), TemporalTable::OVERLAPS );

            if ( agentActionLink == Handle::UNDEFINED ) {
                logger().debug("PAIWorldWrapper - agent[%s] did not sent group command during the last 30 seconds", agentId.c_str( ) );
                continue;
            } // if


            std::string parametersString =
                AtomSpaceUtil::convertAgentActionParametersToString( as, agentActionLink );

            std::vector<std::string> commandParameters;
            boost::split( commandParameters, parametersString, boost::is_any_of( ";" ) );
            // trim all elements
            unsigned int k;
            for ( k = 0; k < commandParameters.size( ); ++k ) {
                boost::trim( commandParameters[k] );
            }

            logger().debug("PAIWorldWrapper - number of parameters[%d], string[%s]", commandParameters.size( ), parametersString.c_str( ) );

            if ( commandParameters.size( ) > 0 && ( commandParameters[0] == "all_agents" ||
                                                    commandParameters[0] == selfName( ) ) ) {
                std::vector<std::string> commandArguments;
                commandArguments.push_back( agentId );

                unsigned int j;
                for ( j = 1; j < commandParameters.size( ); ++j ) {
                    commandArguments.push_back( commandParameters[j] );
                } // for

                _pai.getPetInterface( ).getCurrentModeHandler( ).handleCommand( "receivedGroupCommand", commandArguments );

            } // if
        } // for

    }
    break;

    //we'll do all of the cases where there's bodyPart arg converted to the right integer constants here

    //these have the target and specific parts
    case id::sniff_avatar_part: { // sniff_avatar_part(avatar,RIGHT_FOOT|LEFT_FOOT|RIGHT_HAND|LEFT_HAND|CROTCH|BUTT)
        OC_ASSERT(from.number_of_children() == 2);
        action.addParameter(ActionParameter("avatar",
                                            ActionParamType::ENTITY(),
                                            Entity(get_definite_object(*from.begin()),
                                                   resolveType(*from.begin()))));
        combo::vertex v = *from.last_child();
        OC_ASSERT(is_action_symbol(v),
                         "It is assumed v is an action_symbol");
        combo::pet_action_symbol_enum ase = get_enum(get_action_symbol(v));
        string part;
        // From Pet Action spec: RIGHT_FOOT = 0, LEFT_FOOT = 1, BUTT = 2, RIGHT_HAND = 2, LEFT_HAND = 3, CROTCH = 4
        switch (ase) {
        case id::RIGHT_FOOT:
            part = "0";
            break;
        case id::LEFT_FOOT:
            part = "1";
            break;
        case id::BUTT:
            part = "2";
            break;
        case id::RIGHT_HAND:
            part = "3";
            break;
        case id::LEFT_HAND:
            part = "4";
            break;
        case id::CROTCH:
            part = "5";
            break;
        default:
            logger().error("PAIWorldWrapper - Invalid avatar part as parameter for sniff_avatar_part: %s.", instance(ase)->get_name().c_str());
            OC_ASSERT(false);
        }
        action.addParameter(ActionParameter("part", ActionParamType::INT(), part));
    }
    break;
    case id::sniff_pet_part: {  // sniff_pet_part(pet,NOSE|NECK|BUTT)
        OC_ASSERT(from.number_of_children() == 2);
        action.addParameter(ActionParameter("pet",
                                            ActionParamType::ENTITY(),
                                            Entity(get_definite_object(*from.begin()),
                                                   resolveType(*from.begin()))));
        combo::vertex v = *from.last_child();
        OC_ASSERT(is_action_symbol(v),
                         "It is assumed v is an action_symbol");
        pet_action_symbol_enum ase = get_enum(get_action_symbol(v));
        string part;
        switch (ase) {
        case id::NOSE:
            part = "0";
            break;
        case id::NECK:
            part = "1";
            break;
        case id::BUTT:
            part = "2";
            break;
        default:
            logger().error("PAIWorldWrapper - Invalid pet part as parameter for sniff_pet_part: %s.", instance(ase)->get_name().c_str());
            OC_ASSERT(false);
        }
        action.addParameter(ActionParameter("part", ActionParamType::INT(), part));
    }
    break;
    //these have specific actions for each part
    case id::scratch_self: {    // scratch_self(NOSE|RIGHT_EAR|LEFT_EAR|NECK|RIGHT_SHOULDER|LEFT_SHOULDER)
        combo::vertex v = *from.begin();
        OC_ASSERT(is_action_symbol(v),
                         "It is assumed v is an action_symbol");
        combo::pet_action_symbol_enum ase = get_enum(get_action_symbol(v));
        switch (ase) {
        case id::NOSE:
            action = ActionType::SCRATCH_SELF_NOSE();
            break;
        case id::RIGHT_EAR:
            action = ActionType::SCRATCH_SELF_RIGHT_EAR();
            break;
        case id::LEFT_EAR:
            action = ActionType::SCRATCH_SELF_LEFT_EAR();
            break;
        case id::NECK:
            action = ActionType::SCRATCH_SELF_NECK();
            break;
        case id::RIGHT_SHOULDER:
            action = ActionType::SCRATCH_SELF_RIGHT_SHOULDER();
            break;
        case id::LEFT_SHOULDER:
            action = ActionType::SCRATCH_SELF_LEFT_SHOULDER();
            break;
        default:
            logger().error("PAIWorldWrapper - Invalid pet part as parameter for scratch_self: %s.", instance(ase)->get_name().c_str());
            OC_ASSERT(false);
        }
    }
    break;

    case id::move_left_ear:  // move_left_ear(TWITCH|PERK|BACK)
    case id::move_right_ear: // move_right_ear(TWITCH|PERK|BACK)
        if (*from.begin() == instance(id::TWITCH)) {
            action = (bae == instance(id::move_left_ear)) ? ActionType::LEFT_EAR_TWITCH() : ActionType::RIGHT_EAR_TWITCH();
        } else if (*from.begin() == instance(id::PERK)) {
            action = (bae == instance(id::move_left_ear)) ? ActionType::LEFT_EAR_PERK() : ActionType::RIGHT_EAR_PERK();
        } else {
            action = (bae == instance(id::move_left_ear)) ? ActionType::LEFT_EAR_BACK() : ActionType::RIGHT_EAR_BACK();
        }
        break;

        //now rotations
    case id::rotate_left:      // rotate_left
        action.addParameter(ActionParameter("rotation",
                                            ActionParamType::ROTATION(),
                                            Rotation(0.0, 0.0, _pai.getPetInterface().computeRotationAngle())));
        break;

    case id::rotate_right:     // rotate_right
        action.addParameter(ActionParameter("rotation",
                                            ActionParamType::ROTATION(),
                                            Rotation(0.0, 0.0, -_pai.getPetInterface().computeRotationAngle())));
        break;

        //now stepping actions
    case id::random_step:        // random_step
        theta = 2.0 * PI * rng.randdouble();
        goto build_step;
    case id::step_backward :     // step_backward
        theta = getAngleFacing(WorldWrapperUtil::selfHandle(as,
                               selfName())) + PI;
        if (theta > 2*PI)
            theta -= 2 * PI;
        goto build_step;
    case id::step_forward:      // step_forward
        theta = getAngleFacing(WorldWrapperUtil::selfHandle(as,
                               selfName()));

    build_step:
        //now compute a step in which direction the pet is going
        {
            spatial::Point petLoc = WorldWrapperUtil::getLocation(sm, as, WorldWrapperUtil::selfHandle(as, selfName()));

            double stepSize = (sm.diagonalSize()) * STEP_SIZE_PERCENTAGE / 100; // 2% of the width
            double x = (double)petLoc.first + (cos(theta) * stepSize);
            double y = (double)petLoc.second + (sin(theta) * stepSize);

            // if not moving to an illegal position, then no problem, go
            // to computer position
            if (!sm.illegal(spatial::Point(x, y))) {
                action.addParameter(ActionParameter("target",
                                                    ActionParamType::VECTOR(),
                                                    Vector(x, y, 0.0)));

                // if illegal position, stay in the same place (a walk to
                // the current position
            } else {
                action.addParameter(ActionParameter("target",
                                                    ActionParamType::VECTOR(),
                                                    Vector((double)petLoc.first,
                                                           (double)petLoc.second,
                                                           0.0)));
            }
            action.addParameter(ActionParameter("speed",
                                                ActionParamType::FLOAT(),
                                                lexical_cast<string>(_pai.getPetInterface().computeWalkingSpeed())));
        }
        break;
    case id::step_towards:      // step_towards(obj,TOWARDS|AWAY)
        OC_ASSERT(from.number_of_children() == 2);
        OC_ASSERT(
                         *from.last_child() == instance(id::TOWARDS) ||
                         *from.last_child() == instance(id::AWAY));
        {
            double stepSize = (sm.diagonalSize()) * STEP_SIZE_PERCENTAGE / 100; // 2% of the width

            // object position
            SpaceServer::SpaceMapPoint p = WorldWrapperUtil::getLocation(sm, as, toHandle(get_definite_object(*from.begin())));

            double len = pow((p.first * p.first) + (p.second * p.second), 0.5);
            p.first *= min(stepSize / len, 1.0);
            p.second *= min(stepSize / len, 1.0);

            if (*from.last_child() == instance(id::AWAY)) {
                p.first = -p.first;
                p.second = -p.second;
            }
            action.addParameter(ActionParameter("target",
                                                ActionParamType::VECTOR(),
                                                Vector(p.first, p.second, 0.0)));
            action.addParameter(ActionParameter("speed",
                                                ActionParamType::FLOAT(),
                                                lexical_cast<string>(_pai.getPetInterface().computeWalkingSpeed())));
        }
        break;

        //now a bazillion special cases
    case id::jump_towards:      // jump_towards(obj)
        //we need to compute the obj's position and pass it as and arg to
        //tristan's jumpToward(vector) function
    {
        SpaceServer::SpaceMapPoint p = WorldWrapperUtil::getLocation(sm, as, toHandle(get_definite_object(*from.begin())));
        action.addParameter(ActionParameter("position",
                                            ActionParamType::VECTOR(),
                                            Vector(p.first, p.second, 0.0)));
    }
    break;
    case id::move_head:         // move_head(angle, angle)
        OC_ASSERT(from.number_of_children() == 3);
        action.addParameter(ActionParameter("position",
                                            ActionParamType::VECTOR(),
                                            Vector(0.0, 0.0, 0.0)));
        action.addParameter(ActionParameter("rotation",
                                            ActionParamType::ROTATION(),
                                            Rotation(get_contin(*from.begin()),
                                                     get_contin(*++from.begin()),
                                                     get_contin(*from.last_child()))));
        action.addParameter(ActionParameter("speed",
                                            ActionParamType::FLOAT(),
                                            "1.0"));
        break;

    case id::tail_flex:         // tail_flex(position)
        action.addParameter(ActionParameter("position",
                                            ActionParamType::VECTOR(),
                                            Vector(0.0, 0.0, 0.0)));
        //action.addParameter(ActionParameter("rotation",
        //ActionParamType::ROTATION(),
        //Rotation(0.0,0.0,get_contin(*from.begin()))));
        break;

    case id::turn_to_face: {    // turn_to_face(obj)

        OC_ASSERT(from.number_of_children() == 1);
        OC_ASSERT(is_definite_object(*from.begin()));
        /*
          const string& slObjName = get_definite_object(*from.begin());
          double angleFacing = getAngleFacing(WorldWrapperUtil::selfHandle(as, selfName()));
          logger().debug("PAIWorldWrapper: angleFacing = %f", angleFacing);
          logger().debug("PAIWorldWrapper: slObjName = %s", slObjName.c_str());
          OC_ASSERT( WorldWrapperUtil::inSpaceMap(sm, as, selfName(), ownerName(), selfName()));
          OC_ASSERT( WorldWrapperUtil::inSpaceMap(sm, as, selfName(), ownerName(), slObjName));
          const SpaceServer::SpaceMapPoint& selfObjPoint = WorldWrapperUtil::getLocation(sm, as, toHandle(selfName()));
          const SpaceServer::SpaceMapPoint& slObjPoint =  WorldWrapperUtil::getLocation(sm, as, toHandle(slObjName));
          double deltaX = slObjPoint.first - selfObjPoint.first;
          double deltaY = slObjPoint.second - selfObjPoint.second;
          double angle = atan2f(deltaX,deltaY);
          double rotationAngle = angle - angleFacing;

          logger().debug("PAIWorldWrapper - Final angle = %f. Angle to turn: %f", angle, rotationAngle);
        */

        std::string targetObjectName = get_definite_object(*from.begin());

        float rotationAngle = 0;
        try {
            //const spatial::Object& agentObject = sm.getObject( selfName( ) );
            const spatial::EntityPtr& agentEntity = sm.getEntity( selfName( ) );
            //rotationAngle = agentObject.metaData.yaw;

            //const spatial::Object& targetObject = sm.getObject( targetObjectName );

            spatial::math::Vector3 targetPosition;
            if ( targetObjectName == "custom_position" ) {
                std::stringstream parser(
                    _pai.getPetInterface( ).getCurrentModeHandler( ).getPropertyValue( "customPosition" )
                );
                parser >> targetPosition.x;
                parser >> targetPosition.y;
                parser >> targetPosition.z;
            } else {
                const spatial::EntityPtr& targetEntity = sm.getEntity( targetObjectName );
                targetPosition = targetEntity->getPosition( );
            } // else

            //spatial::math::Vector2 agentPosition( agentObject.metaData.centerX, agentObject.metaData.centerY );
            //spatial::math::Vector2 targetPosition( targetObject.metaData.centerX, targetObject.metaData.centerY );

            //spatial::math::Vector2 agentDirection( cos( agentObject.metaData.yaw ), sin( agentObject.metaData.yaw ) );
            spatial::math::Vector3 targetDirection = targetPosition -  agentEntity->getPosition( );

            rotationAngle = atan2f(targetDirection.y, targetDirection.x);

            logger().debug("PAIWorldWrapper - Agent[pos: %s, ori: %s], Target[pos: %s], Turn Angle: %f", agentEntity->getPosition( ).toString( ).c_str( ),  agentEntity->getOrientation( ).toString( ).c_str( ), targetPosition.toString( ).c_str( ), rotationAngle );

        } catch ( NotFoundException& ex ) {
            logger().debug("PAIWorldWrapper - Cannot find an object inside localspacemap: %s", ex.getMessage( ) );
        } // catch

        action.addParameter(ActionParameter("rotation", ActionParamType::ROTATION(),  Rotation(0, 0, rotationAngle)));

        break;
    }

    default:
        //this will handle simple schema with no arguments - these are:
        /**
           anticipate_play
           bare_teeth
           bark
           beg
           belch
           shake_head
           clean
           run_in_circle
           drop
           back_flip
           growl
           look_up_turn_head
           jump_up
           lick
           lieDown
           tap_dance
           pee
           play_dead
           poo
           trick_for_food
           lean_rock_dance
           sit
           sleep
           fearful_posture
           sniff
           speak
           hide_face
           stretch
           vomit
           whine
           widen_eyes
        **/

        stringstream ss;
        ss << *from;
        logger().debug("PAIWorldWrapper::%s - Cannot find type for action '%s'", 
                       __FUNCTION__, ss.str().c_str( ));

        action = PetAction(ActionType::getFromName(toCamelCase(ss.str())));
    }
    return action;
}

string PAIWorldWrapper::toCamelCase(string str)
{
    for (string::iterator it = str.begin();it != str.end() - 1;++it)
        if (*it == '_') {
            it = str.erase(it);
            *it = toupper(*it);
        }
    return str;
}

string PAIWorldWrapper::resolveType(combo::vertex v)
{
    OC_ASSERT(is_definite_object(v));
    OC_ASSERT(toHandle(get_definite_object(v)) != Handle::UNDEFINED);
    return resolveType(toHandle(get_definite_object(v)));
}
string PAIWorldWrapper::resolveType(Handle h)
{
    Type objType = TLB::getAtom(h)->getType();
    return (objType == AVATAR_NODE ? AVATAR_OBJECT_TYPE :
            objType == PET_NODE ? PET_OBJECT_TYPE :
            objType == HUMANOID_NODE ? HUMANOID_OBJECT_TYPE :
            objType == ACCESSORY_NODE ? ACCESSORY_OBJECT_TYPE :
            objType == STRUCTURE_NODE ? STRUCTURE_OBJECT_TYPE :
            objType == OBJECT_NODE ? ORDINARY_OBJECT_TYPE :
            UNKNOWN_OBJECT_TYPE);
}

string PAIWorldWrapper::selfName()
{
    return _pai.getPetInterface().getPetId();
}

string PAIWorldWrapper::ownerName()
{
    return _pai.getPetInterface().getOwnerId();
}

/**
   do a lookup in:

   AtTimeLink
   TimeNode "$timestamp"
   EvalLink
   PredicateNode "AGISIM_position"
   ListLink
   ObjectNode "$obj_id"
   NumberNode "$pitch"
   NumberNode "$roll"
   NumberNode "$yaw"
**/
double PAIWorldWrapper::getAngleFacing(Handle slobj) throw (opencog::ComboException, opencog::AssertionException, std::bad_exception)
{
    const AtomSpace& as = _pai.getAtomSpace();
    const SpaceServer& spaceServer = as.getSpaceServer();
    //get the time node of the latest map, via the link AtTimeLink(TimeNode,SpaceMap)
    Handle atTimeLink = spaceServer.getLatestMapHandle();
    OC_ASSERT(atTimeLink != Handle::UNDEFINED);
#if 1
    const SpaceServer::SpaceMap& sm = spaceServer.getLatestMap();
    const string& slObjName = as.getName(slobj);
    if (sm.containsObject(slObjName)) {
        //return the yaw
        double result = sm.getEntity(slObjName)->getOrientation( ).getRoll( );//sm.getMetaData(slObjName).yaw;
        logger().debug("getAngleFacing(%s) => %f", as.getName(slobj).c_str(), result);
        return result;
    }
#else
    OC_ASSERT(TLB::getAtom(atTimeLink)->getType() == AT_TIME_LINK);
    OC_ASSERT(TLB::getAtom(atTimeLink)->getArity() == 2);

    Handle timeNode = TLB::getAtom(atTimeLink)->getOutgoingSet()[0];
    OC_ASSERT(TLB::getAtom(timeNode)->getType() == TIME_NODE);

    //now use it to lookup the AtTimeLink to our obj
    HandleSeq outgoing = boost::assign::list_of(timeNode)(Handle::UNDEFINED);
    Type types[] = {TIME_NODE, EVALUATION_LINK};
    std::vector<Handle> tmp;
    as.getHandleSet(back_inserter(tmp),
                    outgoing, types, Handle::UNDEFINED, 2, AT_TIME_LINK, true);

    foreach(Handle h, tmp) {
        //check for well-formedness
        if (TLB::getAtom(TLB::getAtom(TLB::getAtom(h)))->getArity() == 2 &&
                TLB::getAtom(TLB::getAtom(TLB::getAtom(h))->getOutgoingSet()[1])->getArity() == 2) {
            Handle evalLink = TLB::getAtom(TLB::getAtom(h))->getOutgoingSet()[1];
            if (as.getType(TLB::getAtom(evalLink)->getOutgoingSet()[0]) == PREDICATE_NODE &&
                    as.getName(TLB::getAtom(evalLink)->getOutgoingSet()[0]) == AGISIM_ROTATION_PREDICATE_NAME &&
                    TLB::getAtom(TLB::getAtom(evalLink)->getOutgoingSet()[1])->getArity() == 4) {
                Handle ll = TLB::getAtom(TLB::getAtom(TLB::getAtom(h)->getOutgoingSet()[1]))->getOutgoingSet()[1];
                if (TLB::getAtom(ll)->getOutgoingSet()[0] == slobj) {
                    //return the yaw
                    OC_ASSERT(TLB::getAtom(ll)->getOutgoingSet().size() == 4);
                    double result = lexical_cast<double>(as.getName(as.getOutgoing(ll, 3)));
                    logger().debug("getAngleFacing(%s) => %f", as.getName(slobj).c_str(), result);
                    return result;
                }
            }
        }
    }
#endif
    //_pai.getAtomSpace().print();
    std::stringstream stream (std::stringstream::out);
    stream << "Can't find angle that Object '" << as.getName(slobj)
    << "' is facing at" << std::endl;
    throw opencog::ComboException(TRACE_INFO, "PAIWorldWrapper - %s.",
                                  stream.str().c_str());
}

Handle PAIWorldWrapper::toHandle(combo::definite_object obj)
{
    return WorldWrapperUtil::toHandle(_pai.getAtomSpace(), obj,
                                      selfName(), ownerName());
}

}
