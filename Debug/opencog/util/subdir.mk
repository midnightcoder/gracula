################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../opencog/util/getopt.c \
../opencog/util/getopt_long.c 

CC_SRCS += \
../opencog/util/Config.cc \
../opencog/util/FileList.cc \
../opencog/util/Logger.cc \
../opencog/util/StringManipulator.cc \
../opencog/util/StringTokenizer.cc \
../opencog/util/exceptions.cc \
../opencog/util/files.cc \
../opencog/util/lazy_normal_selector.cc \
../opencog/util/lazy_random_selector.cc \
../opencog/util/lazy_selector.cc \
../opencog/util/misc.cc \
../opencog/util/mt19937ar.cc \
../opencog/util/oc_assert.cc \
../opencog/util/platform.cc \
../opencog/util/strndup.cc \
../opencog/util/strnlen.cc \
../opencog/util/tree.cc 

OBJS += \
./opencog/util/Config.o \
./opencog/util/FileList.o \
./opencog/util/Logger.o \
./opencog/util/StringManipulator.o \
./opencog/util/StringTokenizer.o \
./opencog/util/exceptions.o \
./opencog/util/files.o \
./opencog/util/getopt.o \
./opencog/util/getopt_long.o \
./opencog/util/lazy_normal_selector.o \
./opencog/util/lazy_random_selector.o \
./opencog/util/lazy_selector.o \
./opencog/util/misc.o \
./opencog/util/mt19937ar.o \
./opencog/util/oc_assert.o \
./opencog/util/platform.o \
./opencog/util/strndup.o \
./opencog/util/strnlen.o \
./opencog/util/tree.o 

C_DEPS += \
./opencog/util/getopt.d \
./opencog/util/getopt_long.d 

CC_DEPS += \
./opencog/util/Config.d \
./opencog/util/FileList.d \
./opencog/util/Logger.d \
./opencog/util/StringManipulator.d \
./opencog/util/StringTokenizer.d \
./opencog/util/exceptions.d \
./opencog/util/files.d \
./opencog/util/lazy_normal_selector.d \
./opencog/util/lazy_random_selector.d \
./opencog/util/lazy_selector.d \
./opencog/util/misc.d \
./opencog/util/mt19937ar.d \
./opencog/util/oc_assert.d \
./opencog/util/platform.d \
./opencog/util/strndup.d \
./opencog/util/strnlen.d \
./opencog/util/tree.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/util/%.o: ../opencog/util/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

opencog/util/%.o: ../opencog/util/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


