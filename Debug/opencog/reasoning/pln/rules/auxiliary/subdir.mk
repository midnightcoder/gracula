################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/reasoning/pln/rules/auxiliary/CrispTheoremRule.cc \
../opencog/reasoning/pln/rules/auxiliary/IntensionalInheritanceRule.cc \
../opencog/reasoning/pln/rules/auxiliary/LookupRule.cc \
../opencog/reasoning/pln/rules/auxiliary/ScholemFunctionProductionRule.cc \
../opencog/reasoning/pln/rules/auxiliary/SubsetEvalRule.cc \
../opencog/reasoning/pln/rules/auxiliary/UnorderedLinkPermutationRule.cc 

OBJS += \
./opencog/reasoning/pln/rules/auxiliary/CrispTheoremRule.o \
./opencog/reasoning/pln/rules/auxiliary/IntensionalInheritanceRule.o \
./opencog/reasoning/pln/rules/auxiliary/LookupRule.o \
./opencog/reasoning/pln/rules/auxiliary/ScholemFunctionProductionRule.o \
./opencog/reasoning/pln/rules/auxiliary/SubsetEvalRule.o \
./opencog/reasoning/pln/rules/auxiliary/UnorderedLinkPermutationRule.o 

CC_DEPS += \
./opencog/reasoning/pln/rules/auxiliary/CrispTheoremRule.d \
./opencog/reasoning/pln/rules/auxiliary/IntensionalInheritanceRule.d \
./opencog/reasoning/pln/rules/auxiliary/LookupRule.d \
./opencog/reasoning/pln/rules/auxiliary/ScholemFunctionProductionRule.d \
./opencog/reasoning/pln/rules/auxiliary/SubsetEvalRule.d \
./opencog/reasoning/pln/rules/auxiliary/UnorderedLinkPermutationRule.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/reasoning/pln/rules/auxiliary/%.o: ../opencog/reasoning/pln/rules/auxiliary/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


