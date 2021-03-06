# Copyright (c) 2008, OpenCog.org (http://opencog.org)
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

# - Try to find the OPENCOG library; Once done this will define
#
# OPENCOG_FOUND            - system has the OPENCOG library
# OPENCOG_INCLUDE_DIRS     - the OPENCOG include directory
# OPENCOG_LIBRARIES        - The libraries needed to use OPENCOG
# OPENCOG_MODULES_DIR      - The directory with opencog's modules
#
# OPENCOG_<COMPONENT>_DIRS      - the include directory of component <COMPONENT>
# OPENCOG_<COMPONENT>_LIBRARIES - the libraries needed by component <COMPONENT>
#
# <COMPONENT> is always lowercase. For instance, 'atomtable', 'xml', 'server',
# 'scheme', etc.

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${OpenCog_DIR})

# first, check opencog dependencies
IF (WIN32)
	FIND_PACKAGE(PThreads REQUIRED)
	FIND_PACKAGE(STLPort REQUIRED)
	SET(OPENCOG_INCLUDE_DIR ${PTHREADS_INCLUDE_DIR} ${STLPORT_INCLUDE_DIR})
	SET(OPENCOG_LIBRARY ${PTHREADS_LIBRARY} ${STLPORT_LIBRARIES})
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
ELSE (WIN32)
	SET(OPENCOG_LIBRARY pthread)
ENDIF (WIN32)

FIND_PACKAGE(CSockets REQUIRED)
FIND_PACKAGE(OpenSSL REQUIRED)

SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)
FIND_PACKAGE(Boost COMPONENTS signals REQUIRED)

# FIND_PACKAGE(LibMemCached)
# Enable the use of SQL storage, if either iodbc of unixodbc is found.
# Caution: this can also increase RAM usage significantly!
FIND_PACKAGE(IODBC QUIET)
IF (IODBC_FOUND)
	ADD_DEFINITIONS(-DHAVE_SQL_STORAGE)
	SET(ODBC_FOUND 1)
	SET(ODBC_INCLUDE_DIRS ${IODBC_INCLUDE_DIRS})
	SET(ODBC_LIBRARIES ${IODBC_LIBRARIES})
ELSE (IODBC_FOUND)
	FIND_PACKAGE(UnixODBC QUIET)
	IF (UnixODBC_FOUND)
		ADD_DEFINITIONS(-DHAVE_SQL_STORAGE)
		SET(ODBC_FOUND 1)
		SET(ODBC_INCLUDE_DIRS ${UnixODBC_INCLUDE_DIRS})
		SET(ODBC_LIBRARIES ${UnixODBC_LIBRARIES})
	ELSE (UnixODBC_FOUND)
		SET(ODBC_FOUND 0)
		SET(ODBC_DIR_MESSAGE "Neither IODBC or UnixODBC was found. Make sure [Unix|I]ODBC_LIBRARIES and [Unix|I]ODBC_INCLUDE_DIRS are set.")
	ENDIF (UnixODBC_FOUND)
ENDIF (IODBC_FOUND)

FIND_PACKAGE(Guile 1.8.5)
IF (GUILE_FOUND)
	ADD_DEFINITIONS(-DHAVE_GUILE)
ENDIF (GUILE_FOUND)

# Gnu Scientific Library
FIND_PACKAGE(GSL)
IF (GSL_FOUND)
	ADD_DEFINITIONS(-DHAVE_GSL)
ENDIF (GSL_FOUND)

# XML parsing library
FIND_PACKAGE(EXPAT)
IF (EXPAT_FOUND)
	ADD_DEFINITIONS(-DHAVE_EXPAT)
ENDIF (EXPAT_FOUND)

# At this time,. there is no reason to build with memcached for 
# any "normal" use, so just stub it out.
# IF (LIBMEMCACHED_FOUND)
#	ADD_DEFINITIONS(-DHAVE_LIBMEMCACHED)
# ENDIF (LIBMEMCACHED_FOUND)


# set variables with the dependencies of each opencog component
SET(OPENCOG_BASE_INCLUDE_DIR            ${Boost_INCLUDE_DIRS})
SET(OPENCOG_ATOMSPACE_DEPS_LIBRARIES    ${Boost_SIGNALS_LIBRARY})
SET(OPENCOG_ATTENTION_DEPS_INCLUDE_DIRS ${GSL_INCLUDE_DIRS})
SET(OPENCOG_ATTENTION_DEPS_LIBRARIES    ${GSL_LIBRARIES})
SET(OPENCOG_PERSIST_DEPS_INCLUDE_DIRS   ${ODBC_INCLUDE_DIRS})
SET(OPENCOG_PERSIST_DEPS_LIBRARIES      ${ODBC_LIBRARIES})
SET(OPENCOG_SCHEME_DEPS_INCLUDE_DIRS    ${GUILE_INCLUDE_DIRS})
SET(OPENCOG_SCHEME_DEPS_LIBRARIES       ${GUILE_LIBRARIES})
SET(OPENCOG_SERVER_DEPS_INCLUDE_DIRS    ${CSOCKETS_INCLUDE_DIRS})
SET(OPENCOG_SERVER_DEPS_LIBRARIES       ${CSOCKETS_LIBRARIES} ${OPENSSL_LIBRARIES})
SET(OPENCOG_XML_DEPS_INCLUDE_DIRS       ${EXPAT_INCLUDE_DIRS})
SET(OPENCOG_XML_DEPS_LIBRARIES          ${EXPAT_LIBRARIES})

# then process the opencog library itself
GET_FILENAME_COMPONENT(OPENCOG_PREFIX ${OpenCog_DIR} ABSOLUTE)

MACRO(TEST_PREFIX _PREFIX _VAR)
    SET(${_VAR} 0)
    IF (EXISTS "${_PREFIX}/include" AND EXISTS "${_PREFIX}/lib")
		SET(${_VAR} 1)
    ENDIF (EXISTS "${_PREFIX}/include" AND EXISTS "${_PREFIX}/lib")
	IF (NOT ${${_VAR}} AND "${_PREFIX}" STREQUAL "/")
		SET(${_VAR} 1)
	ENDIF (NOT ${${_VAR}} AND "${_PREFIX}" STREQUAL "/")
ENDMACRO(TEST_PREFIX)

TEST_PREFIX(${OPENCOG_PREFIX} OPENCOG_PREFIX_FOUND)
WHILE (NOT ${OPENCOG_PREFIX_FOUND})
	GET_FILENAME_COMPONENT(OPENCOG_PREFIX "${OPENCOG_PREFIX}" PATH)
	TEST_PREFIX(${OPENCOG_PREFIX} OPENCOG_PREFIX_FOUND)
ENDWHILE (NOT ${OPENCOG_PREFIX_FOUND})

FIND_PATH(OPENCOG_INCLUDE_DIR opencog/atomspace/AtomSpace.h PATHS "${OPENCOG_PREFIX}/include" "$ENV{OPENCOG_PREFIX}/include")

IF ("${OpenCog_FIND_COMPONENTS}" STREQUAL "")
	SET(OpenCog_FIND_COMPONENTS atomspace)
ENDIF ("${OpenCog_FIND_COMPONENTS}" STREQUAL "")
FOREACH (COMPONENT ${OpenCog_FIND_COMPONENTS})
	STRING(TOUPPER "${COMPONENT}" UPPER_COMPONENT)
	FIND_LIBRARY(OPENCOG_${UPPER_COMPONENT}_LIBRARY NAMES "${COMPONENT}" PATH_SUFFIXES "opencog" PATHS "${OPENCOG_PREFIX}/lib" "$ENV{OPENCOG_PREFIX}/lib")
	IF (OPENCOG_${UPPER_COMPONENT}_LIBRARY)
		SET (OPENCOG_INCLUDE_DIR ${OPENCOG_INCLUDE_DIR} ${OPENCOG_${UPPER_COMPONENT}_DEPS_INCLUDE_DIR})
		SET (OPENCOG_LIBRARY ${OPENCOG_LIBRARY} ${OPENCOG_${UPPER_COMPONENT}_LIBRARY} ${OPENCOG_${UPPER_COMPONENT}_DEPS_LIBRARIES})
		SET (OPENCOG_MODULE_FILE ${OPENCOG_${UPPER_COMPONENT}_LIBRARY})
	ENDIF (OPENCOG_${UPPER_COMPONENT}_LIBRARY)
ENDFOREACH(COMPONENT)

IF (OPENCOG_INCLUDE_DIR AND OPENCOG_LIBRARY)
	SET (OPENCOG_FOUND 1)
	SET (OPENCOG_INCLUDE_DIRS ${OPENCOG_BASE_INCLUDE_DIR} ${OPENCOG_INCLUDE_DIR})
	SET (OPENCOG_LIBRARIES ${OPENCOG_LIBRARY})
	FOREACH (COMPONENT ${OpenCog_FIND_COMPONENTS})
		STRING(TOUPPER "${COMPONENT}" UPPER_COMPONENT)
		SET (OPENCOG_${UPPER_COMPONENT}_INCLUDE_DIRS ${OPENCOG_BASE_INCLUDE_DIR} ${OPENCOG_${UPPER_COMPONENT}_LIBRARY} ${OPENCOG_${UPPER_COMPONENT}_DEPS_LIBRARIES})
		SET (OPENCOG_${UPPER_COMPONENT}_LIBRARIES ${OPENCOG_${UPPER_COMPONENT}_LIBRARY} ${OPENCOG_${UPPER_COMPONENT}_DEPS_LIBRARIES})
	ENDFOREACH (COMPONENT)
	GET_FILENAME_COMPONENT(OPENCOG_MODULES_DIR ${OPENCOG_MODULE_FILE} PATH)
ELSE (OPENCOG_INCLUDE_DIR AND OPENCOG_LIBRARY)
	SET (OPENCOG_FOUND 0)
	SET (OPENCOG_PREFIX "OPENCOG_PREFIX-NOTFOUND" CACHE PATH "Opencog's base dir")
ENDIF (OPENCOG_INCLUDE_DIR AND OPENCOG_LIBRARY)

# define atom type script macro
MACRO(OPENCOG_ADD_ATOM_TYPES SCRIPT_FILE HEADER_FILE DEFINITIONS_FILE INHERITANCE_FILE)
    ADD_CUSTOM_COMMAND (
        COMMAND "${CMAKE_COMMAND}" -DSCRIPT_FILE=\"${CMAKE_CURRENT_SOURCE_DIR}/${SCRIPT_FILE}\" -DHEADER_FILE=\"${CMAKE_CURRENT_SOURCE_DIR}/${HEADER_FILE}\" -DDEFINITIONS_FILE=\"${CMAKE_CURRENT_SOURCE_DIR}/${DEFINITIONS_FILE}\" -DINHERITANCE_FILE=\"${CMAKE_CURRENT_SOURCE_DIR}/${INHERITANCE_FILE}\" -P "${OpenCog_DIR}/OpenCogAtomTypes.cmake"
        OUTPUT  "${CMAKE_CURRENT_SOURCE_DIR}/${HEADER_FILE}" "${CMAKE_CURRENT_SOURCE_DIR}/${DEFINITIONS_FILE}" "${CMAKE_CURRENT_SOURCE_DIR}/${INHERITANCE_FILE}"
        DEPENDS "${SCRIPT_FILE}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "Generating opencog types"
    )
ENDMACRO(OPENCOG_ADD_ATOM_TYPES SCRIPT_FILE HEADER_FILE DEFINITIONS_FILE INHERITANCE_FILE)

MARK_AS_ADVANCED (
	OPENCOG_INCLUDE_DIR
	OPENCOG_LIBRARY
	OPENCOG_PREFIX
	OPENCOG_BASE_INCLUDE_DIR
)
FOREACH (COMPONENT ${OpenCog_FIND_COMPONENTS})
	STRING(TOUPPER "${COMPONENT}" UPPER_COMPONENT)
	MARK_AS_ADVANCED(
		OPENCOG_${UPPER_COMPONENT}_INCLUDE_DIR
		OPENCOG_${UPPER_COMPONENT}_LIBRARY
	)
ENDFOREACH (COMPONENT ${OpenCog_FIND_COMPONENTS})

IF (NOT OPENCOG_FOUND)
	SET (OPENCOG_DIR_MESSAGE "OPENCOG was not found. Either set the \"OPENCOG_INCLUDE_DIR\" and \"OPENCOG_LIBRARIES\" cmake variables or set the \"OPENCOG_PREFIX\" variable to your opencog's base dir and rerun cmake.")
	IF (NOT OPENCOG_FIND_QUIETLY)
		IF (OPENCOG_FIND_REQUIRED)
			MESSAGE (FATAL_ERROR "[FATAL ERROR]: ${OPENCOG_DIR_MESSAGE}")
		ELSE (NOT OPENCOG_FIND_REQUIRED)
			MESSAGE (STATUS "[ERROR] ${OPENCOG_DIR_MESSAGE}")
		ENDIF (OPENCOG_FIND_REQUIRED)
	ENDIF (NOT OPENCOG_FIND_QUIETLY)
ENDIF (NOT OPENCOG_FOUND)
