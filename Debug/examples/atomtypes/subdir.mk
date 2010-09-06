################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../examples/atomtypes/CustomAtomTypesModule.cc \
../examples/atomtypes/CustomAtomTypesTester.cc 

OBJS += \
./examples/atomtypes/CustomAtomTypesModule.o \
./examples/atomtypes/CustomAtomTypesTester.o 

CC_DEPS += \
./examples/atomtypes/CustomAtomTypesModule.d \
./examples/atomtypes/CustomAtomTypesTester.d 


# Each subdirectory must supply rules for building sources it contributes
examples/atomtypes/%.o: ../examples/atomtypes/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


