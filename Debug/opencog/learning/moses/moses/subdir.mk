################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/learning/moses/moses/build_knobs.cc \
../opencog/learning/moses/moses/complexity.cc \
../opencog/learning/moses/moses/pole_balancing.cc \
../opencog/learning/moses/moses/representation.cc \
../opencog/learning/moses/moses/scoring.cc \
../opencog/learning/moses/moses/types.cc 

OBJS += \
./opencog/learning/moses/moses/build_knobs.o \
./opencog/learning/moses/moses/complexity.o \
./opencog/learning/moses/moses/pole_balancing.o \
./opencog/learning/moses/moses/representation.o \
./opencog/learning/moses/moses/scoring.o \
./opencog/learning/moses/moses/types.o 

CC_DEPS += \
./opencog/learning/moses/moses/build_knobs.d \
./opencog/learning/moses/moses/complexity.d \
./opencog/learning/moses/moses/pole_balancing.d \
./opencog/learning/moses/moses/representation.d \
./opencog/learning/moses/moses/scoring.d \
./opencog/learning/moses/moses/types.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/learning/moses/moses/%.o: ../opencog/learning/moses/moses/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


