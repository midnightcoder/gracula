################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/reasoning/pln/rules/implication/ImplicationBreakdownRule.cc \
../opencog/reasoning/pln/rules/implication/ImplicationConstructionRule.cc \
../opencog/reasoning/pln/rules/implication/ImplicationRedundantExpansionRule.cc \
../opencog/reasoning/pln/rules/implication/ImplicationTailExpansionRule.cc \
../opencog/reasoning/pln/rules/implication/StrictImplicationBreakdownRule.cc 

OBJS += \
./opencog/reasoning/pln/rules/implication/ImplicationBreakdownRule.o \
./opencog/reasoning/pln/rules/implication/ImplicationConstructionRule.o \
./opencog/reasoning/pln/rules/implication/ImplicationRedundantExpansionRule.o \
./opencog/reasoning/pln/rules/implication/ImplicationTailExpansionRule.o \
./opencog/reasoning/pln/rules/implication/StrictImplicationBreakdownRule.o 

CC_DEPS += \
./opencog/reasoning/pln/rules/implication/ImplicationBreakdownRule.d \
./opencog/reasoning/pln/rules/implication/ImplicationConstructionRule.d \
./opencog/reasoning/pln/rules/implication/ImplicationRedundantExpansionRule.d \
./opencog/reasoning/pln/rules/implication/ImplicationTailExpansionRule.d \
./opencog/reasoning/pln/rules/implication/StrictImplicationBreakdownRule.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/reasoning/pln/rules/implication/%.o: ../opencog/reasoning/pln/rules/implication/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


