################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/persist/hypertable/AtomspaceHTabler.cc \
../opencog/persist/hypertable/test.cc 

OBJS += \
./opencog/persist/hypertable/AtomspaceHTabler.o \
./opencog/persist/hypertable/test.o 

CC_DEPS += \
./opencog/persist/hypertable/AtomspaceHTabler.d \
./opencog/persist/hypertable/test.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/persist/hypertable/%.o: ../opencog/persist/hypertable/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


