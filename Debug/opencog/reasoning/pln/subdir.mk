################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/reasoning/pln/ASSOC.cc \
../opencog/reasoning/pln/AtomSpaceWrapper.cc \
../opencog/reasoning/pln/BackChainingAgent.cc \
../opencog/reasoning/pln/BackInferenceTreeNode.cc \
../opencog/reasoning/pln/ForwardChainer.cc \
../opencog/reasoning/pln/ForwardChainingAgent.cc \
../opencog/reasoning/pln/PLNGlobals.cc \
../opencog/reasoning/pln/PLNModule.cc \
../opencog/reasoning/pln/PLNUtils.cc \
../opencog/reasoning/pln/PLNatom.cc \
../opencog/reasoning/pln/Testing.cc 

OBJS += \
./opencog/reasoning/pln/ASSOC.o \
./opencog/reasoning/pln/AtomSpaceWrapper.o \
./opencog/reasoning/pln/BackChainingAgent.o \
./opencog/reasoning/pln/BackInferenceTreeNode.o \
./opencog/reasoning/pln/ForwardChainer.o \
./opencog/reasoning/pln/ForwardChainingAgent.o \
./opencog/reasoning/pln/PLNGlobals.o \
./opencog/reasoning/pln/PLNModule.o \
./opencog/reasoning/pln/PLNUtils.o \
./opencog/reasoning/pln/PLNatom.o \
./opencog/reasoning/pln/Testing.o 

CC_DEPS += \
./opencog/reasoning/pln/ASSOC.d \
./opencog/reasoning/pln/AtomSpaceWrapper.d \
./opencog/reasoning/pln/BackChainingAgent.d \
./opencog/reasoning/pln/BackInferenceTreeNode.d \
./opencog/reasoning/pln/ForwardChainer.d \
./opencog/reasoning/pln/ForwardChainingAgent.d \
./opencog/reasoning/pln/PLNGlobals.d \
./opencog/reasoning/pln/PLNModule.d \
./opencog/reasoning/pln/PLNUtils.d \
./opencog/reasoning/pln/PLNatom.d \
./opencog/reasoning/pln/Testing.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/reasoning/pln/%.o: ../opencog/reasoning/pln/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


