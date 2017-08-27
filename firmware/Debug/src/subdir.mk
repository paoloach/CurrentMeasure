################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/_write.c 

CPP_SRCS += \
../src/ADS7841.cpp \
../src/Buttons.cpp \
../src/Current.cpp \
../src/CurrentMeasure.cpp \
../src/Delay.cpp \
../src/GFX.cpp \
../src/HX8357.cpp \
../src/MessageQueue.cpp \
../src/Timer.cpp \
../src/main.cpp 

OBJS += \
./src/ADS7841.o \
./src/Buttons.o \
./src/Current.o \
./src/CurrentMeasure.o \
./src/Delay.o \
./src/GFX.o \
./src/HX8357.o \
./src/MessageQueue.o \
./src/Timer.o \
./src/_write.o \
./src/main.o 

C_DEPS += \
./src/_write.d 

CPP_DEPS += \
./src/ADS7841.d \
./src/Buttons.d \
./src/Current.d \
./src/CurrentMeasure.d \
./src/Delay.d \
./src/GFX.d \
./src/HX8357.d \
./src/MessageQueue.d \
./src/Timer.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_HAL_DRIVER -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DHSE_VALUE=25000000 -DUSE_SPI -DUSE_DMA -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_HAL_DRIVER -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DHSE_VALUE=25000000 -DUSE_SPI -DUSE_DMA -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


