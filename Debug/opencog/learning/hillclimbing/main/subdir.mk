################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/learning/hillclimbing/main/ant-hillclimbing.cc 

OBJS += \
./opencog/learning/hillclimbing/main/ant-hillclimbing.o 

CC_DEPS += \
./opencog/learning/hillclimbing/main/ant-hillclimbing.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/learning/hillclimbing/main/%.o: ../opencog/learning/hillclimbing/main/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


