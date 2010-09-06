################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/embodiment/Control/PredicateUpdaters/BasicPredicateUpdater.cc \
../opencog/embodiment/Control/PredicateUpdaters/IsMovablePredicateUpdater.cc \
../opencog/embodiment/Control/PredicateUpdaters/IsNoisyPredicateUpdater.cc \
../opencog/embodiment/Control/PredicateUpdaters/IsPeePlacePredicateUpdater.cc \
../opencog/embodiment/Control/PredicateUpdaters/IsPickupablePredicateUpdater.cc \
../opencog/embodiment/Control/PredicateUpdaters/IsPooPlacePredicateUpdater.cc \
../opencog/embodiment/Control/PredicateUpdaters/IsSmallPredicateUpdater.cc \
../opencog/embodiment/Control/PredicateUpdaters/NearPredicateUpdater.cc \
../opencog/embodiment/Control/PredicateUpdaters/PetPsychePredicatesUpdater.cc \
../opencog/embodiment/Control/PredicateUpdaters/PredicatesUpdater.cc 

OBJS += \
./opencog/embodiment/Control/PredicateUpdaters/BasicPredicateUpdater.o \
./opencog/embodiment/Control/PredicateUpdaters/IsMovablePredicateUpdater.o \
./opencog/embodiment/Control/PredicateUpdaters/IsNoisyPredicateUpdater.o \
./opencog/embodiment/Control/PredicateUpdaters/IsPeePlacePredicateUpdater.o \
./opencog/embodiment/Control/PredicateUpdaters/IsPickupablePredicateUpdater.o \
./opencog/embodiment/Control/PredicateUpdaters/IsPooPlacePredicateUpdater.o \
./opencog/embodiment/Control/PredicateUpdaters/IsSmallPredicateUpdater.o \
./opencog/embodiment/Control/PredicateUpdaters/NearPredicateUpdater.o \
./opencog/embodiment/Control/PredicateUpdaters/PetPsychePredicatesUpdater.o \
./opencog/embodiment/Control/PredicateUpdaters/PredicatesUpdater.o 

CC_DEPS += \
./opencog/embodiment/Control/PredicateUpdaters/BasicPredicateUpdater.d \
./opencog/embodiment/Control/PredicateUpdaters/IsMovablePredicateUpdater.d \
./opencog/embodiment/Control/PredicateUpdaters/IsNoisyPredicateUpdater.d \
./opencog/embodiment/Control/PredicateUpdaters/IsPeePlacePredicateUpdater.d \
./opencog/embodiment/Control/PredicateUpdaters/IsPickupablePredicateUpdater.d \
./opencog/embodiment/Control/PredicateUpdaters/IsPooPlacePredicateUpdater.d \
./opencog/embodiment/Control/PredicateUpdaters/IsSmallPredicateUpdater.d \
./opencog/embodiment/Control/PredicateUpdaters/NearPredicateUpdater.d \
./opencog/embodiment/Control/PredicateUpdaters/PetPsychePredicatesUpdater.d \
./opencog/embodiment/Control/PredicateUpdaters/PredicatesUpdater.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/embodiment/Control/PredicateUpdaters/%.o: ../opencog/embodiment/Control/PredicateUpdaters/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


