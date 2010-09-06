################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../opencog/nlp/wordnet-import/wni.c 

OBJS += \
./opencog/nlp/wordnet-import/wni.o 

C_DEPS += \
./opencog/nlp/wordnet-import/wni.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/nlp/wordnet-import/%.o: ../opencog/nlp/wordnet-import/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


