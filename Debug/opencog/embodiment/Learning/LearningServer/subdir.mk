################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/embodiment/Learning/LearningServer/ImitationLearningAgent.cc \
../opencog/embodiment/Learning/LearningServer/LS.cc \
../opencog/embodiment/Learning/LearningServer/LSExecutable.cc \
../opencog/embodiment/Learning/LearningServer/LSMocky.cc \
../opencog/embodiment/Learning/LearningServer/LSMockyExec.cc \
../opencog/embodiment/Learning/LearningServer/SleepAgent.cc 

OBJS += \
./opencog/embodiment/Learning/LearningServer/ImitationLearningAgent.o \
./opencog/embodiment/Learning/LearningServer/LS.o \
./opencog/embodiment/Learning/LearningServer/LSExecutable.o \
./opencog/embodiment/Learning/LearningServer/LSMocky.o \
./opencog/embodiment/Learning/LearningServer/LSMockyExec.o \
./opencog/embodiment/Learning/LearningServer/SleepAgent.o 

CC_DEPS += \
./opencog/embodiment/Learning/LearningServer/ImitationLearningAgent.d \
./opencog/embodiment/Learning/LearningServer/LS.d \
./opencog/embodiment/Learning/LearningServer/LSExecutable.d \
./opencog/embodiment/Learning/LearningServer/LSMocky.d \
./opencog/embodiment/Learning/LearningServer/LSMockyExec.d \
./opencog/embodiment/Learning/LearningServer/SleepAgent.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/embodiment/Learning/LearningServer/%.o: ../opencog/embodiment/Learning/LearningServer/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


