################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/persist/file/CompositeRenumber.cc \
../opencog/persist/file/CoreUtils.cc \
../opencog/persist/file/SavingLoading.cc \
../opencog/persist/file/SpaceServerSavable.cc \
../opencog/persist/file/TemporalTableFile.cc \
../opencog/persist/file/TimeServerSavable.cc 

OBJS += \
./opencog/persist/file/CompositeRenumber.o \
./opencog/persist/file/CoreUtils.o \
./opencog/persist/file/SavingLoading.o \
./opencog/persist/file/SpaceServerSavable.o \
./opencog/persist/file/TemporalTableFile.o \
./opencog/persist/file/TimeServerSavable.o 

CC_DEPS += \
./opencog/persist/file/CompositeRenumber.d \
./opencog/persist/file/CoreUtils.d \
./opencog/persist/file/SavingLoading.d \
./opencog/persist/file/SpaceServerSavable.d \
./opencog/persist/file/TemporalTableFile.d \
./opencog/persist/file/TimeServerSavable.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/persist/file/%.o: ../opencog/persist/file/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


