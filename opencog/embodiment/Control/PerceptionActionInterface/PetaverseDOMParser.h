/*
 * opencog/embodiment/Control/PerceptionActionInterface/PetaverseDOMParser.h
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

#ifndef PETAVERSES_DOM_PARSERT_H_
#define PETAVERSES_DOM_PARSERT_H_
/**
 * This is an extension of XercesDOMParser to log the parse errors according with Petaverse log policy.
 */

#include <xercesc/parsers/XercesDOMParser.hpp>
#include "PAIUtils.h"

namespace PerceptionActionInterface
{

class PetaverseDOMParser : public XERCES_CPP_NAMESPACE::XercesDOMParser
{

public:
    PetaverseDOMParser() {
        PAIUtils::initializeXMLPlatform();
    }
    virtual ~PetaverseDOMParser() {};

    void error(const unsigned int    errCode,
               const XMLCh *const   errDomain,
               const ErrTypes  type,
               const XMLCh *const   errorText,
               const XMLCh *const   systemId,
               const XMLCh *const   publicId,
               const XMLSSize_t  lineNum,
               const XMLSSize_t  colNum);

};

}

#endif // PETAVERSES_DOM_PARSERT_H_
