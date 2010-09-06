################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/embodiment/Learning/LearningServerMessages/LSCmdMessage.cc \
../opencog/embodiment/Learning/LearningServerMessages/LearnMessage.cc \
../opencog/embodiment/Learning/LearningServerMessages/RewardMessage.cc \
../opencog/embodiment/Learning/LearningServerMessages/SchemaMessage.cc \
../opencog/embodiment/Learning/LearningServerMessages/StopLearningMessage.cc \
../opencog/embodiment/Learning/LearningServerMessages/TrySchemaMessage.cc 

OBJS += \
./opencog/embodiment/Learning/LearningServerMessages/LSCmdMessage.o \
./opencog/embodiment/Learning/LearningServerMessages/LearnMessage.o \
./opencog/embodiment/Learning/LearningServerMessages/RewardMessage.o \
./opencog/embodiment/Learning/LearningServerMessages/SchemaMessage.o \
./opencog/embodiment/Learning/LearningServerMessages/StopLearningMessage.o \
./opencog/embodiment/Learning/LearningServerMessages/TrySchemaMessage.o 

CC_DEPS += \
./opencog/embodiment/Learning/LearningServerMessages/LSCmdMessage.d \
./opencog/embodiment/Learning/LearningServerMessages/LearnMessage.d \
./opencog/embodiment/Learning/LearningServerMessages/RewardMessage.d \
./opencog/embodiment/Learning/LearningServerMessages/SchemaMessage.d \
./opencog/embodiment/Learning/LearningServerMessages/StopLearningMessage.d \
./opencog/embodiment/Learning/LearningServerMessages/TrySchemaMessage.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/embodiment/Learning/LearningServerMessages/%.o: ../opencog/embodiment/Learning/LearningServerMessages/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


