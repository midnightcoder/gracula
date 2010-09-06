################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/embodiment/Learning/Filter/ActionFilter.cc \
../opencog/embodiment/Learning/Filter/EntityRelevanceFilter.cc \
../opencog/embodiment/Learning/Filter/EntropyFilter.cc 

OBJS += \
./opencog/embodiment/Learning/Filter/ActionFilter.o \
./opencog/embodiment/Learning/Filter/EntityRelevanceFilter.o \
./opencog/embodiment/Learning/Filter/EntropyFilter.o 

CC_DEPS += \
./opencog/embodiment/Learning/Filter/ActionFilter.d \
./opencog/embodiment/Learning/Filter/EntityRelevanceFilter.d \
./opencog/embodiment/Learning/Filter/EntropyFilter.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/embodiment/Learning/Filter/%.o: ../opencog/embodiment/Learning/Filter/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


