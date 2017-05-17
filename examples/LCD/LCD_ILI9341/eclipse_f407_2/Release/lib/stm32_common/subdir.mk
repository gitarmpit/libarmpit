################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/svn/libarmpit/lib/stm32_common/system_time.c 

CPP_SRCS += \
C:/svn/libarmpit/lib/stm32_common/dac.cpp \
C:/svn/libarmpit/lib/stm32_common/exti.cpp \
C:/svn/libarmpit/lib/stm32_common/peripheral.cpp \
C:/svn/libarmpit/lib/stm32_common/spi.cpp \
C:/svn/libarmpit/lib/stm32_common/timer.cpp 

OBJS += \
./lib/stm32_common/dac.o \
./lib/stm32_common/exti.o \
./lib/stm32_common/peripheral.o \
./lib/stm32_common/spi.o \
./lib/stm32_common/system_time.o \
./lib/stm32_common/timer.o 

C_DEPS += \
./lib/stm32_common/system_time.d 

CPP_DEPS += \
./lib/stm32_common/dac.d \
./lib/stm32_common/exti.d \
./lib/stm32_common/peripheral.d \
./lib/stm32_common/spi.d \
./lib/stm32_common/timer.d 


# Each subdirectory must supply rules for building sources it contributes
lib/stm32_common/dac.o: C:/svn/libarmpit/lib/stm32_common/dac.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g3 -DSTM32F407xx -DLOAD_FONT4 -DSEMIHOSTING -DUSE_SPI -DUSE_TIMER -DHSE_VALUE=8000000 -I"C:\svn\libarmpit\examples\LCD\LCD_ILI9341\eclipse_f407_2\src" -I"C:\svn\libarmpit\lib\mcortex_core" -I"C:\svn\libarmpit\lib" -I"C:\svn\libarmpit\lib\mcortex_core\armv7m" -I"C:\svn\libarmpit\lib\stm32_common" -I"C:\svn\libarmpit\lib\stm32f4xx\lib" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/stm32_common/exti.o: C:/svn/libarmpit/lib/stm32_common/exti.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g3 -DSTM32F407xx -DLOAD_FONT4 -DSEMIHOSTING -DUSE_SPI -DUSE_TIMER -DHSE_VALUE=8000000 -I"C:\svn\libarmpit\examples\LCD\LCD_ILI9341\eclipse_f407_2\src" -I"C:\svn\libarmpit\lib\mcortex_core" -I"C:\svn\libarmpit\lib" -I"C:\svn\libarmpit\lib\mcortex_core\armv7m" -I"C:\svn\libarmpit\lib\stm32_common" -I"C:\svn\libarmpit\lib\stm32f4xx\lib" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/stm32_common/peripheral.o: C:/svn/libarmpit/lib/stm32_common/peripheral.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g3 -DSTM32F407xx -DLOAD_FONT4 -DSEMIHOSTING -DUSE_SPI -DUSE_TIMER -DHSE_VALUE=8000000 -I"C:\svn\libarmpit\examples\LCD\LCD_ILI9341\eclipse_f407_2\src" -I"C:\svn\libarmpit\lib\mcortex_core" -I"C:\svn\libarmpit\lib" -I"C:\svn\libarmpit\lib\mcortex_core\armv7m" -I"C:\svn\libarmpit\lib\stm32_common" -I"C:\svn\libarmpit\lib\stm32f4xx\lib" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/stm32_common/spi.o: C:/svn/libarmpit/lib/stm32_common/spi.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g3 -DSTM32F407xx -DLOAD_FONT4 -DSEMIHOSTING -DUSE_SPI -DUSE_TIMER -DHSE_VALUE=8000000 -I"C:\svn\libarmpit\examples\LCD\LCD_ILI9341\eclipse_f407_2\src" -I"C:\svn\libarmpit\lib\mcortex_core" -I"C:\svn\libarmpit\lib" -I"C:\svn\libarmpit\lib\mcortex_core\armv7m" -I"C:\svn\libarmpit\lib\stm32_common" -I"C:\svn\libarmpit\lib\stm32f4xx\lib" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/stm32_common/system_time.o: C:/svn/libarmpit/lib/stm32_common/system_time.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g3 -DUSE_SPI -DSEMIHOSTING -DUSE_TIMER -DSTM32F407xx -DHSE_VALUE=8000000 -I"C:\svn\libarmpit\examples\LCD\LCD_ILI9341\eclipse_f407_2\src" -I"C:\svn\libarmpit\lib\mcortex_core" -I"C:\svn\libarmpit\lib" -I"C:\svn\libarmpit\lib\mcortex_core\armv7m" -I"C:\svn\libarmpit\lib\stm32_common" -I"C:\svn\libarmpit\lib\stm32f4xx\lib" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/stm32_common/timer.o: C:/svn/libarmpit/lib/stm32_common/timer.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g3 -DSTM32F407xx -DLOAD_FONT4 -DSEMIHOSTING -DUSE_SPI -DUSE_TIMER -DHSE_VALUE=8000000 -I"C:\svn\libarmpit\examples\LCD\LCD_ILI9341\eclipse_f407_2\src" -I"C:\svn\libarmpit\lib\mcortex_core" -I"C:\svn\libarmpit\lib" -I"C:\svn\libarmpit\lib\mcortex_core\armv7m" -I"C:\svn\libarmpit\lib\stm32_common" -I"C:\svn\libarmpit\lib\stm32f4xx\lib" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


