################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/embodiment/Control/Procedure/BuiltIn/PetActionSchema.cc 

OBJS += \
./opencog/embodiment/Control/Procedure/BuiltIn/PetActionSchema.o 

CC_DEPS += \
./opencog/embodiment/Control/Procedure/BuiltIn/PetActionSchema.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/embodiment/Control/Procedure/BuiltIn/%.o: ../opencog/embodiment/Control/Procedure/BuiltIn/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


