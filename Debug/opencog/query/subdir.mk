################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/query/DefaultPatternMatchCB.cc \
../opencog/query/PatternMatch.cc \
../opencog/query/PatternMatchEngine.cc \
../opencog/query/QueryModule.cc 

OBJS += \
./opencog/query/DefaultPatternMatchCB.o \
./opencog/query/PatternMatch.o \
./opencog/query/PatternMatchEngine.o \
./opencog/query/QueryModule.o 

CC_DEPS += \
./opencog/query/DefaultPatternMatchCB.d \
./opencog/query/PatternMatch.d \
./opencog/query/PatternMatchEngine.d \
./opencog/query/QueryModule.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/query/%.o: ../opencog/query/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


