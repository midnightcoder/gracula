################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/learning/moses/eda/field_set.cc \
../opencog/learning/moses/eda/initialization.cc \
../opencog/learning/moses/eda/local_structure.cc 

OBJS += \
./opencog/learning/moses/eda/field_set.o \
./opencog/learning/moses/eda/initialization.o \
./opencog/learning/moses/eda/local_structure.o 

CC_DEPS += \
./opencog/learning/moses/eda/field_set.d \
./opencog/learning/moses/eda/initialization.d \
./opencog/learning/moses/eda/local_structure.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/learning/moses/eda/%.o: ../opencog/learning/moses/eda/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


