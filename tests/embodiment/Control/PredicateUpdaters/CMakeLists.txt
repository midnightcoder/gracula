ADD_CXXTEST(PredicatesUpdaterUTest)
TARGET_LINK_LIBRARIES(PredicatesUpdaterUTest 
        atomspace
        server
	PredicateUpdaters 
	PAI 
	${XERCESC_LIBRARIES}
	${PROJECT_LIBRARIES}
)
IF(WIN32)
TARGET_LINK_LIBRARIES(PredicatesUpdaterUTest 
        atomspace
        server
	PredicateUpdaters 
	PAI 
	winmm
	${XERCESC_LIBRARIES}
	${PROJECT_LIBRARIES}
)
ENDIF(WIN32)

