################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/reasoning/pln/unused/utils/XMLNode.cc \
../opencog/reasoning/pln/unused/utils/XMLNodeLoader.cc 

OBJS += \
./opencog/reasoning/pln/unused/utils/XMLNode.o \
./opencog/reasoning/pln/unused/utils/XMLNodeLoader.o 

CC_DEPS += \
./opencog/reasoning/pln/unused/utils/XMLNode.d \
./opencog/reasoning/pln/unused/utils/XMLNodeLoader.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/reasoning/pln/unused/utils/%.o: ../opencog/reasoning/pln/unused/utils/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


