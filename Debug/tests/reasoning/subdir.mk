################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../tests/reasoning/FormulaUTest.cc 

OBJS += \
./tests/reasoning/FormulaUTest.o 

CC_DEPS += \
./tests/reasoning/FormulaUTest.d 


# Each subdirectory must supply rules for building sources it contributes
tests/reasoning/%.o: ../tests/reasoning/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


