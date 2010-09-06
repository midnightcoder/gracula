################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/python/dynamics/dynamics_module_wrap.cc 

OBJS += \
./opencog/python/dynamics/dynamics_module_wrap.o 

CC_DEPS += \
./opencog/python/dynamics/dynamics_module_wrap.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/python/dynamics/%.o: ../opencog/python/dynamics/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


