/*
 * opencog/embodiment/Learning/behavior/PAIWorldProvider.cc
 *
 * Copyright (C) 2002-2009 Novamente LLC
 * All Rights Reserved
 * Author(s): Novamente team
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

#include "PAIWorldProvider.h"

PAIWorldProvider::PAIWorldProvider(PerceptionActionInterface::PAI* _pai) : pai(_pai)
{}

unsigned long PAIWorldProvider::getLatestSimWorldTimestamp() const
{
    return pai->getLatestSimWorldTimestamp();
}

AtomSpace& PAIWorldProvider::getAtomSpace() const
{
    return pai->getAtomSpace();
}
