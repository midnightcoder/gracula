################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/embodiment/Learning/behavior/BDRetriever.cc \
../opencog/embodiment/Learning/behavior/BDTracker.cc \
../opencog/embodiment/Learning/behavior/BE.cc \
../opencog/embodiment/Learning/behavior/BasicWorldProvider.cc \
../opencog/embodiment/Learning/behavior/BehaviorCategory.cc \
../opencog/embodiment/Learning/behavior/BehaviorDescriptionMatcher.cc \
../opencog/embodiment/Learning/behavior/CompositeBehaviorDescription.cc \
../opencog/embodiment/Learning/behavior/EvaluationLinkSimilarityEvaluator.cc \
../opencog/embodiment/Learning/behavior/PAIWorldProvider.cc \
../opencog/embodiment/Learning/behavior/PredicateHandleSet.cc 

OBJS += \
./opencog/embodiment/Learning/behavior/BDRetriever.o \
./opencog/embodiment/Learning/behavior/BDTracker.o \
./opencog/embodiment/Learning/behavior/BE.o \
./opencog/embodiment/Learning/behavior/BasicWorldProvider.o \
./opencog/embodiment/Learning/behavior/BehaviorCategory.o \
./opencog/embodiment/Learning/behavior/BehaviorDescriptionMatcher.o \
./opencog/embodiment/Learning/behavior/CompositeBehaviorDescription.o \
./opencog/embodiment/Learning/behavior/EvaluationLinkSimilarityEvaluator.o \
./opencog/embodiment/Learning/behavior/PAIWorldProvider.o \
./opencog/embodiment/Learning/behavior/PredicateHandleSet.o 

CC_DEPS += \
./opencog/embodiment/Learning/behavior/BDRetriever.d \
./opencog/embodiment/Learning/behavior/BDTracker.d \
./opencog/embodiment/Learning/behavior/BE.d \
./opencog/embodiment/Learning/behavior/BasicWorldProvider.d \
./opencog/embodiment/Learning/behavior/BehaviorCategory.d \
./opencog/embodiment/Learning/behavior/BehaviorDescriptionMatcher.d \
./opencog/embodiment/Learning/behavior/CompositeBehaviorDescription.d \
./opencog/embodiment/Learning/behavior/EvaluationLinkSimilarityEvaluator.d \
./opencog/embodiment/Learning/behavior/PAIWorldProvider.d \
./opencog/embodiment/Learning/behavior/PredicateHandleSet.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/embodiment/Learning/behavior/%.o: ../opencog/embodiment/Learning/behavior/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


