/*
 * opencog/embodiment/Control/MessagingSystem/Message.cc
 *
 * Copyright (C) 2002-2009 Novamente LLC
 * All Rights Reserved
 * Author(s): Andre Senna
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


#include <stdio.h>
#include "Message.h"
#include "MessagingSystemExceptions.h"
#include "StringMessage.h"
#include "RouterMessage.h"
#include <opencog/embodiment/Learning/LearningServerMessages/RewardMessage.h>
#include <opencog/embodiment/Control/MessagingSystem/TickMessage.h>
#include <opencog/embodiment/Control/MessagingSystem/FeedbackMessage.h>
#include <opencog/embodiment/Learning/LearningServerMessages/SchemaMessage.h>
#include <opencog/embodiment/Learning/LearningServerMessages/LearnMessage.h>
#include <opencog/embodiment/Learning/LearningServerMessages/LSCmdMessage.h>
#include <opencog/embodiment/Learning/LearningServerMessages/TrySchemaMessage.h>
#include <opencog/embodiment/Learning/LearningServerMessages/StopLearningMessage.h>

namespace MessagingSystem
{

Message::~Message()
{
}

Message::Message(const std::string &from, const std::string &to, int type)
{
    this->from = from;
    this->to = to;
    this->type = type;
}

// Factory method

Message *Message::factory(const std::string &from, const std::string &to, int msgType, const std::string &msg) throw (opencog::InvalidParamException, std::bad_exception)
{

    switch (msgType) {

    case STRING: {
        return new StringMessage(from, to, msg);
        break;
    }
    case LEARN: {
        return new LearningServerMessages::LearnMessage(from, to, msg);
        break;
    }
    case REWARD: {
        return new LearningServerMessages::RewardMessage(from, to, msg);
        break;
    }
    case TRY: {
        return new LearningServerMessages::TrySchemaMessage(from, to, msg);
        break;
    }
    case STOP_LEARNING: {
        return new LearningServerMessages::StopLearningMessage(from, to, msg);
        break;
    }
    case SCHEMA:
    case CANDIDATE_SCHEMA: {
        // same message class, but with diferent types,
        // CANDIDATE_SCHEMA besides the schema being learned
        // have the name of the candidate to be tried out
        return new LearningServerMessages::SchemaMessage(from, to, msg, msgType);
        break;
    }
    case LS_CMD: {
        std::cout << "LS_CMD : " << msg << std::endl;
        return new LearningServerMessages::LSCmdMessage(from, to, msg);
        break;
    }
    case TICK: {
        return new TickMessage(from, to);
        break;
    }
    case FEEDBACK: {
        return new FeedbackMessage(from, to, msg);
        break;
    }
    default: {
        throw opencog::InvalidParamException(TRACE_INFO,
                                             "Message - Unknown message type id: '%d'.", msgType);
    }
    }

    // to remove a warning, execution should never reach here.
    return NULL;
}


Message *Message::routerMessageFactory(const std::string &from, const std::string &to, int encapsulateMsgType, const std::string &msg)
{
    return new RouterMessage(from, to, encapsulateMsgType, msg);
}

// Setters and getters

void Message::setFrom(const std::string &from)
{
    this->from = from;
}

void Message::setTo(const std::string &to)
{
    this->to = to;
}

void Message::setType(int type)
{
    this->type = type;
}

const std::string &Message::getFrom() const
{
    return from;
}

const std::string &Message::getTo() const
{
    return to;
}

int Message::getType() const
{
    return type;
}

}
