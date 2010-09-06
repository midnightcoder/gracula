################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../opencog/spatial/math/BoundingBox.cc \
../opencog/spatial/math/Face.cc \
../opencog/spatial/math/MathCommon.cc \
../opencog/spatial/math/Matrix3.cc \
../opencog/spatial/math/Matrix4.cc \
../opencog/spatial/math/Plane.cc \
../opencog/spatial/math/Quaternion.cc \
../opencog/spatial/math/Rectangle.cc 

OBJS += \
./opencog/spatial/math/BoundingBox.o \
./opencog/spatial/math/Face.o \
./opencog/spatial/math/MathCommon.o \
./opencog/spatial/math/Matrix3.o \
./opencog/spatial/math/Matrix4.o \
./opencog/spatial/math/Plane.o \
./opencog/spatial/math/Quaternion.o \
./opencog/spatial/math/Rectangle.o 

CC_DEPS += \
./opencog/spatial/math/BoundingBox.d \
./opencog/spatial/math/Face.d \
./opencog/spatial/math/MathCommon.d \
./opencog/spatial/math/Matrix3.d \
./opencog/spatial/math/Matrix4.d \
./opencog/spatial/math/Plane.d \
./opencog/spatial/math/Quaternion.d \
./opencog/spatial/math/Rectangle.d 


# Each subdirectory must supply rules for building sources it contributes
opencog/spatial/math/%.o: ../opencog/spatial/math/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencog -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


