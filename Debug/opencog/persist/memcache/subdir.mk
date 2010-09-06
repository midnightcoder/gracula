################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/persist/memcache/AtomCache.cc \
../opencog/persist/memcache/sniff.cc \
../opencog/persist/memcache/sniff2.cc 

OBJS += \
./opencog/persist/memcache/AtomCache.o \
./opencog/persist/memcache/sniff.o \
./opencog/persist/memcache/sniff2.o 

CC_DEPS += \
./opencog/persist/memcache/AtomCache.d \
./opencog/persist/memcache/sniff.d \
./opencog/persist/memcache/sniff2.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/persist/memcache/%.o: ../opencog/persist/memcache/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


