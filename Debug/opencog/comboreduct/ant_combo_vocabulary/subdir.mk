################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/comboreduct/ant_combo_vocabulary/ant_action_symbol.cc \
../opencog/comboreduct/ant_combo_vocabulary/ant_builtin_action.cc \
../opencog/comboreduct/ant_combo_vocabulary/ant_combo_vocabulary.cc \
../opencog/comboreduct/ant_combo_vocabulary/ant_indefinite_object.cc \
../opencog/comboreduct/ant_combo_vocabulary/ant_perception.cc 

OBJS += \
./opencog/comboreduct/ant_combo_vocabulary/ant_action_symbol.o \
./opencog/comboreduct/ant_combo_vocabulary/ant_builtin_action.o \
./opencog/comboreduct/ant_combo_vocabulary/ant_combo_vocabulary.o \
./opencog/comboreduct/ant_combo_vocabulary/ant_indefinite_object.o \
./opencog/comboreduct/ant_combo_vocabulary/ant_perception.o 

CC_DEPS += \
./opencog/comboreduct/ant_combo_vocabulary/ant_action_symbol.d \
./opencog/comboreduct/ant_combo_vocabulary/ant_builtin_action.d \
./opencog/comboreduct/ant_combo_vocabulary/ant_combo_vocabulary.d \
./opencog/comboreduct/ant_combo_vocabulary/ant_indefinite_object.d \
./opencog/comboreduct/ant_combo_vocabulary/ant_perception.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/comboreduct/ant_combo_vocabulary/%.o: ../opencog/comboreduct/ant_combo_vocabulary/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


