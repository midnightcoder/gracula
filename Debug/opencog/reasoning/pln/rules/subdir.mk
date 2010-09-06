################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/reasoning/pln/rules/Rule.cc \
../opencog/reasoning/pln/rules/RuleApp.cc \
../opencog/reasoning/pln/rules/RuleFunctions.cc \
../opencog/reasoning/pln/rules/RuleProvider.cc 

OBJS += \
./opencog/reasoning/pln/rules/Rule.o \
./opencog/reasoning/pln/rules/RuleApp.o \
./opencog/reasoning/pln/rules/RuleFunctions.o \
./opencog/reasoning/pln/rules/RuleProvider.o 

CC_DEPS += \
./opencog/reasoning/pln/rules/Rule.d \
./opencog/reasoning/pln/rules/RuleApp.d \
./opencog/reasoning/pln/rules/RuleFunctions.d \
./opencog/reasoning/pln/rules/RuleProvider.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/reasoning/pln/rules/%.o: ../opencog/reasoning/pln/rules/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


