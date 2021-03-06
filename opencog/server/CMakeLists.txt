INCLUDE_DIRECTORIES (
	${GUILE_INCLUDE_DIRS}
)

ADD_LIBRARY (builtinreqs SHARED
	BuiltinRequestsModule
	Request
	DataRequest
	ExitRequest
	SleepRequest
	HelpRequest
	LoadRequest
	SaveRequest
	LoadModuleRequest
	ListRequest
	ShutdownRequest
	UnloadModuleRequest
)

IF (EXPAT_FOUND)
	SET(BUILTIN_LINK_LIBS xml)
ENDIF (EXPAT_FOUND)

IF (BUILTIN_LINK_LIBS)
    TARGET_LINK_LIBRARIES(builtinreqs
        ${BUILTIN_LINK_LIBS}
    )
ENDIF(BUILTIN_LINK_LIBS)

ADD_LIBRARY (server SHARED
	BaseServer
	CogServer
	Request
	NetworkServer
	ServerSocket
	ConsoleSocket
	Agent
	SystemActivityTable
	load-file
)

SET(SERVER_LINK_LIBS 
	atomspace
	${Boost_FILESYSTEM_LIBRARY}
	${Boost_THREAD_LIBRARY}
	dl
)

IF (EXPAT_FOUND)
	SET(SERVER_LINK_LIBS xml ${SERVER_LINK_LIBS})
ENDIF (EXPAT_FOUND)

IF (GUILE_FOUND)
	SET(SERVER_LINK_LIBS smob ${SERVER_LINK_LIBS})
ENDIF (GUILE_FOUND)

IF (ODBC_FOUND)
	SET(SERVER_LINK_LIBS persist ${SERVER_LINK_LIBS})
ENDIF (ODBC_FOUND)

TARGET_LINK_LIBRARIES(server ${SERVER_LINK_LIBS})

ADD_EXECUTABLE(cogserver
	CogServerMain
)

TARGET_LINK_LIBRARIES(cogserver
	server
	${Boost_FILESYSTEM_LIBRARY}
	#${Boost_SIGNALS_LIBRARY}
	${GUILE_LIBRARIES}
)

# to ease testing/debugging modules on the source tree
# IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
IF (APPLE)
    # We should eventually do Apple builds with "packages"
    # Apples's linker seems not to understand a colon-separated list of
    # rpaths.
    SET_TARGET_PROPERTIES(cogserver PROPERTIES LINK_FLAGS
        "-Wl,-rpath,${PROJECT_BINARY_DIR}/opencog/dotty -Wl,-rpath,${PROJECT_BINARY_DIR}/opencog/ubigraph -Wl,-rpath,${PROJECT_BINARY_DIR}/opencog/dynamics/attention -Wl,-rpath,${PROJECT_BINARY_DIR}/opencog/guile -Wl,-rpath,${PROJECT_BINARY_DIR}/opencog/reasoning/pln -Wl,-rpath,${PROJECT_BINARY_DIR}/opencog/nlp/wsd -Wl,-rpath,${PROJECT_BINARY_DIR}/opencog/persist/sql -Wl,-rpath,${PROJECT_BINARY_DIR}/opencog/query -Wl,-rpath,${PROJECT_BINARY_DIR}/opencog/server -Wl,-rpath,${PROJECT_BINARY_DIR}/opencog/shell -Wl,-rpath,${PROJECT_BINARY_DIR}/opencog/util -Wl,-rpath,${PROJECT_BINARY_DIR}/opencog/web")
    # doesn't work for some reason...
    #SET_TARGET_PROPERTIES(cogserver PROPERTIES INSTALL_RPATH
    #    ${PROJECT_BINARY_DIR}/opencog/guile)
    #SET_TARGET_PROPERTIES(cogserver PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
ELSE(APPLE)
    SET_TARGET_PROPERTIES(server PROPERTIES LINK_FLAGS
        -Wl,-rpath,${PROJECT_BINARY_DIR}/opencog/dotty:${PROJECT_BINARY_DIR}/opencog/ubigraph:${PROJECT_BINARY_DIR}/opencog/dynamics/attention:${PROJECT_BINARY_DIR}/opencog/guile:${PROJECT_BINARY_DIR}/opencog/reasoning/pln:${PROJECT_BINARY_DIR}/opencog/nlp/wsd:${PROJECT_BINARY_DIR}/opencog/persist:${PROJECT_BINARY_DIR}/opencog/query:${PROJECT_BINARY_DIR}/opencog/server:${PROJECT_BINARY_DIR}/opencog/shell:${PROJECT_BINARY_DIR}/opencog/util:${PROJECT_BINARY_DIR}/opencog/web)
ENDIF (APPLE)
#ENDIF (CMAKE_BUILD_TYPE STREQUAL "Debug")

SET_TARGET_PROPERTIES(cogserver PROPERTIES LINK_DIRECTORIES "${BOOST_LIBRARY_DIRS}")
IF (WIN32)
	SET_TARGET_PROPERTIES(cogserver PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libctm")
ENDIF (WIN32)

INSTALL (TARGETS server cogserver builtinreqs
	RUNTIME DESTINATION "bin"
	LIBRARY DESTINATION "lib${LIB_DIR_SUFFIX}/opencog"
)

INSTALL (FILES
	Agent.h
	BaseServer.h
	BuiltinRequestsModule.h
	CogServer.h
	ConsoleSocket.h
	DataRequest.h
	ExitRequest.h
	Factory.h
	HelpRequest.h
	IHasMimeType.h
	ListRequest.h
	load-file.h
	LoadModuleRequest.h
	LoadRequest.h
	SaveRequest.h
	Module.h
	NetworkServer.h
	SystemActivityTable.h
	Registry.h
	Request.h
	ShutdownRequest.h
	UnloadModuleRequest.h
	DESTINATION "include/${PROJECT_NAME}/server"
)
