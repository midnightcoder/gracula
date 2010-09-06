################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/embodiment/Learning/RewritingRules/hillclimbing_action_reduction.cc \
../opencog/embodiment/Learning/RewritingRules/hillclimbing_full_reduction.cc \
../opencog/embodiment/Learning/RewritingRules/hillclimbing_perception_reduction.cc \
../opencog/embodiment/Learning/RewritingRules/post_learning_rewriting.cc \
../opencog/embodiment/Learning/RewritingRules/post_learning_rules.cc 

OBJS += \
./opencog/embodiment/Learning/RewritingRules/hillclimbing_action_reduction.o \
./opencog/embodiment/Learning/RewritingRules/hillclimbing_full_reduction.o \
./opencog/embodiment/Learning/RewritingRules/hillclimbing_perception_reduction.o \
./opencog/embodiment/Learning/RewritingRules/post_learning_rewriting.o \
./opencog/embodiment/Learning/RewritingRules/post_learning_rules.o 

CC_DEPS += \
./opencog/embodiment/Learning/RewritingRules/hillclimbing_action_reduction.d \
./opencog/embodiment/Learning/RewritingRules/hillclimbing_full_reduction.d \
./opencog/embodiment/Learning/RewritingRules/hillclimbing_perception_reduction.d \
./opencog/embodiment/Learning/RewritingRules/post_learning_rewriting.d \
./opencog/embodiment/Learning/RewritingRules/post_learning_rules.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/embodiment/Learning/RewritingRules/%.o: ../opencog/embodiment/Learning/RewritingRules/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


