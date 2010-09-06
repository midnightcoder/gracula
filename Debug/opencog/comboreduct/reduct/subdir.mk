################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/comboreduct/reduct/action_reduction.cc \
../opencog/comboreduct/reduct/action_rules.cc \
../opencog/comboreduct/reduct/ann_reduction.cc \
../opencog/comboreduct/reduct/clean_reduction.cc \
../opencog/comboreduct/reduct/contin_reduction.cc \
../opencog/comboreduct/reduct/contin_rules.cc \
../opencog/comboreduct/reduct/flat_normal_form.cc \
../opencog/comboreduct/reduct/full_reduction.cc \
../opencog/comboreduct/reduct/general_rules.cc \
../opencog/comboreduct/reduct/logical_reduction.cc \
../opencog/comboreduct/reduct/logical_rules.cc \
../opencog/comboreduct/reduct/meta_rules.cc \
../opencog/comboreduct/reduct/mixed_reduction.cc \
../opencog/comboreduct/reduct/mixed_rules.cc \
../opencog/comboreduct/reduct/perception_reduction.cc \
../opencog/comboreduct/reduct/perception_rules.cc \
../opencog/comboreduct/reduct/reduct.cc 

OBJS += \
./opencog/comboreduct/reduct/action_reduction.o \
./opencog/comboreduct/reduct/action_rules.o \
./opencog/comboreduct/reduct/ann_reduction.o \
./opencog/comboreduct/reduct/clean_reduction.o \
./opencog/comboreduct/reduct/contin_reduction.o \
./opencog/comboreduct/reduct/contin_rules.o \
./opencog/comboreduct/reduct/flat_normal_form.o \
./opencog/comboreduct/reduct/full_reduction.o \
./opencog/comboreduct/reduct/general_rules.o \
./opencog/comboreduct/reduct/logical_reduction.o \
./opencog/comboreduct/reduct/logical_rules.o \
./opencog/comboreduct/reduct/meta_rules.o \
./opencog/comboreduct/reduct/mixed_reduction.o \
./opencog/comboreduct/reduct/mixed_rules.o \
./opencog/comboreduct/reduct/perception_reduction.o \
./opencog/comboreduct/reduct/perception_rules.o \
./opencog/comboreduct/reduct/reduct.o 

CC_DEPS += \
./opencog/comboreduct/reduct/action_reduction.d \
./opencog/comboreduct/reduct/action_rules.d \
./opencog/comboreduct/reduct/ann_reduction.d \
./opencog/comboreduct/reduct/clean_reduction.d \
./opencog/comboreduct/reduct/contin_reduction.d \
./opencog/comboreduct/reduct/contin_rules.d \
./opencog/comboreduct/reduct/flat_normal_form.d \
./opencog/comboreduct/reduct/full_reduction.d \
./opencog/comboreduct/reduct/general_rules.d \
./opencog/comboreduct/reduct/logical_reduction.d \
./opencog/comboreduct/reduct/logical_rules.d \
./opencog/comboreduct/reduct/meta_rules.d \
./opencog/comboreduct/reduct/mixed_reduction.d \
./opencog/comboreduct/reduct/mixed_rules.d \
./opencog/comboreduct/reduct/perception_reduction.d \
./opencog/comboreduct/reduct/perception_rules.d \
./opencog/comboreduct/reduct/reduct.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/comboreduct/reduct/%.o: ../opencog/comboreduct/reduct/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


