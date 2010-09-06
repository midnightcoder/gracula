/*
 * opencog/embodiment/Control/PerceptionActionInterface/PAIUtils.cc
 *
 * Copyright (C) 2002-2009 Novamente LLC
 * All Rights Reserved
 * Author(s): Welter Luigi
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


#include "PAIUtils.h"
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/util/PlatformUtils.hpp>

using namespace PerceptionActionInterface;

const int PAIUtils::MAX_TAG_LENGTH = 512;
const boost::posix_time::ptime PAIUtils::epoch( boost::gregorian::date( 2008, boost::gregorian::Jan, 1 ) );

void PAIUtils::initializeXMLPlatform() throw (opencog::XMLException, std::bad_exception)
{
    static bool initialized = false;

    // TODO: This is not multi-thread safe
    if (!initialized) {
        // Initialize the XML4C2 system
        try {
            //printf("Calling XMLPlatformUtils::Initialize()\n");
            XERCES_CPP_NAMESPACE::XMLPlatformUtils::Initialize();
        } catch (const XERCES_CPP_NAMESPACE::XMLException &toCatch) {
            throw opencog::XMLException(TRACE_INFO, "Error during Xerces-c initialization.");
        }
        initialized = true;
    }
}

void PAIUtils::terminateXMLPlatform()
{
    XERCES_CPP_NAMESPACE::XMLPlatformUtils::Terminate();
}

XERCES_CPP_NAMESPACE::DOMImplementation* PAIUtils::getDOMImplementation() throw (opencog::XMLException, std::bad_exception)
{

    initializeXMLPlatform();

    XMLCh* xmlVersion = XERCES_CPP_NAMESPACE::XMLString::transcode("XML 1.0");
    static XERCES_CPP_NAMESPACE::DOMImplementation* implementation =
        XERCES_CPP_NAMESPACE::DOMImplementationRegistry::getDOMImplementation(xmlVersion);
    XERCES_CPP_NAMESPACE::XMLString::release(&xmlVersion);
    if (implementation == NULL) {
        throw opencog::XMLException(TRACE_INFO, "DOM Implementation not supported.");
    }
    return implementation;
}

std::string PAIUtils::getSerializedXMLString(XERCES_CPP_NAMESPACE::DOMDocument * doc) throw (opencog::RuntimeException, std::bad_exception)
{
    std::string result;
    if (!doc) return result;
    XMLCh tag[PAIUtils::MAX_TAG_LENGTH+1];


    try {
        XERCES_CPP_NAMESPACE::XMLString::transcode("LS", tag, PAIUtils::MAX_TAG_LENGTH);
        XERCES_CPP_NAMESPACE::DOMImplementation * pImplement =
            XERCES_CPP_NAMESPACE::DOMImplementationRegistry::getDOMImplementation(tag);
        XERCES_CPP_NAMESPACE::DOMWriter * pSerializer = ((XERCES_CPP_NAMESPACE::DOMImplementationLS*)pImplement)->createDOMWriter();
        XERCES_CPP_NAMESPACE::XMLFormatTarget * formatTarget = new XERCES_CPP_NAMESPACE::MemBufFormatTarget();

        XERCES_CPP_NAMESPACE::XMLString::transcode("\n", tag, PAIUtils::MAX_TAG_LENGTH);
        pSerializer->setNewLine(tag);
        XERCES_CPP_NAMESPACE::XMLString::transcode("UTF-8", tag, PAIUtils::MAX_TAG_LENGTH);
        pSerializer->setEncoding(tag);

        if (pSerializer->canSetFeature(XERCES_CPP_NAMESPACE::XMLUni::fgDOMWRTFormatPrettyPrint, true))
            pSerializer->setFeature(XERCES_CPP_NAMESPACE::XMLUni::fgDOMWRTFormatPrettyPrint, true);

        pSerializer->writeNode(formatTarget, *doc);
        result = ((char *)((XERCES_CPP_NAMESPACE::MemBufFormatTarget *)formatTarget)->getRawBuffer());

        pSerializer->release();
        delete (formatTarget);
    } catch (const XERCES_CPP_NAMESPACE::OutOfMemoryException& toCatch) {
        throw opencog::RuntimeException(TRACE_INFO, "PAIUtils - Out of Memory Exception!");
    } catch (const XERCES_CPP_NAMESPACE::XMLException& toCatch) {
        char* message = XERCES_CPP_NAMESPACE::XMLString::transcode(toCatch.getMessage());
        throw opencog::RuntimeException(TRACE_INFO, "PAIUtils - Exception message is: %s.",  message);
        XERCES_CPP_NAMESPACE::XMLString::release(&message);
    }

    return result;
}

std::string PAIUtils::getInternalId(const char* externalId)
{
    std::string  result("id_");
    result += externalId;
    return result;
}

std::string PAIUtils::getExternalId(const char* internalId)
{
    std::string  result(internalId + 3);
    return result;
}

const boost::posix_time::ptime PAIUtils::getSystemEpoch( void )
{
    return epoch;
}

tm PAIUtils::getTimeInfo( unsigned long timestamp )
{

#ifdef DATETIME_DECIMAL_RESOLUTION
    unsigned long sec = ( timestamp / 10 ) * 10;
    unsigned long milli = timestamp - sec;
    sec /= 10;
#else
    unsigned long sec = ( timestamp / 100 ) * 100;
    unsigned long milli = timestamp - sec;
    sec /= 100;
#endif

    boost::posix_time::ptime today = getSystemEpoch( ) + ( boost::posix_time::seconds( sec ) + boost::posix_time::millisec( milli ) );

    return to_tm( today );
}

int PAIUtils::getTimeFactor( void )
{
#ifdef DATETIME_DECIMAL_RESOLUTION
    return 10;
#else
    return 100;
#endif
}
