################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/embodiment/Learning/FitnessEstimator/DistortedComboSize.cc \
../opencog/embodiment/Learning/FitnessEstimator/NoSpaceLifeFitnessEstimator.cc \
../opencog/embodiment/Learning/FitnessEstimator/SizePenalty.cc 

OBJS += \
./opencog/embodiment/Learning/FitnessEstimator/DistortedComboSize.o \
./opencog/embodiment/Learning/FitnessEstimator/NoSpaceLifeFitnessEstimator.o \
./opencog/embodiment/Learning/FitnessEstimator/SizePenalty.o 

CC_DEPS += \
./opencog/embodiment/Learning/FitnessEstimator/DistortedComboSize.d \
./opencog/embodiment/Learning/FitnessEstimator/NoSpaceLifeFitnessEstimator.d \
./opencog/embodiment/Learning/FitnessEstimator/SizePenalty.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/embodiment/Learning/FitnessEstimator/%.o: ../opencog/embodiment/Learning/FitnessEstimator/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


