################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/embodiment/PetComboVocabulary/PetComboVocabulary.cc \
../opencog/embodiment/PetComboVocabulary/pet_action_symbol.cc \
../opencog/embodiment/PetComboVocabulary/pet_builtin_action.cc \
../opencog/embodiment/PetComboVocabulary/pet_indefinite_object.cc \
../opencog/embodiment/PetComboVocabulary/pet_perception.cc 

OBJS += \
./opencog/embodiment/PetComboVocabulary/PetComboVocabulary.o \
./opencog/embodiment/PetComboVocabulary/pet_action_symbol.o \
./opencog/embodiment/PetComboVocabulary/pet_builtin_action.o \
./opencog/embodiment/PetComboVocabulary/pet_indefinite_object.o \
./opencog/embodiment/PetComboVocabulary/pet_perception.o 

CC_DEPS += \
./opencog/embodiment/PetComboVocabulary/PetComboVocabulary.d \
./opencog/embodiment/PetComboVocabulary/pet_action_symbol.d \
./opencog/embodiment/PetComboVocabulary/pet_builtin_action.d \
./opencog/embodiment/PetComboVocabulary/pet_indefinite_object.d \
./opencog/embodiment/PetComboVocabulary/pet_perception.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/embodiment/PetComboVocabulary/%.o: ../opencog/embodiment/PetComboVocabulary/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


