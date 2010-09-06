################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/reasoning/pln/formulas/DeductionLookupTable.cc \
../opencog/reasoning/pln/formulas/Formulas.cc \
../opencog/reasoning/pln/formulas/FormulasIndefinite.cc 

OBJS += \
./opencog/reasoning/pln/formulas/DeductionLookupTable.o \
./opencog/reasoning/pln/formulas/Formulas.o \
./opencog/reasoning/pln/formulas/FormulasIndefinite.o 

CC_DEPS += \
./opencog/reasoning/pln/formulas/DeductionLookupTable.d \
./opencog/reasoning/pln/formulas/Formulas.d \
./opencog/reasoning/pln/formulas/FormulasIndefinite.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/reasoning/pln/formulas/%.o: ../opencog/reasoning/pln/formulas/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


