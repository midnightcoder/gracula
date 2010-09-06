################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/embodiment/Learning/NoSpaceLife/NoSpaceLife.cc 

OBJS += \
./opencog/embodiment/Learning/NoSpaceLife/NoSpaceLife.o 

CC_DEPS += \
./opencog/embodiment/Learning/NoSpaceLife/NoSpaceLife.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/embodiment/Learning/NoSpaceLife/%.o: ../opencog/embodiment/Learning/NoSpaceLife/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


