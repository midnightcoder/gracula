/*
 * opencog/embodiment/RuleValidation/VirtualWorldData/XmlErrorHandler.cc
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

#include "XmlErrorHandler.h"
#include <opencog/util/Logger.h>

using namespace VirtualWorldData;
using namespace opencog;

void XmlErrorHandler::warning(const XERCES_CPP_NAMESPACE::SAXParseException& exc)
{
    char* errorMsg = XERCES_CPP_NAMESPACE::XMLString::transcode(exc.getMessage());
    char* publicIdStr = XERCES_CPP_NAMESPACE::XMLString::transcode(exc.getPublicId());
    char* systemIdStr = XERCES_CPP_NAMESPACE::XMLString::transcode(exc.getSystemId());
    logger().warn("XmlErrorHandler - Got a warning while parsing XML: %s\n"
                 "Line: %d\n"
                 "Column: %d\n"
                 "SystemId: %s\n"
                 , errorMsg
                 , exc.getLineNumber()
                 , exc.getColumnNumber()
                 , systemIdStr
                );
    XERCES_CPP_NAMESPACE::XMLString::release(&errorMsg);
    XERCES_CPP_NAMESPACE::XMLString::release(&publicIdStr);
    XERCES_CPP_NAMESPACE::XMLString::release(&systemIdStr);
}

void XmlErrorHandler::error(const XERCES_CPP_NAMESPACE::SAXParseException& exc)
{
    char* errorMsg = XERCES_CPP_NAMESPACE::XMLString::transcode(exc.getMessage());
    char* publicIdStr = XERCES_CPP_NAMESPACE::XMLString::transcode(exc.getPublicId());
    char* systemIdStr = XERCES_CPP_NAMESPACE::XMLString::transcode(exc.getSystemId());
    logger().error("XmlErrorHandler - Got an error while parsing XML: %s\n"
                 "Line: %d\n"
                 "Column: %d\n"
                 "SystemId: %s\n"
                 , errorMsg
                 , exc.getLineNumber()
                 , exc.getColumnNumber()
                 , systemIdStr
                );
    XERCES_CPP_NAMESPACE::XMLString::release(&errorMsg);
    XERCES_CPP_NAMESPACE::XMLString::release(&publicIdStr);
    XERCES_CPP_NAMESPACE::XMLString::release(&systemIdStr);
    throw exc;
}

void XmlErrorHandler::fatalError(const XERCES_CPP_NAMESPACE::SAXParseException& exc)
{
    char* errorMsg = XERCES_CPP_NAMESPACE::XMLString::transcode(exc.getMessage());
    char* publicIdStr = XERCES_CPP_NAMESPACE::XMLString::transcode(exc.getPublicId());
    char* systemIdStr = XERCES_CPP_NAMESPACE::XMLString::transcode(exc.getSystemId());
    logger().error("XmlErrorHandler - Got a fatal error while parsing XML: %s\n"
                 "Line: %d\n"
                 "Column: %d\n"
                 "SystemId: %s\n"
                 , errorMsg
                 , exc.getLineNumber()
                 , exc.getColumnNumber()
                 , systemIdStr
                );
    XERCES_CPP_NAMESPACE::XMLString::release(&errorMsg);
    XERCES_CPP_NAMESPACE::XMLString::release(&publicIdStr);
    XERCES_CPP_NAMESPACE::XMLString::release(&systemIdStr);
    throw exc;
}

void XmlErrorHandler::resetErrors()
{
    logger().info("XmlErrorHandler - resetErrors() called\n");
}

