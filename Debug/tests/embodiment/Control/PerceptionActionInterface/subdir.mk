################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../tests/embodiment/Control/PerceptionActionInterface/ProcessPVPMessage.cc 

OBJS += \
./tests/embodiment/Control/PerceptionActionInterface/ProcessPVPMessage.o 

CC_DEPS += \
./tests/embodiment/Control/PerceptionActionInterface/ProcessPVPMessage.d 


# Each subdirectory must supply rules for building sources it contributes
tests/embodiment/Control/PerceptionActionInterface/%.o: ../tests/embodiment/Control/PerceptionActionInterface/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


