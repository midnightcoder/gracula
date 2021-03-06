/*
 * opencog/atomspace/NodeIndex.h
 *
 * Copyright (C) 2008 Linas Vepstas <linasvepstas@gmail.com>
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

#ifndef _OPENCOG_NODEINDEX_H
#define _OPENCOG_NODEINDEX_H

#include <set>
#include <vector>

#include <opencog/atomspace/NameIndex.h>
#include <opencog/atomspace/types.h>

namespace opencog
{
class HandleEntry;

/**
 * Implements an (type, name) index array of RB-trees (C++ set)
 */
class NodeIndex
{
	private:
	public:
		std::vector<NameIndex> idx;
		NodeIndex(void);
		void insertHandle(Handle);
		void removeHandle(Handle);
		void remove(bool (*)(Handle));
        void resize();

		Handle getHandle(Type type, const char *) const;
		HandleEntry* getHandleSet(Type type, const char *, bool subclass) const;
};

} //namespace opencog

#endif // _OPENCOG_NODEINDEX_H
