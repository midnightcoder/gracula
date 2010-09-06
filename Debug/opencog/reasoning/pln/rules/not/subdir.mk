################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/reasoning/pln/rules/not/NotEvaluatorRule.cc 

OBJS += \
./opencog/reasoning/pln/rules/not/NotEvaluatorRule.o 

CC_DEPS += \
./opencog/reasoning/pln/rules/not/NotEvaluatorRule.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/reasoning/pln/rules/not/%.o: ../opencog/reasoning/pln/rules/not/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


