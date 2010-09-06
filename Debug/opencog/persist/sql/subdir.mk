################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/persist/sql/AtomStorage.cc \
../opencog/persist/sql/PersistModule.cc \
../opencog/persist/sql/odbcxx.cc \
../opencog/persist/sql/sniff.cc 

OBJS += \
./opencog/persist/sql/AtomStorage.o \
./opencog/persist/sql/PersistModule.o \
./opencog/persist/sql/odbcxx.o \
./opencog/persist/sql/sniff.o 

CC_DEPS += \
./opencog/persist/sql/AtomStorage.d \
./opencog/persist/sql/PersistModule.d \
./opencog/persist/sql/odbcxx.d \
./opencog/persist/sql/sniff.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/persist/sql/%.o: ../opencog/persist/sql/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


