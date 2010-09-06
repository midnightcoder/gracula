################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/comboreduct/main/action-reductor.cc \
../opencog/comboreduct/main/contin-reductor.cc \
../opencog/comboreduct/main/full-reductor.cc \
../opencog/comboreduct/main/interactive-reductor.cc \
../opencog/comboreduct/main/logical-reductor.cc \
../opencog/comboreduct/main/mixed-reductor.cc \
../opencog/comboreduct/main/perception-reductor.cc \
../opencog/comboreduct/main/tree_gen.cc 

OBJS += \
./opencog/comboreduct/main/action-reductor.o \
./opencog/comboreduct/main/contin-reductor.o \
./opencog/comboreduct/main/full-reductor.o \
./opencog/comboreduct/main/interactive-reductor.o \
./opencog/comboreduct/main/logical-reductor.o \
./opencog/comboreduct/main/mixed-reductor.o \
./opencog/comboreduct/main/perception-reductor.o \
./opencog/comboreduct/main/tree_gen.o 

CC_DEPS += \
./opencog/comboreduct/main/action-reductor.d \
./opencog/comboreduct/main/contin-reductor.d \
./opencog/comboreduct/main/full-reductor.d \
./opencog/comboreduct/main/interactive-reductor.d \
./opencog/comboreduct/main/logical-reductor.d \
./opencog/comboreduct/main/mixed-reductor.d \
./opencog/comboreduct/main/perception-reductor.d \
./opencog/comboreduct/main/tree_gen.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/comboreduct/main/%.o: ../opencog/comboreduct/main/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


