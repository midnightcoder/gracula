/*
 * opencog/atomspace/types.h
 *
 * Copyright (C) 2002-2007 Novamente LLC
 * All Rights Reserved
 *
 * Written by Thiago Maia <thiago@vettatech.com>
 *            Andre Senna <senna@vettalabs.com>
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

/**
 * basic type definitions.
 */

#ifndef _OPENCOG_TYPES_H
#define _OPENCOG_TYPES_H

#include <boost/variant.hpp>

#include <opencog/atomspace/Handle.h>

namespace opencog
{

// type and arity of Atoms, represented as short integers (16 bits)
typedef unsigned short Type;
typedef unsigned short Arity;
// stimulus
typedef short stim_t;

typedef boost::variant<Handle, Type, int, unsigned int, float, bool,
                       unsigned char, char, short int> Vertex;

} // namespace opencog

#endif // _OPENCOG_TYPES_H
