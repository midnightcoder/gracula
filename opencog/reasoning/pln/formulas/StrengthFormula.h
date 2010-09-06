/*
 * Copyright (C) 2002-2007 Novamente LLC
 * Copyright (C) 2008-2009 by Singularity Institute for Artificial Intelligence
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

#ifndef _STRENGTH_FORMULA
#define _STRENGTH_FORMULA

#include "TruthValue.h"

namespace opencog {
namespace pln {

// abstract class to be inherited by formula involving TV strength
class StrengthFormula {

public:

    // check formula precondition based on TV strength values
    // of the input atoms TV strengths
    virtual bool checkPrecondition(const strength_seq& ss) const = 0;

    // compute TruthValue strength based on the input atoms
    // TV strengths
    virtual strength_t computeStrength(const strength_seq& ss) const = 0;
};

}} // ~namespace opencog::pln

#endif
