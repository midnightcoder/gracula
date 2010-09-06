################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/embodiment/Control/PerceptionActionInterface/ActionParamType.cc \
../opencog/embodiment/Control/PerceptionActionInterface/ActionParameter.cc \
../opencog/embodiment/Control/PerceptionActionInterface/ActionPlan.cc \
../opencog/embodiment/Control/PerceptionActionInterface/ActionPlanDispatcher.cc \
../opencog/embodiment/Control/PerceptionActionInterface/ActionType.cc \
../opencog/embodiment/Control/PerceptionActionInterface/PAI.cc \
../opencog/embodiment/Control/PerceptionActionInterface/PAIUtils.cc \
../opencog/embodiment/Control/PerceptionActionInterface/PetAction.cc \
../opencog/embodiment/Control/PerceptionActionInterface/PetaverseDOMParser.cc \
../opencog/embodiment/Control/PerceptionActionInterface/PetaverseErrorHandler.cc 

OBJS += \
./opencog/embodiment/Control/PerceptionActionInterface/ActionParamType.o \
./opencog/embodiment/Control/PerceptionActionInterface/ActionParameter.o \
./opencog/embodiment/Control/PerceptionActionInterface/ActionPlan.o \
./opencog/embodiment/Control/PerceptionActionInterface/ActionPlanDispatcher.o \
./opencog/embodiment/Control/PerceptionActionInterface/ActionType.o \
./opencog/embodiment/Control/PerceptionActionInterface/PAI.o \
./opencog/embodiment/Control/PerceptionActionInterface/PAIUtils.o \
./opencog/embodiment/Control/PerceptionActionInterface/PetAction.o \
./opencog/embodiment/Control/PerceptionActionInterface/PetaverseDOMParser.o \
./opencog/embodiment/Control/PerceptionActionInterface/PetaverseErrorHandler.o 

CC_DEPS += \
./opencog/embodiment/Control/PerceptionActionInterface/ActionParamType.d \
./opencog/embodiment/Control/PerceptionActionInterface/ActionParameter.d \
./opencog/embodiment/Control/PerceptionActionInterface/ActionPlan.d \
./opencog/embodiment/Control/PerceptionActionInterface/ActionPlanDispatcher.d \
./opencog/embodiment/Control/PerceptionActionInterface/ActionType.d \
./opencog/embodiment/Control/PerceptionActionInterface/PAI.d \
./opencog/embodiment/Control/PerceptionActionInterface/PAIUtils.d \
./opencog/embodiment/Control/PerceptionActionInterface/PetAction.d \
./opencog/embodiment/Control/PerceptionActionInterface/PetaverseDOMParser.d \
./opencog/embodiment/Control/PerceptionActionInterface/PetaverseErrorHandler.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/embodiment/Control/PerceptionActionInterface/%.o: ../opencog/embodiment/Control/PerceptionActionInterface/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


