################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/learning/moses/main/build-representation.cc \
../opencog/learning/moses/main/continmax.cc \
../opencog/learning/moses/main/eval-truth-table.cc \
../opencog/learning/moses/main/gen-contin-table.cc \
../opencog/learning/moses/main/gen-mixed-table.cc \
../opencog/learning/moses/main/gen-truth-table.cc \
../opencog/learning/moses/main/moses-ann-pole1.cc \
../opencog/learning/moses/main/moses-ann-pole2-hillclimbing.cc \
../opencog/learning/moses/main/moses-ann-pole2.cc \
../opencog/learning/moses/main/moses-ann-pole2nv.cc \
../opencog/learning/moses/main/moses-ann-xor.cc \
../opencog/learning/moses/main/moses-ant-hillclimbing.cc \
../opencog/learning/moses/main/moses-exec.cc \
../opencog/learning/moses/main/nmax.cc \
../opencog/learning/moses/main/onemax.cc \
../opencog/learning/moses/main/ontomax.cc 

OBJS += \
./opencog/learning/moses/main/build-representation.o \
./opencog/learning/moses/main/continmax.o \
./opencog/learning/moses/main/eval-truth-table.o \
./opencog/learning/moses/main/gen-contin-table.o \
./opencog/learning/moses/main/gen-mixed-table.o \
./opencog/learning/moses/main/gen-truth-table.o \
./opencog/learning/moses/main/moses-ann-pole1.o \
./opencog/learning/moses/main/moses-ann-pole2-hillclimbing.o \
./opencog/learning/moses/main/moses-ann-pole2.o \
./opencog/learning/moses/main/moses-ann-pole2nv.o \
./opencog/learning/moses/main/moses-ann-xor.o \
./opencog/learning/moses/main/moses-ant-hillclimbing.o \
./opencog/learning/moses/main/moses-exec.o \
./opencog/learning/moses/main/nmax.o \
./opencog/learning/moses/main/onemax.o \
./opencog/learning/moses/main/ontomax.o 

CC_DEPS += \
./opencog/learning/moses/main/build-representation.d \
./opencog/learning/moses/main/continmax.d \
./opencog/learning/moses/main/eval-truth-table.d \
./opencog/learning/moses/main/gen-contin-table.d \
./opencog/learning/moses/main/gen-mixed-table.d \
./opencog/learning/moses/main/gen-truth-table.d \
./opencog/learning/moses/main/moses-ann-pole1.d \
./opencog/learning/moses/main/moses-ann-pole2-hillclimbing.d \
./opencog/learning/moses/main/moses-ann-pole2.d \
./opencog/learning/moses/main/moses-ann-pole2nv.d \
./opencog/learning/moses/main/moses-ann-xor.d \
./opencog/learning/moses/main/moses-ant-hillclimbing.d \
./opencog/learning/moses/main/moses-exec.d \
./opencog/learning/moses/main/nmax.d \
./opencog/learning/moses/main/onemax.d \
./opencog/learning/moses/main/ontomax.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/learning/moses/main/%.o: ../opencog/learning/moses/main/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


