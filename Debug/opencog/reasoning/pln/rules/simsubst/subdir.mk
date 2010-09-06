################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/reasoning/pln/rules/simsubst/SimSubstRule1.cc \
../opencog/reasoning/pln/rules/simsubst/SimSubstRule2.cc 

OBJS += \
./opencog/reasoning/pln/rules/simsubst/SimSubstRule1.o \
./opencog/reasoning/pln/rules/simsubst/SimSubstRule2.o 

CC_DEPS += \
./opencog/reasoning/pln/rules/simsubst/SimSubstRule1.d \
./opencog/reasoning/pln/rules/simsubst/SimSubstRule2.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/reasoning/pln/rules/simsubst/%.o: ../opencog/reasoning/pln/rules/simsubst/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


