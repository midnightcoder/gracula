################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/reasoning/pln/utils/Grim.cc \
../opencog/reasoning/pln/utils/NMPrinter.cc \
../opencog/reasoning/pln/utils/mva.cc 

OBJS += \
./opencog/reasoning/pln/utils/Grim.o \
./opencog/reasoning/pln/utils/NMPrinter.o \
./opencog/reasoning/pln/utils/mva.o 

CC_DEPS += \
./opencog/reasoning/pln/utils/Grim.d \
./opencog/reasoning/pln/utils/NMPrinter.d \
./opencog/reasoning/pln/utils/mva.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/reasoning/pln/utils/%.o: ../opencog/reasoning/pln/utils/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


