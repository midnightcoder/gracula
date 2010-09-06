################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/python/server/Agent_wrap.cc \
../opencog/python/server/BaseServer_wrap.cc \
../opencog/python/server/CogServer_wrap.cc \
../opencog/python/server/Factory_wrap.cc \
../opencog/python/server/Registry_wrap.cc \
../opencog/python/server/Request_wrap.cc \
../opencog/python/server/server_module_wrap.cc 

OBJS += \
./opencog/python/server/Agent_wrap.o \
./opencog/python/server/BaseServer_wrap.o \
./opencog/python/server/CogServer_wrap.o \
./opencog/python/server/Factory_wrap.o \
./opencog/python/server/Registry_wrap.o \
./opencog/python/server/Request_wrap.o \
./opencog/python/server/server_module_wrap.o 

CC_DEPS += \
./opencog/python/server/Agent_wrap.d \
./opencog/python/server/BaseServer_wrap.d \
./opencog/python/server/CogServer_wrap.d \
./opencog/python/server/Factory_wrap.d \
./opencog/python/server/Registry_wrap.d \
./opencog/python/server/Request_wrap.d \
./opencog/python/server/server_module_wrap.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/python/server/%.o: ../opencog/python/server/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


