################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/SpiApp.cpp \
../src/libSpi.cpp \
../src/max3107.cpp 

OBJS += \
./src/SpiApp.o \
./src/libSpi.o \
./src/max3107.o 

CPP_DEPS += \
./src/SpiApp.d \
./src/libSpi.d \
./src/max3107.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabi-g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


