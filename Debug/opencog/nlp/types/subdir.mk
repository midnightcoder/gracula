################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/nlp/types/NLPTypes.cc 

OBJS += \
./opencog/nlp/types/NLPTypes.o 

CC_DEPS += \
./opencog/nlp/types/NLPTypes.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/nlp/types/%.o: ../opencog/nlp/types/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


