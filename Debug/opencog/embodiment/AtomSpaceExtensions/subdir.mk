################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/embodiment/AtomSpaceExtensions/AtomSpaceUtil.cc \
../opencog/embodiment/AtomSpaceExtensions/CompareAtomTreeTemplate.cc \
../opencog/embodiment/AtomSpaceExtensions/atom_types_init.cc 

OBJS += \
./opencog/embodiment/AtomSpaceExtensions/AtomSpaceUtil.o \
./opencog/embodiment/AtomSpaceExtensions/CompareAtomTreeTemplate.o \
./opencog/embodiment/AtomSpaceExtensions/atom_types_init.o 

CC_DEPS += \
./opencog/embodiment/AtomSpaceExtensions/AtomSpaceUtil.d \
./opencog/embodiment/AtomSpaceExtensions/CompareAtomTreeTemplate.d \
./opencog/embodiment/AtomSpaceExtensions/atom_types_init.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/embodiment/AtomSpaceExtensions/%.o: ../opencog/embodiment/AtomSpaceExtensions/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


