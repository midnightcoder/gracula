################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/server/Agent.cc \
../opencog/server/BaseServer.cc \
../opencog/server/BuiltinRequestsModule.cc \
../opencog/server/CogServer.cc \
../opencog/server/CogServerMain.cc \
../opencog/server/ConsoleSocket.cc \
../opencog/server/DataRequest.cc \
../opencog/server/ExitRequest.cc \
../opencog/server/HelpRequest.cc \
../opencog/server/ListRequest.cc \
../opencog/server/LoadModuleRequest.cc \
../opencog/server/LoadRequest.cc \
../opencog/server/NetworkServer.cc \
../opencog/server/Request.cc \
../opencog/server/SaveRequest.cc \
../opencog/server/ServerSocket.cc \
../opencog/server/ShutdownRequest.cc \
../opencog/server/SleepRequest.cc \
../opencog/server/SystemActivityTable.cc \
../opencog/server/UnloadModuleRequest.cc \
../opencog/server/load-file.cc 

OBJS += \
./opencog/server/Agent.o \
./opencog/server/BaseServer.o \
./opencog/server/BuiltinRequestsModule.o \
./opencog/server/CogServer.o \
./opencog/server/CogServerMain.o \
./opencog/server/ConsoleSocket.o \
./opencog/server/DataRequest.o \
./opencog/server/ExitRequest.o \
./opencog/server/HelpRequest.o \
./opencog/server/ListRequest.o \
./opencog/server/LoadModuleRequest.o \
./opencog/server/LoadRequest.o \
./opencog/server/NetworkServer.o \
./opencog/server/Request.o \
./opencog/server/SaveRequest.o \
./opencog/server/ServerSocket.o \
./opencog/server/ShutdownRequest.o \
./opencog/server/SleepRequest.o \
./opencog/server/SystemActivityTable.o \
./opencog/server/UnloadModuleRequest.o \
./opencog/server/load-file.o 

CC_DEPS += \
./opencog/server/Agent.d \
./opencog/server/BaseServer.d \
./opencog/server/BuiltinRequestsModule.d \
./opencog/server/CogServer.d \
./opencog/server/CogServerMain.d \
./opencog/server/ConsoleSocket.d \
./opencog/server/DataRequest.d \
./opencog/server/ExitRequest.d \
./opencog/server/HelpRequest.d \
./opencog/server/ListRequest.d \
./opencog/server/LoadModuleRequest.d \
./opencog/server/LoadRequest.d \
./opencog/server/NetworkServer.d \
./opencog/server/Request.d \
./opencog/server/SaveRequest.d \
./opencog/server/ServerSocket.d \
./opencog/server/ShutdownRequest.d \
./opencog/server/SleepRequest.d \
./opencog/server/SystemActivityTable.d \
./opencog/server/UnloadModuleRequest.d \
./opencog/server/load-file.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/server/%.o: ../opencog/server/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


