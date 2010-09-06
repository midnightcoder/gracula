################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/python/examples/hello/hello.cc \
../opencog/python/examples/hello/hello_wrap.cc 

OBJS += \
./opencog/python/examples/hello/hello.o \
./opencog/python/examples/hello/hello_wrap.o 

CC_DEPS += \
./opencog/python/examples/hello/hello.d \
./opencog/python/examples/hello/hello_wrap.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/python/examples/hello/%.o: ../opencog/python/examples/hello/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


