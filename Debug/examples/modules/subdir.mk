################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../examples/modules/SampleAgent.cc \
../examples/modules/SingleAgentModule.cc 

OBJS += \
./examples/modules/SampleAgent.o \
./examples/modules/SingleAgentModule.o 

CC_DEPS += \
./examples/modules/SampleAgent.d \
./examples/modules/SingleAgentModule.d 


# Each subdirectory must supply rules for building sources it contributes
examples/modules/%.o: ../examples/modules/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


