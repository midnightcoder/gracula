/*
 * opencog/atomspace/LinkIndex.h
 *
 * Copyright (C) 2008,2009 Linas Vepstas <linasvepstas@gmail.com>
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

#ifndef _OPENCOG_LINK_INDEX_H
#define _OPENCOG_LINK_INDEX_H

#include <set>
#include <vector>

#include <opencog/atomspace/HandleSeqIndex.h>
#include <opencog/atomspace/types.h>

namespace opencog
{
class HandleEntry;

/**
 * Implements an (type, HandleSeq) index array of RB-trees (C++ set)
 */
class LinkIndex
{
    private:
    public:
        std::vector<HandleSeqIndex> idx;
        LinkIndex(void);
        void insertHandle(Handle);
        void removeHandle(Handle);
        void remove(bool (*)(Handle));
        void resize();

        Handle getHandle(Type type, const HandleSeq&) const;
        HandleEntry* getHandleSet(Type type, const HandleSeq &, bool subclass) const;
};

} //namespace opencog

#endif // _OPENCOG_LINK_INDEX_H
