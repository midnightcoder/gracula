################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/embodiment/RuleValidation/VirtualWorldData/VirtualEntity.cc \
../opencog/embodiment/RuleValidation/VirtualWorldData/VirtualWorldActions.cc \
../opencog/embodiment/RuleValidation/VirtualWorldData/VirtualWorldState.cc \
../opencog/embodiment/RuleValidation/VirtualWorldData/XmlErrorHandler.cc \
../opencog/embodiment/RuleValidation/VirtualWorldData/XmlLoader.cc 

OBJS += \
./opencog/embodiment/RuleValidation/VirtualWorldData/VirtualEntity.o \
./opencog/embodiment/RuleValidation/VirtualWorldData/VirtualWorldActions.o \
./opencog/embodiment/RuleValidation/VirtualWorldData/VirtualWorldState.o \
./opencog/embodiment/RuleValidation/VirtualWorldData/XmlErrorHandler.o \
./opencog/embodiment/RuleValidation/VirtualWorldData/XmlLoader.o 

CC_DEPS += \
./opencog/embodiment/RuleValidation/VirtualWorldData/VirtualEntity.d \
./opencog/embodiment/RuleValidation/VirtualWorldData/VirtualWorldActions.d \
./opencog/embodiment/RuleValidation/VirtualWorldData/VirtualWorldState.d \
./opencog/embodiment/RuleValidation/VirtualWorldData/XmlErrorHandler.d \
./opencog/embodiment/RuleValidation/VirtualWorldData/XmlLoader.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/embodiment/RuleValidation/VirtualWorldData/%.o: ../opencog/embodiment/RuleValidation/VirtualWorldData/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


