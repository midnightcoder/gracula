################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../examples/server/DerivedCogServer.cc \
../examples/server/DerivedMain.cc 

OBJS += \
./examples/server/DerivedCogServer.o \
./examples/server/DerivedMain.o 

CC_DEPS += \
./examples/server/DerivedCogServer.d \
./examples/server/DerivedMain.d 


# Each subdirectory must supply rules for building sources it contributes
examples/server/%.o: ../examples/server/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


