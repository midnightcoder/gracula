################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../examples/hopfield/HopfieldDemo.cc \
../examples/hopfield/HopfieldOptions.cc \
../examples/hopfield/HopfieldServer.cc \
../examples/hopfield/HopfieldUbigrapher.cc \
../examples/hopfield/ImprintAgent.cc \
../examples/hopfield/Pattern.cc \
../examples/hopfield/StorkeyAgent.cc 

OBJS += \
./examples/hopfield/HopfieldDemo.o \
./examples/hopfield/HopfieldOptions.o \
./examples/hopfield/HopfieldServer.o \
./examples/hopfield/HopfieldUbigrapher.o \
./examples/hopfield/ImprintAgent.o \
./examples/hopfield/Pattern.o \
./examples/hopfield/StorkeyAgent.o 

CC_DEPS += \
./examples/hopfield/HopfieldDemo.d \
./examples/hopfield/HopfieldOptions.d \
./examples/hopfield/HopfieldServer.d \
./examples/hopfield/HopfieldUbigrapher.d \
./examples/hopfield/ImprintAgent.d \
./examples/hopfield/Pattern.d \
./examples/hopfield/StorkeyAgent.d 


# Each subdirectory must supply rules for building sources it contributes
examples/hopfield/%.o: ../examples/hopfield/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


