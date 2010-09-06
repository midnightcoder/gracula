################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/persist/xml/FileXMLBufferReader.cc \
../opencog/persist/xml/NMXmlExporter.cc \
../opencog/persist/xml/NMXmlParser.cc \
../opencog/persist/xml/StringXMLBufferReader.cc 

OBJS += \
./opencog/persist/xml/FileXMLBufferReader.o \
./opencog/persist/xml/NMXmlExporter.o \
./opencog/persist/xml/NMXmlParser.o \
./opencog/persist/xml/StringXMLBufferReader.o 

CC_DEPS += \
./opencog/persist/xml/FileXMLBufferReader.d \
./opencog/persist/xml/NMXmlExporter.d \
./opencog/persist/xml/NMXmlParser.d \
./opencog/persist/xml/StringXMLBufferReader.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/persist/xml/%.o: ../opencog/persist/xml/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


