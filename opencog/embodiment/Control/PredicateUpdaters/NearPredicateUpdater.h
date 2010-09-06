/*
 * opencog/embodiment/Control/PredicateUpdaters/NearPredicateUpdater.h
 *
 * Copyright (C) 2002-2009 Novamente LLC
 * All Rights Reserved
 * Author(s): Ari Heljakka, Welter Luigi
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

#ifndef NEARPREDICATEUPDATER_H_
#define NEARPREDICATEUPDATER_H_

/**
 * This class is used to update the near predicates whenever an object
 * changes its position in the latest SpaceMap.
 */
#include "BasicPredicateUpdater.h"
#include <opencog/atomspace/AtomSpace.h>

namespace OperationalPetController
{

class NearPredicateUpdater : public OperationalPetController::BasicPredicateUpdater
{

public:

    NearPredicateUpdater(AtomSpace& _atomSpace);
    ~NearPredicateUpdater();

    virtual void update(Handle object, Handle pet, unsigned long timestamp );

 protected:
    
    void setPredicate( const Handle& entityA, const Handle& entityB, const std::string& predicateName, float mean );    

    unsigned long lastTimestamp;

    boost::unordered_set<std::string, boost::hash<std::string> > processedEntities;
        

    
}; // class
}  // namespace

#endif

