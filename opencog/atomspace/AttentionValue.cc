/*
 * opencog/atomspace/AttentionValue.cc
 *
 * Copyright (C) 2002-2007 Novamente LLC
 * All Rights Reserved
 *
 * Written by Tony Lofthouse <tony_lofthouse@btinternet.com>
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

#include "AttentionValue.h"

#include <math.h>

#include <opencog/atomspace/TLB.h>
#include <opencog/util/platform.h>

using namespace opencog;

AttentionValue::AttentionValue(sti_t STI, lti_t LTI, vlti_t VLTI)
{
    m_STI = STI;
    m_LTI = LTI;
    m_VLTI = VLTI;
}

AttentionValue::sti_t AttentionValue::getSTI() const
{
    return m_STI;
}

float AttentionValue::getScaledSTI() const
{
    return (((float) m_STI) + 32768) / 65534;
}

AttentionValue::lti_t AttentionValue::getLTI() const
{
    return m_LTI;
}

AttentionValue::vlti_t AttentionValue::getVLTI() const
{
    return m_VLTI;
}

void AttentionValue::decaySTI()
{
    if (m_STI != -32768)
        m_STI--;
}

std::string AttentionValue::toString() const
{
    char buffer[256];
    sprintf(buffer, "[%d, %d, %s]", (int)m_STI, (int)m_LTI, m_VLTI ? "NONDISPOSABLE" : "DISPOSABLE");
    return buffer;
}

AttentionValue* AttentionValue::clone() const
{
    return new AttentionValue(m_STI, m_LTI, m_VLTI);
}

bool AttentionValue::operator==(const AttentionValue& av) const
{
    return (m_STI == av.getSTI() && m_LTI == av.getLTI() && m_VLTI == av.getVLTI());
}

bool AttentionValue::STISort::operator()(const Handle& h1, const Handle& h2)
{
    return TLB::getAtom(h1)->getAttentionValue().getSTI() > TLB::getAtom(h2)->getAttentionValue().getSTI();
}

bool AttentionValue::LTIAndTVAscendingSort::operator()(const Handle& h1, const Handle& h2)
{
    lti_t lti1, lti2;
    float tv1, tv2;

    tv1 = fabs(TLB::getAtom(h1)->getTruthValue().getMean());
    tv2 = fabs(TLB::getAtom(h2)->getTruthValue().getMean());

    lti1 = TLB::getAtom(h1)->getAttentionValue().getLTI();
    lti2 = TLB::getAtom(h2)->getAttentionValue().getLTI();

    if (lti1 < 0)
        tv1 = lti1 * (1.0f - tv1);
    else
        tv1 = lti1 * tv1;

    if (lti2 < 0)
        tv2 = lti2 * (1.0f - tv2);
    else
        tv2 = lti2 * tv2;

    return tv1 < tv2;
}

bool AttentionValue::LTIThenTVAscendingSort::operator()(const Handle& h1, const Handle& h2)
{
    lti_t lti1, lti2;
    lti1 = TLB::getAtom(h1)->getAttentionValue().getLTI();
    lti2 = TLB::getAtom(h2)->getAttentionValue().getLTI();
    if (lti1 != lti2) return lti1 < lti2;

    float tv1, tv2;
    tv1 = TLB::getAtom(h1)->getTruthValue().getMean();
    tv2 = TLB::getAtom(h2)->getTruthValue().getMean();
    return tv1 < tv2;
}

AttentionValue* AttentionValue::m_defaultAV = NULL;

const AttentionValue& AttentionValue::getDefaultAV()
{
    if (!m_defaultAV)
        m_defaultAV = AttentionValue::factory();
    return *m_defaultAV;
}

AttentionValue* AttentionValue::factory()
{
    return new AttentionValue(DEFAULTATOMSTI, DEFAULTATOMLTI, DEFAULTATOMVLTI);
}

AttentionValue* AttentionValue::factory(AttentionValue::sti_t sti)
{
    return new AttentionValue(sti, DEFAULTATOMLTI, DEFAULTATOMVLTI);
}

AttentionValue* AttentionValue::factory(float scaledSti)
{

    AttentionValue::sti_t sti;

    if (scaledSti >= 1.0) {
        sti = 32767;
    } else if (scaledSti <= 0.0) {
        sti = -32768;
    } else {
        sti = (AttentionValue::sti_t) ((65534 * scaledSti) - 32767);
    }

    return new AttentionValue(sti, DEFAULTATOMLTI, DEFAULTATOMVLTI);
}

AttentionValue* AttentionValue::factory(AttentionValue::sti_t sti, AttentionValue::lti_t lti)
{
    return new AttentionValue(sti, lti, DEFAULTATOMVLTI);
}

AttentionValue* AttentionValue::factory(AttentionValue::sti_t sti, AttentionValue::lti_t lti, AttentionValue::vlti_t vlti)
{
    return new AttentionValue(sti, lti, vlti);
}

