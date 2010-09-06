################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../opencog/web/mongoose.c 

CC_SRCS += \
../opencog/web/AtomURLHandler.cc \
../opencog/web/BaseURLHandler.cc \
../opencog/web/CreateAtomRequest.cc \
../opencog/web/GetAtomRequest.cc \
../opencog/web/GetListRequest.cc \
../opencog/web/JsonUtil.cc \
../opencog/web/ListURLHandler.cc \
../opencog/web/ServerRequestWrapper.cc \
../opencog/web/UpdateAtomRequest.cc \
../opencog/web/WebModule.cc 

OBJS += \
./opencog/web/AtomURLHandler.o \
./opencog/web/BaseURLHandler.o \
./opencog/web/CreateAtomRequest.o \
./opencog/web/GetAtomRequest.o \
./opencog/web/GetListRequest.o \
./opencog/web/JsonUtil.o \
./opencog/web/ListURLHandler.o \
./opencog/web/ServerRequestWrapper.o \
./opencog/web/UpdateAtomRequest.o \
./opencog/web/WebModule.o \
./opencog/web/mongoose.o 

C_DEPS += \
./opencog/web/mongoose.d 

CC_DEPS += \
./opencog/web/AtomURLHandler.d \
./opencog/web/BaseURLHandler.d \
./opencog/web/CreateAtomRequest.d \
./opencog/web/GetAtomRequest.d \
./opencog/web/GetListRequest.d \
./opencog/web/JsonUtil.d \
./opencog/web/ListURLHandler.d \
./opencog/web/ServerRequestWrapper.d \
./opencog/web/UpdateAtomRequest.d \
./opencog/web/WebModule.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/web/%.o: ../opencog/web/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

opencog/web/%.o: ../opencog/web/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


