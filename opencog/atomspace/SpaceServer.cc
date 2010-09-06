/*
 * opencog/embodiment/SpaceServer.cc
 *
 * Copyright (C) 2002-2007 Novamente LLC
 * All Rights Reserved
 *
 * Written by Welter Luigi
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
#include "SpaceServer.h"
#include "TLB.h"

#include <opencog/util/Logger.h>
#include <opencog/util/StringTokenizer.h>
#include <opencog/util/StringManipulator.h>
#include <opencog/util/oc_assert.h>

#include <opencog/atomspace/atom_types.h>

#include <string>
#include <vector>
#include <cstdlib>

using namespace opencog;

const char* SpaceServer::SPACE_MAP_NODE_NAME = "SpaceMap";

SpaceServer::SpaceServer(SpaceServerContainer &_container): container(_container)
{
    // Default values (should only be used for test purposes)
    agentRadius = 0.25;
    xMin = 0;
    xMax = 256;
    yMin = 0;
    yMax = 256;
    xDim = 1024;
    yDim = 1024;
}

SpaceServer::~SpaceServer()
{
    for (HandleToSpaceMap::iterator itr = spaceMaps.begin(); itr != spaceMaps.end(); itr++) {
        delete itr->second;
    }
}

void SpaceServer::setAgentRadius(double _radius)
{
    if (agentRadius != _radius) {
        agentRadius = _radius;
        logger().info("SpaceServer - AgentRadius: %.3lf", agentRadius);
    }
}

void SpaceServer::setMapBoundaries(double _xMin, double _xMax, double _yMin, double _yMax,
                                   unsigned int _xDim, unsigned int _yDim)
{
    xMin = _xMin;
    xMax = _xMax;
    yMin = _yMin;
    yMax = _yMax;
    xDim = _xDim;
    yDim = _yDim;
    logger().info("SpaceServer - MapBondaries: xMin: %.3lf, xMax: %.3lf, yMin: %.3lf, yMax: %.3lf, xDim %d, yDim %d.", xMin, xMax, yMin, yMax, xDim, yDim);
}

SpaceServer::SpaceMap* SpaceServer::addOrGetSpaceMap(bool keepPreviousMap, Handle spaceMapHandle)
{
    SpaceMap* map;
    HandleToSpaceMap::iterator itr = spaceMaps.find(spaceMapHandle);

    if (itr == spaceMaps.end()) {
        // a new map
        logger().info(
                     "SpaceServer - New map: xMin: %.3lf, xMax: %.3lf, yMin: %.3lf, yMax: %.3lf",
                     xMin, xMax, yMin, yMax);

        if (!sortedMapHandles.empty()) {
            Handle latestMapHandle = sortedMapHandles.back();
            SpaceMap* latestMap = spaceMaps[latestMapHandle];
            if (latestMap->xMin() == xMin &&
                    latestMap->xMax() == xMax &&
                    latestMap->yMin() == yMin &&
                    latestMap->yMax() == yMax) {
                // latest map dimensions match new map dimensions
                bool mapReused = false;
                if (keepPreviousMap) {
                    if (sortedMapHandles.size() > 1 && persistentMapHandles.find(latestMapHandle) == persistentMapHandles.end()) {
                        Handle lastButOneMapHandle = *(sortedMapHandles.end() - 2);
                        SpaceMap* lastButOneMap = spaceMaps[lastButOneMapHandle];
                        // Check if the 2 latest maps are equals
                        if (*latestMap == *lastButOneMap) {
                            logger().debug("SpaceServer - The 2 previous maps are equals. Previous map (%s) transfered to new map (%s).", TLB::getAtom(latestMapHandle)->toString().c_str(), TLB::getAtom(spaceMapHandle)->toString().c_str());
                            sortedMapHandles.erase(sortedMapHandles.end() - 1);
                            spaceMaps.erase(latestMapHandle);
                            container.mapRemoved(latestMapHandle);
                            map = latestMap; // reuse the spaceMap object
                            mapReused = true;
                            persistentMapHandles.insert(lastButOneMapHandle);
                            container.mapPersisted(lastButOneMapHandle);
                            logger().debug("SpaceServer - Map (%s) marked as persistent.", TLB::getAtom(lastButOneMapHandle)->toString().c_str());
                        } else {
                            persistentMapHandles.insert(latestMapHandle);
                            container.mapPersisted(latestMapHandle);
                            logger().debug("SpaceServer - Map (%s) marked as persistent.", TLB::getAtom(latestMapHandle)->toString().c_str());
                        }
                    } else {
                        persistentMapHandles.insert(latestMapHandle);
                        container.mapPersisted(latestMapHandle);
                        logger().debug("SpaceServer - Map (%s) marked as persistent.", TLB::getAtom(latestMapHandle)->toString().c_str());
                    }
                } else if (persistentMapHandles.find(latestMapHandle) == persistentMapHandles.end()) {
                    logger().debug("SpaceServer - Previous map (%s) transfered to new map (%s).", TLB::getAtom(latestMapHandle)->toString().c_str(), TLB::getAtom(spaceMapHandle)->toString().c_str());
                    sortedMapHandles.erase(sortedMapHandles.end() - 1);
                    spaceMaps.erase(latestMapHandle);
                    container.mapRemoved(latestMapHandle);
                    map = latestMap; // reuse the spaceMap object
                    mapReused = true;
                }
                if (!mapReused) {
                    // Create the new one by cloning the latest map
                    logger().debug("SpaceServer - New map (%s) cloned from previous map (%s).", TLB::getAtom(spaceMapHandle)->toString().c_str(), TLB::getAtom(latestMapHandle)->toString().c_str());
                    map = latestMap->clone();
                }
            } else {
                // latest map dimensions do not match new map dimensions.
                // Create an empty map
                logger().debug("SpaceServer - New map (%s) created by copying.", TLB::getAtom(spaceMapHandle)->toString().c_str());
                map = new SpaceMap(xMin, xMax, xDim, yMin, yMax, yDim, agentRadius);
                // Copy each object in latest map into the new map
                map->copyObjects(*latestMap);
                if (!keepPreviousMap) {
                    logger().debug("SpaceServer - Previous map (%s) removed.", TLB::getAtom(latestMapHandle)->toString().c_str());

                    sortedMapHandles.erase(sortedMapHandles.end() - 1);
                    spaceMaps.erase(latestMapHandle);
                    container.mapRemoved(latestMapHandle);
                    delete latestMap;
                }
            }
        } else {
            // Create first map
            map = new SpaceMap(xMin, xMax, xDim, yMin, yMax, yDim, agentRadius);
            logger().debug("SpaceServer - First map (%s) created", TLB::getAtom(spaceMapHandle)->toString().c_str());
        }
        spaceMaps[spaceMapHandle] = map;
        sortedMapHandles.push_back(spaceMapHandle);
        container.mapPersisted(spaceMapHandle); // Ensure the latest map will not be removed by forgetting mechanism
        logger().debug("SpaceServer - spaceMaps size: %u, sortedMapHandles size: %u", spaceMaps.size(), sortedMapHandles.size());

    } else {
        // get the existing map
        map = itr->second;
    }
    return map;
}

bool SpaceServer::add(bool keepPreviousMap, Handle spaceMapHandle, const std::string& objectId,
                      double centerX, double centerY, double centerZ,
                      double length, double width, double height,
                      double yaw, bool isObstacle)
{


    SpaceMap* map = addOrGetSpaceMap(keepPreviousMap, spaceMapHandle);
//    logger().fine("SpaceServer::add After addOrGet");

    logger().fine(
                 "SpaceServer::add map->xMin() = %lf, map->xMax() = %lf, map->yMin() = %lf, map->yMax() = %lf, map->xGridWidth() = %lf, map->yGridWidth() = %lf",
                 map->xMin(), map->xMax(), map->yMin(), map->yMax(),
                 map->xGridWidth(), map->yGridWidth());

    SpaceServer::ObjectMetadata metadata(centerX, centerY, centerZ, length, width, height, yaw);
    bool mapContainsObject = map->containsObject(objectId);
    bool needUpdate = false;
//    logger().fine("SpaceServer::add After contains");
    if (mapContainsObject) {
        //const SpaceServer::ObjectMetadata& oldMetadata = map->getMetaData(objectId);
        const spatial::EntityPtr& oldEntity = map->getEntity( objectId );
        SpaceServer::ObjectMetadata oldMetadata( oldEntity->getPosition( ).x,
                                                 oldEntity->getPosition( ).y,
                                                 oldEntity->getPosition( ).z,
                                                 oldEntity->getLength( ),
                                                 oldEntity->getWidth( ),
                                                 oldEntity->getHeight( ),
                                                 oldEntity->getOrientation( ).getRoll( ) );

//    logger().fine("SpaceServer::add After getMetaData");

        if (metadata != oldMetadata) {
            needUpdate = true;
            logger().fine(
                         "SpaceServer::add Old metadata (x=%lf, y=%lf, length=%lf, width=%lf, height=%lf, yaw=%lf) is different: object must be updated",
                         oldMetadata.centerX, oldMetadata.centerY, oldMetadata.centerZ,
                         oldMetadata.length, oldMetadata.width, oldMetadata.height,
                         oldMetadata.yaw);
        } else {
            bool wasObstacle = map->isObstacle(objectId);
            if (isObstacle != wasObstacle) {
                needUpdate = true;
                logger().fine("SpaceServer::add Object is %san obstacle now. So, it must be updated.",
                             isObstacle ? " " : "not ");
            }
        }
    } else {
        logger().fine("SpaceServer::add Object does not exist in the map yet. So, it will be added.");
    }

    if (!mapContainsObject || needUpdate) {

        logger().debug(
                     "SpaceServer - add(mapH=%lu, objId=%s, x=%lf, y=%lf, length=%lf, width=%lf, height=%lf, yaw=%lf, isObstacle=%d)",
                     spaceMapHandle.value(), objectId.c_str(), centerX, centerY, centerZ,
                     length, width, height, yaw, isObstacle);

        if (mapContainsObject) {
            logger().fine(
                         "SpaceServer::add - updating object into the space map");

            map->updateObject(objectId, metadata, isObstacle );

        } else {
            logger().fine("SpaceServer::add - adding object into the space map");
            map->addObject(objectId, metadata, isObstacle );
        }
        return true;
    }
    return false;
}

void SpaceServer::add(Handle spaceMapHandle, SpaceMap * map)
{

    logger().info("SpaceServer - New map (%s) added",
                 TLB::getAtom(spaceMapHandle)->toString().c_str());
    sortedMapHandles.push_back(spaceMapHandle);
    spaceMaps[spaceMapHandle] = map;
    logger().debug("SpaceServer - spaceMaps size: %d",
                 spaceMaps.size());

}

void SpaceServer::remove(bool keepPreviousMap, Handle spaceMapHandle, const std::string& objectId)
{
    logger().info("SpaceServer::remove()");
    SpaceMap* map = addOrGetSpaceMap(keepPreviousMap, spaceMapHandle);
    if (map->containsObject(objectId)) map->removeObject(objectId);
}

const SpaceServer::SpaceMap& SpaceServer::getMap(Handle spaceMapHandle) const throw (opencog::RuntimeException, std::bad_exception)
{
    logger().fine("SpaceServer::getMap() for mapHandle = %s", spaceMapHandle != Handle::UNDEFINED ? TLB::getAtom(spaceMapHandle)->toString().c_str() : "Handle::UNDEFINED");

    HandleToSpaceMap::const_iterator itr = spaceMaps.find(spaceMapHandle);

    if (itr == spaceMaps.end()) {
        throw opencog::RuntimeException(TRACE_INFO,
                                        "SpaceServer - Found no SpaceMap associate with handle: '%s'.",
                                        TLB::getAtom(spaceMapHandle)->toString().c_str());
    }

    return *(itr->second);
}

const bool SpaceServer::containsMap(Handle spaceMapHandle) const
{
    return (spaceMaps.find(spaceMapHandle) != spaceMaps.end());
}

const bool SpaceServer::isLatestMapValid() const
{
    return (!sortedMapHandles.empty());
}

const SpaceServer::SpaceMap& SpaceServer::getLatestMap() const throw (opencog::AssertionException, std::bad_exception)
{
    OC_ASSERT(isLatestMapValid(), "SpaceServer - No lastestMap avaiable to return.");
    HandleToSpaceMap::const_iterator itr = spaceMaps.find(getLatestMapHandle());
    return *(itr->second);
}

Handle SpaceServer::getLatestMapHandle() const
{
    if (sortedMapHandles.empty()) {
        return Handle::UNDEFINED;
    }
    return sortedMapHandles.back();
}

Handle SpaceServer::getOlderMapHandle() const
{
    if (sortedMapHandles.empty()) {
        return Handle::UNDEFINED;
    }
    return sortedMapHandles.front();
}

Handle SpaceServer::getPreviousMapHandle(Handle spaceMapHandle) const
{
    Handle result = Handle::UNDEFINED;
    if (spaceMapHandle != Handle::UNDEFINED) {
        std::vector<Handle>::const_iterator itr = std::lower_bound(sortedMapHandles.begin(), sortedMapHandles.end(), spaceMapHandle);
        if (itr != sortedMapHandles.begin() && *itr == spaceMapHandle) {
            result = *(--itr);
        }
    }
    return result;
}

Handle SpaceServer::getNextMapHandle(Handle spaceMapHandle) const
{
    Handle result = Handle::UNDEFINED;
    if (spaceMapHandle != Handle::UNDEFINED) {
        std::vector<Handle>::const_iterator itr = std::lower_bound(sortedMapHandles.begin(), sortedMapHandles.end(), spaceMapHandle);
        if (*itr == spaceMapHandle) {
            ++itr;
            if (itr != sortedMapHandles.end())
                result = *itr;
        }
    }
    return result;
}

void SpaceServer::removeMap(Handle spaceMapHandle)
{
    HandleToSpaceMap::iterator itr = spaceMaps.find(spaceMapHandle);
    if (itr != spaceMaps.end()) {

        std::vector<Handle>::iterator itr_map = std::find(sortedMapHandles.begin()
                                                , sortedMapHandles.end(), spaceMapHandle);
        if (*itr_map == spaceMapHandle) {
            sortedMapHandles.erase(itr_map);
        } else {
            if (itr_map != sortedMapHandles.end()) {
                logger().error("SpaceServer::removeSpaceMap - Removed is not mapHandle.\n");
                sortedMapHandles.erase(itr_map);
            }
            logger().error("SpaceServer::removeSpaceMap - Trying to remove inexisting map. spaceMapSize = %d sortedMapHandlesSize = %d\n", 
                    spaceMaps.size(), sortedMapHandles.size());
        }

        delete itr->second;
        spaceMaps.erase(itr);

    }
}

void SpaceServer::markMapAsPersistent(Handle spaceMapHandle)
{
    HandleToSpaceMap::const_iterator itr = spaceMaps.find(spaceMapHandle);

    if (itr == spaceMaps.end()) {
        throw opencog::RuntimeException(TRACE_INFO,
                                        "SpaceServer - Found no SpaceMap associate with handle: '%s'.",
                                        TLB::getAtom(spaceMapHandle)->toString().c_str());
    }
    persistentMapHandles.insert(spaceMapHandle);
    container.mapPersisted(spaceMapHandle);
}

bool SpaceServer::isMapPersistent(Handle spaceMapHandle) const
{
    return (persistentMapHandles.find(spaceMapHandle) != persistentMapHandles.end());
}

void SpaceServer::removeObject(const std::string& objectId)
{
    for (HandleToSpaceMap::iterator itr = spaceMaps.begin(); itr != spaceMaps.end(); itr++) {
        itr->second->removeObject(objectId);
    }
}

unsigned int SpaceServer::getSpaceMapsSize() const
{
    return spaceMaps.size();
}

void SpaceServer::clear()
{
    for (HandleToSpaceMap::iterator itr = spaceMaps.begin(); itr != spaceMaps.end(); itr++) {
        delete itr->second;
    }
    spaceMaps.clear();
}


std::string SpaceServer::mapToString(Handle mapHandle) const
{

    std::stringstream stringMap;
    stringMap.precision(25);
    const SpaceMap& map = getMap(mapHandle);

    stringMap << container.getMapIdString(mapHandle);
    stringMap << " ";

    stringMap << SpaceMap::toString( map );

    return stringMap.str( );
}

SpaceServer::TimestampMap SpaceServer::mapFromString(const std::string& stringMap)
{

    unsigned long timestamp;
    std::stringstream mapParser;
    {
        std::stringstream parser( stringMap );
        parser.precision(25);    
        parser >> timestamp;
        mapParser << parser.rdbuf( );
    }        

    SpaceServer::TimestampMap timestampMap(timestamp, SpaceMap::fromString( mapParser.str( ) ) );

    return timestampMap;
}

SpaceServer& SpaceServer::operator=(const SpaceServer& other)
{
    throw opencog::RuntimeException(TRACE_INFO, 
            "SpaceServer - Cannot copy an object of this class");
}

class FakeContainer: public SpaceServerContainer {
    void mapRemoved(Handle mapId) {}
    void mapPersisted(Handle mapId) {}
    std::string getMapIdString(Handle mapId) {return "";}

};
SpaceServer::SpaceServer(const SpaceServer& other): container(*(new FakeContainer())) 
{
    throw opencog::RuntimeException(TRACE_INFO, 
            "SpaceServer - Cannot copy an object of this class");
}

