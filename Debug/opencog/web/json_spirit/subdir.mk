################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../opencog/web/json_spirit/json_spirit_reader.cpp \
../opencog/web/json_spirit/json_spirit_value.cpp \
../opencog/web/json_spirit/json_spirit_writer.cpp 

OBJS += \
./opencog/web/json_spirit/json_spirit_reader.o \
./opencog/web/json_spirit/json_spirit_value.o \
./opencog/web/json_spirit/json_spirit_writer.o 

CPP_DEPS += \
./opencog/web/json_spirit/json_spirit_reader.d \
./opencog/web/json_spirit/json_spirit_value.d \
./opencog/web/json_spirit/json_spirit_writer.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/web/json_spirit/%.o: ../opencog/web/json_spirit/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


