/*
 * opencog/embodiment/Control/PredicateUpdaters/PredicatesUpdater.cc
 *
 * Copyright (C) 2002-2009 Novamente LLC
 * All Rights Reserved
 * Author(s): Carlos Lopes
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

#include "PredicatesUpdater.h"
#include "NearPredicateUpdater.h"
#include "IsSmallPredicateUpdater.h"
#include "IsNoisyPredicateUpdater.h"
#include "IsMovablePredicateUpdater.h"
#include "IsPooPlacePredicateUpdater.h"
#include "IsPeePlacePredicateUpdater.h"
#include "IsPickupablePredicateUpdater.h"
#include "PetPsychePredicatesUpdater.h"


// this time frame corresponde to one minute
static const unsigned long timeWindow = 600;

using namespace OperationalPetController;

PredicatesUpdater::PredicatesUpdater(AtomSpace &_atomSpace, const std::string &_petId) :
        atomSpace(_atomSpace), petId(_petId)
{
    logger().debug("%s - PetId: '%s'.", __FUNCTION__, _petId.c_str());

    // perceptual predicates
    updaters.push_back(new IsSmallPredicateUpdater(atomSpace));
    updaters.push_back(new IsNoisyPredicateUpdater(atomSpace));
    updaters.push_back(new IsMovablePredicateUpdater(atomSpace));
    updaters.push_back(new IsPickupablePredicateUpdater(atomSpace));
    updaters.push_back(new IsPooPlacePredicateUpdater(atomSpace));
    updaters.push_back(new IsPeePlacePredicateUpdater(atomSpace));
    // relation predicates
    updaters.push_back(new NearPredicateUpdater(atomSpace));

    petPsychePredicatesUpdater = new PetPsychePredicatesUpdater(atomSpace);
}

PredicatesUpdater::~PredicatesUpdater()
{
    foreach(BasicPredicateUpdater* updater, updaters) {
        delete updater;
    }
    updaters.clear();
    delete this->petPsychePredicatesUpdater;
}

void PredicatesUpdater::update(std::vector<Handle> objects, unsigned long timestamp)
{

    Handle petHandle = atomSpace.getHandle(PET_NODE, petId);
    if ( petHandle == Handle::UNDEFINED ) {
        petHandle = atomSpace.getHandle(HUMANOID_NODE, petId);
    } // if

    for (unsigned int i = 0; i < objects.size(); i++) {
        // updating all predicates ...
        for (unsigned int j = 0; j < updaters.size(); j++) {
            updaters[j]->update(objects[i], petHandle, timestamp );
        }

    } // for

    if (objects.size() > 0) {
        petPsychePredicatesUpdater->update( Handle::UNDEFINED, petHandle, timestamp );
    }
}