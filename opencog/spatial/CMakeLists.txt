IF(CMAKE_COMPILER_IS_GNUCXX)
  # Best compromise between compile time and performance:
  # SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -finline-functions")

  # Fastest:
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g -fno-inline")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

ADD_LIBRARY(SpaceMap
    LocalSpaceMap2DUtil
    LocalSpaceMap2D
    VisibilityMap
    math/BoundingBox.cc  
    math/MathCommon.cc  
    math/Matrix4.cc  
    math/Quaternion.cc
    math/Face.cc         
    math/Matrix3.cc     
    math/Plane.cc    
    math/Rectangle.cc

    Entity.cc
    SuperEntity.cc
    MovableEntity.cc
    
    MapExplorerServer.cc
)

ADD_LIBRARY(HPASearch
    HPASearch
    QuadTree
)

ADD_LIBRARY(TangentBug
    TangentBugCommons
    TangentBug
)

IF(COMMAND cmake_policy)
      CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

ADD_EXECUTABLE (tangentbug_test TangentBugTestExec.cc)
TARGET_LINK_LIBRARIES(tangentbug_test
    SpaceMap
    TangentBug
    util
)

# Uncomment the following line to enable ncurses printing:
#SET(TB_PRINT_NCURSES 1)

IF(TB_PRINT_NCURSES)
  ADD_DEFINITIONS(-DTB_PRINT_NCURSES)
  TARGET_LINK_LIBRARIES(tangentbug ncurses)
ENDIF(TB_PRINT_NCURSES)

###################
# AStar Directives
###################

ADD_LIBRARY (AStar
    AStarController
    LSMap2DSearchNode
)

ADD_EXECUTABLE (astartest AStarTest)
TARGET_LINK_LIBRARIES(astartest
    SpaceMap
    TangentBug
    AStar
    util
)

IF(Boost_PROGRAM_OPTIONS_FOUND AND OPENGL_FOUND AND SDL_FOUND AND SDLGFX_FOUND)
ADD_EXECUTABLE(MapTools
    MapTools
    MapExplorerRenderer
    MapExplorerSystem
)
TARGET_LINK_LIBRARIES(MapTools
    SpaceMap
    util
    ${SDL_LIBRARY}
    ${SDLGFX_LIBRARY}
    ${OPENGL_LIBRARY}
    ${Boost_THREAD_LIBRARY} 
    ${Boost_PROGRAM_OPTIONS_LIBRARY} 
)
ENDIF(Boost_PROGRAM_OPTIONS_FOUND AND OPENGL_FOUND AND SDL_FOUND AND SDLGFX_FOUND)
