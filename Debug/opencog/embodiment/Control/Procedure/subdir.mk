################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/embodiment/Control/Procedure/BuiltInProcedureRepository.cc \
../opencog/embodiment/Control/Procedure/ComboInterpreter.cc \
../opencog/embodiment/Control/Procedure/ComboPAIExecutable.cc \
../opencog/embodiment/Control/Procedure/ComboProcedure.cc \
../opencog/embodiment/Control/Procedure/ComboProcedureRepository.cc \
../opencog/embodiment/Control/Procedure/ComboSTDIOExecutable.cc \
../opencog/embodiment/Control/Procedure/ComboSelectInterpreter.cc \
../opencog/embodiment/Control/Procedure/ComboSelectProcedure.cc \
../opencog/embodiment/Control/Procedure/ComboSelectProcedureRepository.cc \
../opencog/embodiment/Control/Procedure/ComboShellServer.cc \
../opencog/embodiment/Control/Procedure/ProcedureInterpreter.cc \
../opencog/embodiment/Control/Procedure/ProcedureRepository.cc \
../opencog/embodiment/Control/Procedure/RunningBuiltInProcedure.cc \
../opencog/embodiment/Control/Procedure/RunningComboProcedure.cc \
../opencog/embodiment/Control/Procedure/RunningComboSelectProcedure.cc \
../opencog/embodiment/Control/Procedure/RunningProcedureId.cc 

OBJS += \
./opencog/embodiment/Control/Procedure/BuiltInProcedureRepository.o \
./opencog/embodiment/Control/Procedure/ComboInterpreter.o \
./opencog/embodiment/Control/Procedure/ComboPAIExecutable.o \
./opencog/embodiment/Control/Procedure/ComboProcedure.o \
./opencog/embodiment/Control/Procedure/ComboProcedureRepository.o \
./opencog/embodiment/Control/Procedure/ComboSTDIOExecutable.o \
./opencog/embodiment/Control/Procedure/ComboSelectInterpreter.o \
./opencog/embodiment/Control/Procedure/ComboSelectProcedure.o \
./opencog/embodiment/Control/Procedure/ComboSelectProcedureRepository.o \
./opencog/embodiment/Control/Procedure/ComboShellServer.o \
./opencog/embodiment/Control/Procedure/ProcedureInterpreter.o \
./opencog/embodiment/Control/Procedure/ProcedureRepository.o \
./opencog/embodiment/Control/Procedure/RunningBuiltInProcedure.o \
./opencog/embodiment/Control/Procedure/RunningComboProcedure.o \
./opencog/embodiment/Control/Procedure/RunningComboSelectProcedure.o \
./opencog/embodiment/Control/Procedure/RunningProcedureId.o 

CC_DEPS += \
./opencog/embodiment/Control/Procedure/BuiltInProcedureRepository.d \
./opencog/embodiment/Control/Procedure/ComboInterpreter.d \
./opencog/embodiment/Control/Procedure/ComboPAIExecutable.d \
./opencog/embodiment/Control/Procedure/ComboProcedure.d \
./opencog/embodiment/Control/Procedure/ComboProcedureRepository.d \
./opencog/embodiment/Control/Procedure/ComboSTDIOExecutable.d \
./opencog/embodiment/Control/Procedure/ComboSelectInterpreter.d \
./opencog/embodiment/Control/Procedure/ComboSelectProcedure.d \
./opencog/embodiment/Control/Procedure/ComboSelectProcedureRepository.d \
./opencog/embodiment/Control/Procedure/ComboShellServer.d \
./opencog/embodiment/Control/Procedure/ProcedureInterpreter.d \
./opencog/embodiment/Control/Procedure/ProcedureRepository.d \
./opencog/embodiment/Control/Procedure/RunningBuiltInProcedure.d \
./opencog/embodiment/Control/Procedure/RunningComboProcedure.d \
./opencog/embodiment/Control/Procedure/RunningComboSelectProcedure.d \
./opencog/embodiment/Control/Procedure/RunningProcedureId.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/embodiment/Control/Procedure/%.o: ../opencog/embodiment/Control/Procedure/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


