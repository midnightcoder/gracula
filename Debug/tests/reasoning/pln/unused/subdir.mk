################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../tests/reasoning/pln/unused/OldTester.cc 

OBJS += \
./tests/reasoning/pln/unused/OldTester.o 

CC_DEPS += \
./tests/reasoning/pln/unused/OldTester.d 


# Each subdirectory must supply rules for building sources it contributes
tests/reasoning/pln/unused/%.o: ../tests/reasoning/pln/unused/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


