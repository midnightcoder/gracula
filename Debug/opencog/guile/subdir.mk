################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/guile/PrimitiveExample.cc \
../opencog/guile/SchemeAdHoc.cc \
../opencog/guile/SchemeEval.cc \
../opencog/guile/SchemeExec.cc \
../opencog/guile/SchemePrimitive.cc \
../opencog/guile/SchemeSmob.cc \
../opencog/guile/SchemeSmobAV.cc \
../opencog/guile/SchemeSmobAtom.cc \
../opencog/guile/SchemeSmobGC.cc \
../opencog/guile/SchemeSmobNew.cc \
../opencog/guile/SchemeSmobTV.cc \
../opencog/guile/SchemeSmobVH.cc 

OBJS += \
./opencog/guile/PrimitiveExample.o \
./opencog/guile/SchemeAdHoc.o \
./opencog/guile/SchemeEval.o \
./opencog/guile/SchemeExec.o \
./opencog/guile/SchemePrimitive.o \
./opencog/guile/SchemeSmob.o \
./opencog/guile/SchemeSmobAV.o \
./opencog/guile/SchemeSmobAtom.o \
./opencog/guile/SchemeSmobGC.o \
./opencog/guile/SchemeSmobNew.o \
./opencog/guile/SchemeSmobTV.o \
./opencog/guile/SchemeSmobVH.o 

CC_DEPS += \
./opencog/guile/PrimitiveExample.d \
./opencog/guile/SchemeAdHoc.d \
./opencog/guile/SchemeEval.d \
./opencog/guile/SchemeExec.d \
./opencog/guile/SchemePrimitive.d \
./opencog/guile/SchemeSmob.d \
./opencog/guile/SchemeSmobAV.d \
./opencog/guile/SchemeSmobAtom.d \
./opencog/guile/SchemeSmobGC.d \
./opencog/guile/SchemeSmobNew.d \
./opencog/guile/SchemeSmobTV.d \
./opencog/guile/SchemeSmobVH.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/guile/%.o: ../opencog/guile/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


