################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/embodiment/Control/MessagingSystem/EmbodimentCogServer.cc \
../opencog/embodiment/Control/MessagingSystem/FeedbackMessage.cc \
../opencog/embodiment/Control/MessagingSystem/FileMessageCentral.cc \
../opencog/embodiment/Control/MessagingSystem/MemoryMessageCentral.cc \
../opencog/embodiment/Control/MessagingSystem/Message.cc \
../opencog/embodiment/Control/MessagingSystem/MessageCentral.cc \
../opencog/embodiment/Control/MessagingSystem/MessageTesterExecutable.cc \
../opencog/embodiment/Control/MessagingSystem/MessagingSystemExceptions.cc \
../opencog/embodiment/Control/MessagingSystem/NetworkElement.cc \
../opencog/embodiment/Control/MessagingSystem/NetworkElementCommon.cc \
../opencog/embodiment/Control/MessagingSystem/Router.cc \
../opencog/embodiment/Control/MessagingSystem/RouterExecutable.cc \
../opencog/embodiment/Control/MessagingSystem/RouterMessage.cc \
../opencog/embodiment/Control/MessagingSystem/RouterServerSocket.cc \
../opencog/embodiment/Control/MessagingSystem/ServerSocket.cc \
../opencog/embodiment/Control/MessagingSystem/Spawner.cc \
../opencog/embodiment/Control/MessagingSystem/SpawnerExecutable.cc \
../opencog/embodiment/Control/MessagingSystem/StringMessage.cc \
../opencog/embodiment/Control/MessagingSystem/TickMessage.cc 

OBJS += \
./opencog/embodiment/Control/MessagingSystem/EmbodimentCogServer.o \
./opencog/embodiment/Control/MessagingSystem/FeedbackMessage.o \
./opencog/embodiment/Control/MessagingSystem/FileMessageCentral.o \
./opencog/embodiment/Control/MessagingSystem/MemoryMessageCentral.o \
./opencog/embodiment/Control/MessagingSystem/Message.o \
./opencog/embodiment/Control/MessagingSystem/MessageCentral.o \
./opencog/embodiment/Control/MessagingSystem/MessageTesterExecutable.o \
./opencog/embodiment/Control/MessagingSystem/MessagingSystemExceptions.o \
./opencog/embodiment/Control/MessagingSystem/NetworkElement.o \
./opencog/embodiment/Control/MessagingSystem/NetworkElementCommon.o \
./opencog/embodiment/Control/MessagingSystem/Router.o \
./opencog/embodiment/Control/MessagingSystem/RouterExecutable.o \
./opencog/embodiment/Control/MessagingSystem/RouterMessage.o \
./opencog/embodiment/Control/MessagingSystem/RouterServerSocket.o \
./opencog/embodiment/Control/MessagingSystem/ServerSocket.o \
./opencog/embodiment/Control/MessagingSystem/Spawner.o \
./opencog/embodiment/Control/MessagingSystem/SpawnerExecutable.o \
./opencog/embodiment/Control/MessagingSystem/StringMessage.o \
./opencog/embodiment/Control/MessagingSystem/TickMessage.o 

CC_DEPS += \
./opencog/embodiment/Control/MessagingSystem/EmbodimentCogServer.d \
./opencog/embodiment/Control/MessagingSystem/FeedbackMessage.d \
./opencog/embodiment/Control/MessagingSystem/FileMessageCentral.d \
./opencog/embodiment/Control/MessagingSystem/MemoryMessageCentral.d \
./opencog/embodiment/Control/MessagingSystem/Message.d \
./opencog/embodiment/Control/MessagingSystem/MessageCentral.d \
./opencog/embodiment/Control/MessagingSystem/MessageTesterExecutable.d \
./opencog/embodiment/Control/MessagingSystem/MessagingSystemExceptions.d \
./opencog/embodiment/Control/MessagingSystem/NetworkElement.d \
./opencog/embodiment/Control/MessagingSystem/NetworkElementCommon.d \
./opencog/embodiment/Control/MessagingSystem/Router.d \
./opencog/embodiment/Control/MessagingSystem/RouterExecutable.d \
./opencog/embodiment/Control/MessagingSystem/RouterMessage.d \
./opencog/embodiment/Control/MessagingSystem/RouterServerSocket.d \
./opencog/embodiment/Control/MessagingSystem/ServerSocket.d \
./opencog/embodiment/Control/MessagingSystem/Spawner.d \
./opencog/embodiment/Control/MessagingSystem/SpawnerExecutable.d \
./opencog/embodiment/Control/MessagingSystem/StringMessage.d \
./opencog/embodiment/Control/MessagingSystem/TickMessage.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/embodiment/Control/MessagingSystem/%.o: ../opencog/embodiment/Control/MessagingSystem/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


