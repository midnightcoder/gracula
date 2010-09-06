################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/python/atomspace/AtomSpace_wrap.cc \
../opencog/python/atomspace/AtomTable_wrap.cc \
../opencog/python/atomspace/Atom_wrap.cc \
../opencog/python/atomspace/AttentionValue_wrap.cc \
../opencog/python/atomspace/ClassServer_wrap.cc \
../opencog/python/atomspace/CompositeTruthValue_wrap.cc \
../opencog/python/atomspace/CountTruthValue_wrap.cc \
../opencog/python/atomspace/HandleEntry_wrap.cc \
../opencog/python/atomspace/Handle_wrap.cc \
../opencog/python/atomspace/IndefiniteTruthValue_wrap.cc \
../opencog/python/atomspace/Link_wrap.cc \
../opencog/python/atomspace/Node_wrap.cc \
../opencog/python/atomspace/SavableRepository_wrap.cc \
../opencog/python/atomspace/SimpleTruthValue_wrap.cc \
../opencog/python/atomspace/SpaceServerContainer_wrap.cc \
../opencog/python/atomspace/SpaceServer_wrap.cc \
../opencog/python/atomspace/TLB_wrap.cc \
../opencog/python/atomspace/TruthValue_wrap.cc \
../opencog/python/atomspace/atom_types_wrap.cc \
../opencog/python/atomspace/atomspace_module_wrap.cc 

OBJS += \
./opencog/python/atomspace/AtomSpace_wrap.o \
./opencog/python/atomspace/AtomTable_wrap.o \
./opencog/python/atomspace/Atom_wrap.o \
./opencog/python/atomspace/AttentionValue_wrap.o \
./opencog/python/atomspace/ClassServer_wrap.o \
./opencog/python/atomspace/CompositeTruthValue_wrap.o \
./opencog/python/atomspace/CountTruthValue_wrap.o \
./opencog/python/atomspace/HandleEntry_wrap.o \
./opencog/python/atomspace/Handle_wrap.o \
./opencog/python/atomspace/IndefiniteTruthValue_wrap.o \
./opencog/python/atomspace/Link_wrap.o \
./opencog/python/atomspace/Node_wrap.o \
./opencog/python/atomspace/SavableRepository_wrap.o \
./opencog/python/atomspace/SimpleTruthValue_wrap.o \
./opencog/python/atomspace/SpaceServerContainer_wrap.o \
./opencog/python/atomspace/SpaceServer_wrap.o \
./opencog/python/atomspace/TLB_wrap.o \
./opencog/python/atomspace/TruthValue_wrap.o \
./opencog/python/atomspace/atom_types_wrap.o \
./opencog/python/atomspace/atomspace_module_wrap.o 

CC_DEPS += \
./opencog/python/atomspace/AtomSpace_wrap.d \
./opencog/python/atomspace/AtomTable_wrap.d \
./opencog/python/atomspace/Atom_wrap.d \
./opencog/python/atomspace/AttentionValue_wrap.d \
./opencog/python/atomspace/ClassServer_wrap.d \
./opencog/python/atomspace/CompositeTruthValue_wrap.d \
./opencog/python/atomspace/CountTruthValue_wrap.d \
./opencog/python/atomspace/HandleEntry_wrap.d \
./opencog/python/atomspace/Handle_wrap.d \
./opencog/python/atomspace/IndefiniteTruthValue_wrap.d \
./opencog/python/atomspace/Link_wrap.d \
./opencog/python/atomspace/Node_wrap.d \
./opencog/python/atomspace/SavableRepository_wrap.d \
./opencog/python/atomspace/SimpleTruthValue_wrap.d \
./opencog/python/atomspace/SpaceServerContainer_wrap.d \
./opencog/python/atomspace/SpaceServer_wrap.d \
./opencog/python/atomspace/TLB_wrap.d \
./opencog/python/atomspace/TruthValue_wrap.d \
./opencog/python/atomspace/atom_types_wrap.d \
./opencog/python/atomspace/atomspace_module_wrap.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/python/atomspace/%.o: ../opencog/python/atomspace/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


