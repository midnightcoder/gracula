################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/spatial/AStarController.cc \
../opencog/spatial/AStarTest.cc \
../opencog/spatial/Entity.cc \
../opencog/spatial/HPASearch.cc \
../opencog/spatial/LSMap2DSearchNode.cc \
../opencog/spatial/LocalSpaceMap2D.cc \
../opencog/spatial/LocalSpaceMap2DUtil.cc \
../opencog/spatial/MapExplorerRenderer.cc \
../opencog/spatial/MapExplorerServer.cc \
../opencog/spatial/MapExplorerSystem.cc \
../opencog/spatial/MapTools.cc \
../opencog/spatial/MovableEntity.cc \
../opencog/spatial/QuadTree.cc \
../opencog/spatial/SuperEntity.cc \
../opencog/spatial/TangentBug.cc \
../opencog/spatial/TangentBugCommons.cc \
../opencog/spatial/TangentBugTestExec.cc \
../opencog/spatial/VisibilityMap.cc 

OBJS += \
./opencog/spatial/AStarController.o \
./opencog/spatial/AStarTest.o \
./opencog/spatial/Entity.o \
./opencog/spatial/HPASearch.o \
./opencog/spatial/LSMap2DSearchNode.o \
./opencog/spatial/LocalSpaceMap2D.o \
./opencog/spatial/LocalSpaceMap2DUtil.o \
./opencog/spatial/MapExplorerRenderer.o \
./opencog/spatial/MapExplorerServer.o \
./opencog/spatial/MapExplorerSystem.o \
./opencog/spatial/MapTools.o \
./opencog/spatial/MovableEntity.o \
./opencog/spatial/QuadTree.o \
./opencog/spatial/SuperEntity.o \
./opencog/spatial/TangentBug.o \
./opencog/spatial/TangentBugCommons.o \
./opencog/spatial/TangentBugTestExec.o \
./opencog/spatial/VisibilityMap.o 

CC_DEPS += \
./opencog/spatial/AStarController.d \
./opencog/spatial/AStarTest.d \
./opencog/spatial/Entity.d \
./opencog/spatial/HPASearch.d \
./opencog/spatial/LSMap2DSearchNode.d \
./opencog/spatial/LocalSpaceMap2D.d \
./opencog/spatial/LocalSpaceMap2DUtil.d \
./opencog/spatial/MapExplorerRenderer.d \
./opencog/spatial/MapExplorerServer.d \
./opencog/spatial/MapExplorerSystem.d \
./opencog/spatial/MapTools.d \
./opencog/spatial/MovableEntity.d \
./opencog/spatial/QuadTree.d \
./opencog/spatial/SuperEntity.d \
./opencog/spatial/TangentBug.d \
./opencog/spatial/TangentBugCommons.d \
./opencog/spatial/TangentBugTestExec.d \
./opencog/spatial/VisibilityMap.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/spatial/%.o: ../opencog/spatial/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


