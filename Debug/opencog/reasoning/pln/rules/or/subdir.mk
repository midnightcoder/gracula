################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/reasoning/pln/rules/or/ORPartitionRule.cc \
../opencog/reasoning/pln/rules/or/ORRule.cc 

OBJS += \
./opencog/reasoning/pln/rules/or/ORPartitionRule.o \
./opencog/reasoning/pln/rules/or/ORRule.o 

CC_DEPS += \
./opencog/reasoning/pln/rules/or/ORPartitionRule.d \
./opencog/reasoning/pln/rules/or/ORRule.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/reasoning/pln/rules/or/%.o: ../opencog/reasoning/pln/rules/or/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


