################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/python/examples/hopfield/HopfieldOptions_wrap.cc \
../opencog/python/examples/hopfield/HopfieldServer_wrap.cc \
../opencog/python/examples/hopfield/hopfield_module_wrap.cc 

OBJS += \
./opencog/python/examples/hopfield/HopfieldOptions_wrap.o \
./opencog/python/examples/hopfield/HopfieldServer_wrap.o \
./opencog/python/examples/hopfield/hopfield_module_wrap.o 

CC_DEPS += \
./opencog/python/examples/hopfield/HopfieldOptions_wrap.d \
./opencog/python/examples/hopfield/HopfieldServer_wrap.d \
./opencog/python/examples/hopfield/hopfield_module_wrap.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/python/examples/hopfield/%.o: ../opencog/python/examples/hopfield/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


