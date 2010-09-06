/*
 * opencog/atomspace/LinkIndex.cc
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

#include <opencog/atomspace/Link.h>
#include <opencog/atomspace/LinkIndex.h>
#include <opencog/atomspace/ClassServer.h>
#include <opencog/atomspace/HandleEntry.h>
#include <opencog/atomspace/TLB.h>
#include <opencog/atomspace/atom_types.h>

using namespace opencog;

LinkIndex::LinkIndex(void)
{
	resize();
}

void LinkIndex::resize()
{
	idx.resize(classserver().getNumberOfClasses());
}

void LinkIndex::insertHandle(Handle h)
{
	Atom *a = TLB::getAtom(h);
	Type t = a->getType();
	HandleSeqIndex &hsi = idx[t];

	Link *l = dynamic_cast<Link *>(a);
	if (NULL == l) return;

	hsi.insert(l->getOutgoingSet(),h);
}

void LinkIndex::removeHandle(Handle h)
{
	Atom *a = TLB::getAtom(h);
	Type t = a->getType();
	HandleSeqIndex &hsi = idx[t];

	Link *l = dynamic_cast<Link *>(a);
	if (NULL == l) return;

	hsi.remove(l->getOutgoingSet(),h);
}

Handle LinkIndex::getHandle(Type t, const HandleSeq &seq) const
{
	if (t >= idx.size()) throw RuntimeException(TRACE_INFO, 
            "Index out of bounds for atom type (t = %lu)", t);
	const HandleSeqIndex &hsi = idx[t];
	return hsi.get(seq);
}

void LinkIndex::remove(bool (*filter)(Handle))
{
	std::vector<HandleSeqIndex>::iterator s;
	for (s = idx.begin(); s != idx.end(); s++)
	{
		s->remove(filter);
	}
}

HandleEntry * LinkIndex::getHandleSet(Type type, const HandleSeq& seq, bool subclass) const
{
	if (subclass)
	{
		HandleEntry *he = NULL;
		
		int max = classserver().getNumberOfClasses();
		for (Type s = 0; s < max; s++)
		{
			// The 'AssignableFrom' direction is unit-tested in AtomSpaceUTest.cxxtest
			if (classserver().isA(s, type))
			{
				if (s >= idx.size()) throw RuntimeException(TRACE_INFO, 
                        "Index out of bounds for atom type (s = %lu)", s);
				const HandleSeqIndex &hsi = idx[s];
				Handle h = hsi.get(seq);
				if (TLB::isValidHandle(h))
					he = new HandleEntry(h, he);
			}
		}
		return he;
	}
	else
	{
		return new HandleEntry(getHandle(type, seq));
	}
}

// ================================================================
