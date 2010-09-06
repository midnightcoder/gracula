################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/dotty/DottyModule.cc 

OBJS += \
./opencog/dotty/DottyModule.o 

CC_DEPS += \
./opencog/dotty/DottyModule.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/dotty/%.o: ../opencog/dotty/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


