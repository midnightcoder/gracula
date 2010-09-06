/*
 * Copyright (C) 2002-2007 Novamente LLC
 * Copyright (C) 2008 by Singularity Institute for Artificial Intelligence
 * All Rights Reserved
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

#ifndef _PLNSHELL_H
#define _PLNSHELL_H

#include "utils/Singleton.h"

#ifdef __cplusplus
extern "C"
{
#endif

class PLNShell : public Singleton<PLNShell>
{
	void Init();
public:
	void Launch(vtree *target);
	void Launch();
};

#define ThePLNShell PLNShell::Instance()

#define LOG_ON_FILE 0
 
//extern bool RunPLNtest;

#ifdef __cplusplus
}
#endif

#endif /* _PLNSHELL_H */
