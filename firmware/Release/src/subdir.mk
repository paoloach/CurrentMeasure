################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/_write.c 

CPP_SRCS += \
../src/BlinkLed.cpp \
../src/Current.cpp \
../src/Delay.cpp \
../src/GFX.cpp \
../src/HX8367.cpp \
../src/ILI9325.cpp \
../src/ILI9341.cpp \
../src/SampleHour.cpp \
../src/Timer.cpp \
../src/main.cpp 

OBJS += \
./src/BlinkLed.o \
./src/Current.o \
./src/Delay.o \
./src/GFX.o \
./src/HX8367.o \
./src/ILI9325.o \
./src/ILI9341.o \
./src/SampleHour.o \
./src/Timer.o \
./src/_write.o \
./src/main.o 

C_DEPS += \
./src/_write.d 

CPP_DEPS += \
./src/BlinkLed.d \
./src/Current.d \
./src/Delay.d \
./src/GFX.d \
./src/HX8367.d \
./src/ILI9325.d \
./src/ILI9341.d \
./src/SampleHour.d \
./src/Timer.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


