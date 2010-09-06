################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/embodiment/RuleValidation/Processor/ProcessorExec.cc \
../opencog/embodiment/RuleValidation/Processor/RuleProcessor.cc 

OBJS += \
./opencog/embodiment/RuleValidation/Processor/ProcessorExec.o \
./opencog/embodiment/RuleValidation/Processor/RuleProcessor.o 

CC_DEPS += \
./opencog/embodiment/RuleValidation/Processor/ProcessorExec.d \
./opencog/embodiment/RuleValidation/Processor/RuleProcessor.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/embodiment/RuleValidation/Processor/%.o: ../opencog/embodiment/RuleValidation/Processor/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


