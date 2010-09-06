################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/embodiment/AutomatedSystemTest/GoldStdGen.cc \
../opencog/embodiment/AutomatedSystemTest/GoldStdMessage.cc \
../opencog/embodiment/AutomatedSystemTest/GoldStdReaderAgent.cc \
../opencog/embodiment/AutomatedSystemTest/PBTester.cc \
../opencog/embodiment/AutomatedSystemTest/PBTesterExecutable.cc \
../opencog/embodiment/AutomatedSystemTest/TestConfig.cc 

OBJS += \
./opencog/embodiment/AutomatedSystemTest/GoldStdGen.o \
./opencog/embodiment/AutomatedSystemTest/GoldStdMessage.o \
./opencog/embodiment/AutomatedSystemTest/GoldStdReaderAgent.o \
./opencog/embodiment/AutomatedSystemTest/PBTester.o \
./opencog/embodiment/AutomatedSystemTest/PBTesterExecutable.o \
./opencog/embodiment/AutomatedSystemTest/TestConfig.o 

CC_DEPS += \
./opencog/embodiment/AutomatedSystemTest/GoldStdGen.d \
./opencog/embodiment/AutomatedSystemTest/GoldStdMessage.d \
./opencog/embodiment/AutomatedSystemTest/GoldStdReaderAgent.d \
./opencog/embodiment/AutomatedSystemTest/PBTester.d \
./opencog/embodiment/AutomatedSystemTest/PBTesterExecutable.d \
./opencog/embodiment/AutomatedSystemTest/TestConfig.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/embodiment/AutomatedSystemTest/%.o: ../opencog/embodiment/AutomatedSystemTest/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


