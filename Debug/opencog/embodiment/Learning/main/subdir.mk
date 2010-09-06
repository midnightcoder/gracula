################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/embodiment/Learning/main/interactive-hillclimbing.cc \
../opencog/embodiment/Learning/main/moses-petbrain.cc \
../opencog/embodiment/Learning/main/post_learning-rewriter.cc 

OBJS += \
./opencog/embodiment/Learning/main/interactive-hillclimbing.o \
./opencog/embodiment/Learning/main/moses-petbrain.o \
./opencog/embodiment/Learning/main/post_learning-rewriter.o 

CC_DEPS += \
./opencog/embodiment/Learning/main/interactive-hillclimbing.d \
./opencog/embodiment/Learning/main/moses-petbrain.d \
./opencog/embodiment/Learning/main/post_learning-rewriter.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/embodiment/Learning/main/%.o: ../opencog/embodiment/Learning/main/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


