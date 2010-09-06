################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/reasoning/pln/unused/BackwardInferenceTask.cc \
../opencog/reasoning/pln/unused/PLNShell.cc \
../opencog/reasoning/pln/unused/Tester.cc \
../opencog/reasoning/pln/unused/TimeStamp.cc \
../opencog/reasoning/pln/unused/spacetime.cc 

OBJS += \
./opencog/reasoning/pln/unused/BackwardInferenceTask.o \
./opencog/reasoning/pln/unused/PLNShell.o \
./opencog/reasoning/pln/unused/Tester.o \
./opencog/reasoning/pln/unused/TimeStamp.o \
./opencog/reasoning/pln/unused/spacetime.o 

CC_DEPS += \
./opencog/reasoning/pln/unused/BackwardInferenceTask.d \
./opencog/reasoning/pln/unused/PLNShell.d \
./opencog/reasoning/pln/unused/Tester.d \
./opencog/reasoning/pln/unused/TimeStamp.d \
./opencog/reasoning/pln/unused/spacetime.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/reasoning/pln/unused/%.o: ../opencog/reasoning/pln/unused/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


