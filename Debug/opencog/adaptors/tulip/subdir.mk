################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/adaptors/tulip/TulipWriter.cc 

OBJS += \
./opencog/adaptors/tulip/TulipWriter.o 

CC_DEPS += \
./opencog/adaptors/tulip/TulipWriter.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/adaptors/tulip/%.o: ../opencog/adaptors/tulip/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


