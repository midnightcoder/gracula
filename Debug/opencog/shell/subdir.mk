################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/shell/GenericShell.cc \
../opencog/shell/SchemeShell.cc \
../opencog/shell/SchemeShellModule.cc 

OBJS += \
./opencog/shell/GenericShell.o \
./opencog/shell/SchemeShell.o \
./opencog/shell/SchemeShellModule.o 

CC_DEPS += \
./opencog/shell/GenericShell.d \
./opencog/shell/SchemeShell.d \
./opencog/shell/SchemeShellModule.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/shell/%.o: ../opencog/shell/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


