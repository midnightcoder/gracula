################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/nlp/chatbot/CogitaConfig.cc \
../opencog/nlp/chatbot/IRC.cc \
../opencog/nlp/chatbot/go-irc.cc \
../opencog/nlp/chatbot/whirr-sockets.cc 

OBJS += \
./opencog/nlp/chatbot/CogitaConfig.o \
./opencog/nlp/chatbot/IRC.o \
./opencog/nlp/chatbot/go-irc.o \
./opencog/nlp/chatbot/whirr-sockets.o 

CC_DEPS += \
./opencog/nlp/chatbot/CogitaConfig.d \
./opencog/nlp/chatbot/IRC.d \
./opencog/nlp/chatbot/go-irc.d \
./opencog/nlp/chatbot/whirr-sockets.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/nlp/chatbot/%.o: ../opencog/nlp/chatbot/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


