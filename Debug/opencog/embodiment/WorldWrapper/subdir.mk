################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/embodiment/WorldWrapper/NoSpaceLifeWorldWrapper.cc \
../opencog/embodiment/WorldWrapper/PAIWorldWrapper.cc \
../opencog/embodiment/WorldWrapper/RuleValidationWorldWrapper.cc \
../opencog/embodiment/WorldWrapper/ShellWorldWrapper.cc \
../opencog/embodiment/WorldWrapper/WorldWrapper.cc \
../opencog/embodiment/WorldWrapper/WorldWrapperUtil.cc \
../opencog/embodiment/WorldWrapper/WorldWrapperUtilCache.cc \
../opencog/embodiment/WorldWrapper/WorldWrapperUtilMock.cc 

OBJS += \
./opencog/embodiment/WorldWrapper/NoSpaceLifeWorldWrapper.o \
./opencog/embodiment/WorldWrapper/PAIWorldWrapper.o \
./opencog/embodiment/WorldWrapper/RuleValidationWorldWrapper.o \
./opencog/embodiment/WorldWrapper/ShellWorldWrapper.o \
./opencog/embodiment/WorldWrapper/WorldWrapper.o \
./opencog/embodiment/WorldWrapper/WorldWrapperUtil.o \
./opencog/embodiment/WorldWrapper/WorldWrapperUtilCache.o \
./opencog/embodiment/WorldWrapper/WorldWrapperUtilMock.o 

CC_DEPS += \
./opencog/embodiment/WorldWrapper/NoSpaceLifeWorldWrapper.d \
./opencog/embodiment/WorldWrapper/PAIWorldWrapper.d \
./opencog/embodiment/WorldWrapper/RuleValidationWorldWrapper.d \
./opencog/embodiment/WorldWrapper/ShellWorldWrapper.d \
./opencog/embodiment/WorldWrapper/WorldWrapper.d \
./opencog/embodiment/WorldWrapper/WorldWrapperUtil.d \
./opencog/embodiment/WorldWrapper/WorldWrapperUtilCache.d \
./opencog/embodiment/WorldWrapper/WorldWrapperUtilMock.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/embodiment/WorldWrapper/%.o: ../opencog/embodiment/WorldWrapper/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


