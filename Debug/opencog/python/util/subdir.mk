################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/python/util/Logger_wrap.cc \
../opencog/python/util/util_module_wrap.cc 

OBJS += \
./opencog/python/util/Logger_wrap.o \
./opencog/python/util/util_module_wrap.o 

CC_DEPS += \
./opencog/python/util/Logger_wrap.d \
./opencog/python/util/util_module_wrap.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/python/util/%.o: ../opencog/python/util/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


