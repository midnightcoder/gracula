################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/reasoning/pln/rules/and/ANDBreakdownRule.cc \
../opencog/reasoning/pln/rules/and/ANDPartitionRule.cc \
../opencog/reasoning/pln/rules/and/ANDRule.cc \
../opencog/reasoning/pln/rules/and/ANDRuleArityFree.cc \
../opencog/reasoning/pln/rules/and/ANDSubstRule.cc 

OBJS += \
./opencog/reasoning/pln/rules/and/ANDBreakdownRule.o \
./opencog/reasoning/pln/rules/and/ANDPartitionRule.o \
./opencog/reasoning/pln/rules/and/ANDRule.o \
./opencog/reasoning/pln/rules/and/ANDRuleArityFree.o \
./opencog/reasoning/pln/rules/and/ANDSubstRule.o 

CC_DEPS += \
./opencog/reasoning/pln/rules/and/ANDBreakdownRule.d \
./opencog/reasoning/pln/rules/and/ANDPartitionRule.d \
./opencog/reasoning/pln/rules/and/ANDRule.d \
./opencog/reasoning/pln/rules/and/ANDRuleArityFree.d \
./opencog/reasoning/pln/rules/and/ANDSubstRule.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/reasoning/pln/rules/and/%.o: ../opencog/reasoning/pln/rules/and/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


