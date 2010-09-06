/*
 * opencog/embodiment/Learning/LearningServer/LSMocky.h
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

#ifndef LSMOCKY_H
#define LSMOCKY_H

#include <opencog/util/Logger.h>
#include <opencog/atomspace/AtomSpace.h>
#include <opencog/embodiment/Control/MessagingSystem/EmbodimentCogServer.h>
#include <opencog/embodiment/Control/EmbodimentConfig.h>
#include "SleepAgent.h"

using namespace opencog;

namespace LearningServer
{

class LSMocky : public MessagingSystem::EmbodimentCogServer
{

public:

    /**
     * Constructor and Destructor
     */
    static BaseServer* createInstance();
    LSMocky();
    void init(const std::string &myId, const std::string &ip, int portNumber);
    ~LSMocky();

    bool processNextMessage(MessagingSystem::Message *msg);

    Factory<SleepAgent, Agent> sleepAgentFactory;

private:

    AtomSpace * atomSpace;   // store behavior descriptors and space server
    // with latest map

    std::string learningPet;  // the id of the pet using the LS
    std::string learningSchema;     // the trick being learned

}; // class
}  // namespace

#endif
