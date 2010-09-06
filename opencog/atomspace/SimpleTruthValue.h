/*
 * opencog/atomspace/SimpleTruthValue.h
 *
 * Copyright (C) 2002-2007 Novamente LLC
 * All Rights Reserved
 *
 * Written by Guilherme Lamacie
 *            Murilo Queiroz <murilo@vettalabs.com>
 *            Welter Silva <welter@vettalabs.com>
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

#ifndef _OPENCOG_SIMPLE_TRUTH_VALUE_H_
#define _OPENCOG_SIMPLE_TRUTH_VALUE_H_

#include <opencog/atomspace/TruthValue.h>

namespace opencog
{

class SimpleTruthValue : public TruthValue
{

protected:

    strength_t mean;
    count_t count;

    void init(strength_t mean, count_t count);

public:

    SimpleTruthValue(strength_t mean, count_t count);
    SimpleTruthValue(const TruthValue&);
    SimpleTruthValue(SimpleTruthValue const&);

    SimpleTruthValue* clone() const;
    SimpleTruthValue& operator=(const TruthValue& rhs)
    throw (RuntimeException);

    virtual bool operator==(const TruthValue& rhs) const;

    static SimpleTruthValue* fromString(const char*);
    static count_t confidenceToCount(confidence_t);
    static confidence_t countToConfidence(count_t);

    float toFloat() const;
    std::string toString() const;
    TruthValueType getType() const;

    strength_t getMean() const;
    count_t getCount() const;
    confidence_t getConfidence() const;
    void setMean(strength_t);
    void setCount(count_t);
    void setConfidence(confidence_t);
};

} // namespace opencog

#endif // _OPENCOG_SIMPLE_TRUTH_VALUE_H_
