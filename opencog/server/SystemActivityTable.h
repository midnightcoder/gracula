/*
 * opencog/server/SystemActivityTable.h
 *
 * Copyright (C) 2009 by Singularity Institute for Artificial Intelligence
 * All Rights Reserved
 *
 * Written by Trent Waddington <trent.waddington@gmail.com>
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

#ifndef _OPENCOG_SYSTEM_ACTIVITY_TABLE_H
#define _OPENCOG_SYSTEM_ACTIVITY_TABLE_H

#include <string>
#include <queue>
#include <map>
#include <vector>

#include <pthread.h>

#include <opencog/server/Agent.h>
#include <opencog/server/SystemActivityTable.h>
#include <opencog/util/Logger.h>

namespace opencog
{

/**
 * This class implements storage for activity data.
 */
class Activity
{
public:
    Activity(long cycleCount, time_t elapsedTime, size_t memUsed, 
                              size_t atomsUsed,
                              const HandleSetSeq &utilized) :
            cycleCount(cycleCount), 
            elapsedTime(elapsedTime), 
            memUsed(memUsed),
            atomsUsed(atomsUsed) {
        for (size_t n = 0; n < utilized.size(); n++)
            utilizedHandleSets.push_back(utilized[n]->clone());
    }
    ~Activity() {
        for (size_t n = 0; n < utilizedHandleSets.size(); n++)
            delete utilizedHandleSets[n];
    }
    long cycleCount;
    time_t elapsedTime;
    size_t memUsed;
    size_t atomsUsed;
    HandleSetSeq utilizedHandleSets;
};
typedef std::vector<Activity*> ActivitySeq;
typedef std::map<Agent*, ActivitySeq> AgentActivityTable;

class CogServer;

/**
 * This class implements the entity responsible for logging of all system
 * activity.
 *
 * See http://opencog.org/wiki/OpenCogPrime:AttentionalDataMining
 */
class SystemActivityTable
{
protected:
    AgentActivityTable _agentActivityTable;
    size_t _maxAgentActivityTableSeqSize;
    CogServer *cogServer;
    boost::signals::connection conn;

    /** called by AtomTable via a boost:signal when an atom is removed. */
    void atomRemoved(Handle h);

    /** trim Activity sequence to a maximum size */
    void trimActivitySeq(ActivitySeq &seq, size_t max);

public:

    /** constructor. */
    SystemActivityTable();

    /** destructor. */
    virtual ~SystemActivityTable();

    /** initialize the SystemActivityTable */
    virtual void init(CogServer *cogServer);

    /** Returns a reference to the agent activity table 
     *  Activities will be listed with the most recent first. */
    AgentActivityTable& agentActivityTable() {
        return _agentActivityTable;
    }

    /** Get the maximum size of a sequence in the AgentActivityTable */
    size_t maxAgentActivityTableSeqSize() const { 
        return _maxAgentActivityTableSeqSize; 
    }

    /** Set the maximum size of a sequence in the AgentActivityTable */
    void setMaxAgentActivityTableSeqSize(size_t n);

    /** Logs activity of an Agent.
     *
     *  This will call agent->getUtilizedHandleSets() to get a list of handle
     *  sets utilized in the activity that has just been completed.
     */
    void logActivity(Agent *agent, time_t elapsedTime, size_t memUsed, 
                                   size_t atomsUsed);

    /** Clear activity of a specified Agent. */
    void clearActivity(Agent *agent); 

    /** Clear all activity */
    void clearActivity();

}; // class

}  // namespace

#endif // _OPENCOG_SYSTEM_ACTIVITY_TABLE_H
