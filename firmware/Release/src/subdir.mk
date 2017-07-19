################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/_write.c 

CPP_SRCS += \
../src/ADS7841.cpp \
../src/BlinkLed.cpp \
../src/Current.cpp \
../src/CurrentMeasure.cpp \
../src/Delay.cpp \
../src/GFX.cpp \
../src/Graphic.cpp \
../src/HX8357.cpp \
../src/SampleHour.cpp \
../src/Sampling.cpp \
../src/Timer.cpp \
../src/main.cpp 

OBJS += \
./src/ADS7841.o \
./src/BlinkLed.o \
./src/Current.o \
./src/CurrentMeasure.o \
./src/Delay.o \
./src/GFX.o \
./src/Graphic.o \
./src/HX8357.o \
./src/SampleHour.o \
./src/Sampling.o \
./src/Timer.o \
./src/_write.o \
./src/main.o 

C_DEPS += \
./src/_write.d 

CPP_DEPS += \
./src/ADS7841.d \
./src/BlinkLed.d \
./src/Current.d \
./src/CurrentMeasure.d \
./src/Delay.d \
./src/GFX.d \
./src/Graphic.d \
./src/HX8357.d \
./src/SampleHour.d \
./src/Sampling.d \
./src/Timer.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g -DHSE_VALUE=8000000 -DUSE_HAL_DRIVER -DNDEBUG -DSTM32F407xx -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g -DHSE_VALUE=8000000 -DUSE_HAL_DRIVER -DNDEBUG -DSTM32F407xx -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


