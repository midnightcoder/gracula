################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/embodiment/Learning/PetaverseHC/HCPetaverseVocabularyProvider.cc \
../opencog/embodiment/Learning/PetaverseHC/petaverse-hillclimber.cc 

OBJS += \
./opencog/embodiment/Learning/PetaverseHC/HCPetaverseVocabularyProvider.o \
./opencog/embodiment/Learning/PetaverseHC/petaverse-hillclimber.o 

CC_DEPS += \
./opencog/embodiment/Learning/PetaverseHC/HCPetaverseVocabularyProvider.d \
./opencog/embodiment/Learning/PetaverseHC/petaverse-hillclimber.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/embodiment/Learning/PetaverseHC/%.o: ../opencog/embodiment/Learning/PetaverseHC/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


