################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/nlp/wsd/EdgeThin.cc \
../opencog/nlp/wsd/EdgeUtils.cc \
../opencog/nlp/wsd/Mihalcea.cc \
../opencog/nlp/wsd/MihalceaEdge.cc \
../opencog/nlp/wsd/MihalceaLabel.cc \
../opencog/nlp/wsd/NNAdjust.cc \
../opencog/nlp/wsd/ParseRank.cc \
../opencog/nlp/wsd/ReportRank.cc \
../opencog/nlp/wsd/SenseCache.cc \
../opencog/nlp/wsd/SenseRank.cc \
../opencog/nlp/wsd/SenseSimilarityLCH.cc \
../opencog/nlp/wsd/SenseSimilaritySQL.cc \
../opencog/nlp/wsd/Sweep.cc \
../opencog/nlp/wsd/WordSenseProcessor.cc 

OBJS += \
./opencog/nlp/wsd/EdgeThin.o \
./opencog/nlp/wsd/EdgeUtils.o \
./opencog/nlp/wsd/Mihalcea.o \
./opencog/nlp/wsd/MihalceaEdge.o \
./opencog/nlp/wsd/MihalceaLabel.o \
./opencog/nlp/wsd/NNAdjust.o \
./opencog/nlp/wsd/ParseRank.o \
./opencog/nlp/wsd/ReportRank.o \
./opencog/nlp/wsd/SenseCache.o \
./opencog/nlp/wsd/SenseRank.o \
./opencog/nlp/wsd/SenseSimilarityLCH.o \
./opencog/nlp/wsd/SenseSimilaritySQL.o \
./opencog/nlp/wsd/Sweep.o \
./opencog/nlp/wsd/WordSenseProcessor.o 

CC_DEPS += \
./opencog/nlp/wsd/EdgeThin.d \
./opencog/nlp/wsd/EdgeUtils.d \
./opencog/nlp/wsd/Mihalcea.d \
./opencog/nlp/wsd/MihalceaEdge.d \
./opencog/nlp/wsd/MihalceaLabel.d \
./opencog/nlp/wsd/NNAdjust.d \
./opencog/nlp/wsd/ParseRank.d \
./opencog/nlp/wsd/ReportRank.d \
./opencog/nlp/wsd/SenseCache.d \
./opencog/nlp/wsd/SenseRank.d \
./opencog/nlp/wsd/SenseSimilarityLCH.d \
./opencog/nlp/wsd/SenseSimilaritySQL.d \
./opencog/nlp/wsd/Sweep.d \
./opencog/nlp/wsd/WordSenseProcessor.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/nlp/wsd/%.o: ../opencog/nlp/wsd/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


