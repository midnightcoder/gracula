/*
 * PatternMatch.h
 *
 * Author: Linas Vepstas February 2008
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

#ifndef _OPENCOG_PATTERN_MATCH_H
#define _OPENCOG_PATTERN_MATCH_H

#include <map>
#include <vector>

#include <opencog/atomspace/types.h>
#include <opencog/atomspace/Atom.h>
#include <opencog/query/DefaultPatternMatchCB.h>
#include <opencog/query/PatternMatchEngine.h>

namespace opencog {

class PatternMatch
{
	private:
		AtomSpace *atom_space;
		PatternMatchEngine pme;
		int get_vartype(Handle, Atom *, 
		                std::vector<Handle> &,
		                VariableTypeMap &);
		Handle do_imply(Handle, PatternMatchCallback *, std::vector<Handle> *);
		Handle do_varscope(Handle, DefaultPatternMatchCB *);

	public:
		PatternMatch(void);
		void set_atomspace(AtomSpace *as)
		{
			atom_space = as;
			pme.set_atomspace(as);
		}

		void match(PatternMatchCallback *,
		           Handle vars,
		           Handle clauses,
		           Handle negations = Handle::UNDEFINED);

		Handle imply(Handle);             // deprecated
		Handle crisp_logic_imply(Handle); // deprecated
		Handle varscope(Handle);
};

} // namespace opencog

#endif // _OPENCOG_PATTERN_MATCH_H
