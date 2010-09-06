################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/nlp/question/FrameQuery.cc \
../opencog/nlp/question/SentenceQuery.cc \
../opencog/nlp/question/WordRelQuery.cc 

OBJS += \
./opencog/nlp/question/FrameQuery.o \
./opencog/nlp/question/SentenceQuery.o \
./opencog/nlp/question/WordRelQuery.o 

CC_DEPS += \
./opencog/nlp/question/FrameQuery.d \
./opencog/nlp/question/SentenceQuery.d \
./opencog/nlp/question/WordRelQuery.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/nlp/question/%.o: ../opencog/nlp/question/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


