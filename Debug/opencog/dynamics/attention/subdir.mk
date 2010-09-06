################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/dynamics/attention/AttentionModule.cc \
../opencog/dynamics/attention/ForgettingAgent.cc \
../opencog/dynamics/attention/HebbianUpdatingAgent.cc \
../opencog/dynamics/attention/ImportanceDiffusionAgent.cc \
../opencog/dynamics/attention/ImportanceSpreadingAgent.cc \
../opencog/dynamics/attention/ImportanceUpdatingAgent.cc \
../opencog/dynamics/attention/STIDecayingAgent.cc 

OBJS += \
./opencog/dynamics/attention/AttentionModule.o \
./opencog/dynamics/attention/ForgettingAgent.o \
./opencog/dynamics/attention/HebbianUpdatingAgent.o \
./opencog/dynamics/attention/ImportanceDiffusionAgent.o \
./opencog/dynamics/attention/ImportanceSpreadingAgent.o \
./opencog/dynamics/attention/ImportanceUpdatingAgent.o \
./opencog/dynamics/attention/STIDecayingAgent.o 

CC_DEPS += \
./opencog/dynamics/attention/AttentionModule.d \
./opencog/dynamics/attention/ForgettingAgent.d \
./opencog/dynamics/attention/HebbianUpdatingAgent.d \
./opencog/dynamics/attention/ImportanceDiffusionAgent.d \
./opencog/dynamics/attention/ImportanceSpreadingAgent.d \
./opencog/dynamics/attention/ImportanceUpdatingAgent.d \
./opencog/dynamics/attention/STIDecayingAgent.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/dynamics/attention/%.o: ../opencog/dynamics/attention/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


