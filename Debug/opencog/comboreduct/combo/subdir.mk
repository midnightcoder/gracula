################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/comboreduct/combo/action.cc \
../opencog/comboreduct/combo/action_eval.cc \
../opencog/comboreduct/combo/action_symbol.cc \
../opencog/comboreduct/combo/assumption.cc \
../opencog/comboreduct/combo/builtin_action.cc \
../opencog/comboreduct/combo/definite_object.cc \
../opencog/comboreduct/combo/eval.cc \
../opencog/comboreduct/combo/indefinite_object.cc \
../opencog/comboreduct/combo/message.cc \
../opencog/comboreduct/combo/perception.cc \
../opencog/comboreduct/combo/procedure_call.cc \
../opencog/comboreduct/combo/procedure_repository.cc \
../opencog/comboreduct/combo/simple_nn.cc \
../opencog/comboreduct/combo/table.cc \
../opencog/comboreduct/combo/type_tree.cc \
../opencog/comboreduct/combo/type_tree_def.cc \
../opencog/comboreduct/combo/variable_unifier.cc \
../opencog/comboreduct/combo/vertex.cc 

OBJS += \
./opencog/comboreduct/combo/action.o \
./opencog/comboreduct/combo/action_eval.o \
./opencog/comboreduct/combo/action_symbol.o \
./opencog/comboreduct/combo/assumption.o \
./opencog/comboreduct/combo/builtin_action.o \
./opencog/comboreduct/combo/definite_object.o \
./opencog/comboreduct/combo/eval.o \
./opencog/comboreduct/combo/indefinite_object.o \
./opencog/comboreduct/combo/message.o \
./opencog/comboreduct/combo/perception.o \
./opencog/comboreduct/combo/procedure_call.o \
./opencog/comboreduct/combo/procedure_repository.o \
./opencog/comboreduct/combo/simple_nn.o \
./opencog/comboreduct/combo/table.o \
./opencog/comboreduct/combo/type_tree.o \
./opencog/comboreduct/combo/type_tree_def.o \
./opencog/comboreduct/combo/variable_unifier.o \
./opencog/comboreduct/combo/vertex.o 

CC_DEPS += \
./opencog/comboreduct/combo/action.d \
./opencog/comboreduct/combo/action_eval.d \
./opencog/comboreduct/combo/action_symbol.d \
./opencog/comboreduct/combo/assumption.d \
./opencog/comboreduct/combo/builtin_action.d \
./opencog/comboreduct/combo/definite_object.d \
./opencog/comboreduct/combo/eval.d \
./opencog/comboreduct/combo/indefinite_object.d \
./opencog/comboreduct/combo/message.d \
./opencog/comboreduct/combo/perception.d \
./opencog/comboreduct/combo/procedure_call.d \
./opencog/comboreduct/combo/procedure_repository.d \
./opencog/comboreduct/combo/simple_nn.d \
./opencog/comboreduct/combo/table.d \
./opencog/comboreduct/combo/type_tree.d \
./opencog/comboreduct/combo/type_tree_def.d \
./opencog/comboreduct/combo/variable_unifier.d \
./opencog/comboreduct/combo/vertex.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/comboreduct/combo/%.o: ../opencog/comboreduct/combo/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


