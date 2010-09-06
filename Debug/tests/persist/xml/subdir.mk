################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../tests/persist/xml/NMXmlParserExperiment.cc 

OBJS += \
./tests/persist/xml/NMXmlParserExperiment.o 

CC_DEPS += \
./tests/persist/xml/NMXmlParserExperiment.d 


# Each subdirectory must supply rules for building sources it contributes
tests/persist/xml/%.o: ../tests/persist/xml/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


