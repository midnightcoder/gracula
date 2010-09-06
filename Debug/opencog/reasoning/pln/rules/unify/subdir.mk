################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/reasoning/pln/rules/unify/BaseCrispUnificationRule.cc \
../opencog/reasoning/pln/rules/unify/CrispUnificationRule.cc \
../opencog/reasoning/pln/rules/unify/CustomCrispUnificationRule.cc \
../opencog/reasoning/pln/rules/unify/CustomCrispUnificationRuleComposer.cc \
../opencog/reasoning/pln/rules/unify/StrictCrispUnificationRule.cc 

OBJS += \
./opencog/reasoning/pln/rules/unify/BaseCrispUnificationRule.o \
./opencog/reasoning/pln/rules/unify/CrispUnificationRule.o \
./opencog/reasoning/pln/rules/unify/CustomCrispUnificationRule.o \
./opencog/reasoning/pln/rules/unify/CustomCrispUnificationRuleComposer.o \
./opencog/reasoning/pln/rules/unify/StrictCrispUnificationRule.o 

CC_DEPS += \
./opencog/reasoning/pln/rules/unify/BaseCrispUnificationRule.d \
./opencog/reasoning/pln/rules/unify/CrispUnificationRule.d \
./opencog/reasoning/pln/rules/unify/CustomCrispUnificationRule.d \
./opencog/reasoning/pln/rules/unify/CustomCrispUnificationRuleComposer.d \
./opencog/reasoning/pln/rules/unify/StrictCrispUnificationRule.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/reasoning/pln/rules/unify/%.o: ../opencog/reasoning/pln/rules/unify/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


