################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../opencog/ubigraph/client.c 

CC_SRCS += \
../opencog/ubigraph/BITUbigrapher.cc \
../opencog/ubigraph/UbigraphModule.cc \
../opencog/ubigraph/Ubigrapher.cc 

OBJS += \
./opencog/ubigraph/BITUbigrapher.o \
./opencog/ubigraph/UbigraphModule.o \
./opencog/ubigraph/Ubigrapher.o \
./opencog/ubigraph/client.o 

C_DEPS += \
./opencog/ubigraph/client.d 

CC_DEPS += \
./opencog/ubigraph/BITUbigrapher.d \
./opencog/ubigraph/UbigraphModule.d \
./opencog/ubigraph/Ubigrapher.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/ubigraph/%.o: ../opencog/ubigraph/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

opencog/ubigraph/%.o: ../opencog/ubigraph/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


