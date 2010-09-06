/*
 * opencog/server/Agent.cc
 *
 * Copyright (C) 2008 by Singularity Institute for Artificial Intelligence
 * All Rights Reserved
 *
 * Written by Gustavo Gama <gama@vettalabs.com>
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

#include "Agent.h"

#include <opencog/server/CogServer.h>
#include <opencog/util/Config.h>

using namespace opencog;

Agent::Agent(const unsigned int f) : _frequency(f)
{
    // an empty set of parameters and defaults (so that various
    // methods will still work even if none are set in this or a derived
    // class)
    static const std::string defaultConfig[] = {
        "", ""
    };
    setParameters(defaultConfig);

    stimulatedAtoms = new AtomStimHashMap();
    totalStimulus = 0;

    conn = server().getAtomSpace()->removeAtomSignal().connect(
            boost::bind(&Agent::atomRemoved, this, _1));
}

Agent::~Agent()
{
    // give back funds
    server().getAtomSpace()->setSTI(this, 0);
    server().getAtomSpace()->setLTI(this, 0);

    resetUtilizedHandleSets();
    delete stimulatedAtoms;
    conn.disconnect();
}

void Agent::setParameters(const std::string* params) {
    PARAMETERS = params;

    for (unsigned int i = 0; params[i] != ""; i += 2) {
        if (!config().has(params[i])) {
           config().set(params[i], params[i + 1]);
        }
    }
}

std::string Agent::to_string() const
{
    std::ostringstream oss;
    oss << classinfo().id;
    oss << " {\"";
    for (unsigned int i = 0; PARAMETERS[i] != ""; i += 2) {
        if (i != 0) oss << "\", \"";
        oss << PARAMETERS[i] << "\" => \"" << config()[PARAMETERS[i]];
    }
    oss << "\"}";
    return oss.str();
}

void Agent::atomRemoved(Handle h)
{
    for (size_t i = 0; i < _utilizedHandleSets.size(); i++)
        if (_utilizedHandleSets[i]->contains(h))
            _utilizedHandleSets[i]->remove(h);
    removeAtomStimulus(h);
}

void Agent::resetUtilizedHandleSets()
{
    for (size_t i = 0; i < _utilizedHandleSets.size(); i++)
        delete _utilizedHandleSets[i];
    _utilizedHandleSets.clear();
}


stim_t Agent::stimulateAtom(Handle h, stim_t amount)
{
    // Add atom to the map of atoms with stimulus
    // and add stimulus to it
    (*stimulatedAtoms)[h] += amount;

    // update record of total stimulus given out
    totalStimulus += amount;
    //logger().fine("%d added to totalStimulus, now %d", amount, totalStimulus);
    return totalStimulus;
}

void Agent::removeAtomStimulus(Handle h)
{
    stim_t amount;
    // if handle not in map then return
    if (stimulatedAtoms->find(h) == stimulatedAtoms->end())
        return;

    amount = (*stimulatedAtoms)[h];
    stimulatedAtoms->erase(h);

    // update record of total stimulus given out
    totalStimulus -= amount;
}

stim_t Agent::stimulateAtom(HandleEntry* h, stim_t amount)
{
    HandleEntry* p;
    stim_t split;

    // how much to give each atom
    split = amount / h->getSize();

    p = h;
    while (p) {
        stimulateAtom(p->handle, split);
        p = p->next;
    }

    // return unused stimulus
    return amount - (split * h->getSize());
}

stim_t Agent::resetStimulus()
{
    stimulatedAtoms->clear();
    // reset stimulus counter
    totalStimulus = 0;
    return totalStimulus;
}

stim_t Agent::getTotalStimulus() const
{
    return totalStimulus;
}

stim_t Agent::getAtomStimulus(Handle h) const
{
    if (stimulatedAtoms->find(h) == stimulatedAtoms->end()) {
        return 0;
    } else {
        return (*stimulatedAtoms)[h];
    }
}

