/*
 * opencog/embodiment/Control/OperationalPetController/ActionSelectionAgent.cc
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


#include "OPC.h"
#include "ActionSelectionAgent.h"

using namespace OperationalPetController;

ActionSelectionAgent::~ActionSelectionAgent()
{
}

ActionSelectionAgent::ActionSelectionAgent()
{
    lastTickTime = 0;
}

void ActionSelectionAgent::run(opencog::CogServer *server)
{
    logger().debug("ActionSelectionAgent - Executing schemaSelection().");
    ((OPC *) server)->schemaSelection();
}
