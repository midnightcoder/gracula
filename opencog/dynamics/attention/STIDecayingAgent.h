/*
 * opencog/dynamics/attention/STIDecayingAgent.h
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


/* -Short description-
 * This agent implements another forgetting mechanism, way
 * simpler than Joel's forgetting agent. It mimmics the forgetting mechanism
 * used by Petaverse's novamente */

#ifndef _OPENCOG_STI_DECAYING_AGENT_H
#define _OPENCOG_STI_DECAYING_AGENT_H

#include <opencog/atomspace/AtomSpace.h>
#include <opencog/server/Agent.h>
#include <opencog/util/Logger.h>

namespace opencog
{

class CogServer;

class STIDecayingAgent : public Agent
{

public:

    virtual const ClassInfo& classinfo() const { return info(); }
    static const ClassInfo& info() {
        static const ClassInfo _ci("opencog::STIDecayingAgent");
        return _ci;
    }

    STIDecayingAgent();
    virtual ~STIDecayingAgent();
    virtual void run(CogServer *server);

}; // class

} // namespace

#endif // _OPENCOG_STI_DECAYING_AGENT_H
