################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/svn/libarmpit/lib/stm32f4xx/lib/RCC_Helper.c \
C:/svn/libarmpit/lib/stm32f4xx/lib/dma_c.c \
C:/svn/libarmpit/lib/stm32f4xx/lib/flash.c \
C:/svn/libarmpit/lib/stm32f4xx/lib/i2c_c.c \
C:/svn/libarmpit/lib/stm32f4xx/lib/pwr.c \
C:/svn/libarmpit/lib/stm32f4xx/lib/rcc.c \
C:/svn/libarmpit/lib/stm32f4xx/lib/rtc.c \
C:/svn/libarmpit/lib/stm32f4xx/lib/system_init.c 

CPP_SRCS += \
C:/svn/libarmpit/lib/stm32f4xx/lib/GPIO_Helper.cpp \
C:/svn/libarmpit/lib/stm32f4xx/lib/adc.cpp \
C:/svn/libarmpit/lib/stm32f4xx/lib/dma.cpp \
C:/svn/libarmpit/lib/stm32f4xx/lib/gpio.cpp \
C:/svn/libarmpit/lib/stm32f4xx/lib/i2c.cpp \
C:/svn/libarmpit/lib/stm32f4xx/lib/syscfg.cpp \
C:/svn/libarmpit/lib/stm32f4xx/lib/usart.cpp 

OBJS += \
./lib/stm32f4xx/lib/GPIO_Helper.o \
./lib/stm32f4xx/lib/RCC_Helper.o \
./lib/stm32f4xx/lib/adc.o \
./lib/stm32f4xx/lib/dma.o \
./lib/stm32f4xx/lib/dma_c.o \
./lib/stm32f4xx/lib/flash.o \
./lib/stm32f4xx/lib/gpio.o \
./lib/stm32f4xx/lib/i2c.o \
./lib/stm32f4xx/lib/i2c_c.o \
./lib/stm32f4xx/lib/pwr.o \
./lib/stm32f4xx/lib/rcc.o \
./lib/stm32f4xx/lib/rtc.o \
./lib/stm32f4xx/lib/syscfg.o \
./lib/stm32f4xx/lib/system_init.o \
./lib/stm32f4xx/lib/usart.o 

C_DEPS += \
./lib/stm32f4xx/lib/RCC_Helper.d \
./lib/stm32f4xx/lib/dma_c.d \
./lib/stm32f4xx/lib/flash.d \
./lib/stm32f4xx/lib/i2c_c.d \
./lib/stm32f4xx/lib/pwr.d \
./lib/stm32f4xx/lib/rcc.d \
./lib/stm32f4xx/lib/rtc.d \
./lib/stm32f4xx/lib/system_init.d 

CPP_DEPS += \
./lib/stm32f4xx/lib/GPIO_Helper.d \
./lib/stm32f4xx/lib/adc.d \
./lib/stm32f4xx/lib/dma.d \
./lib/stm32f4xx/lib/gpio.d \
./lib/stm32f4xx/lib/i2c.d \
./lib/stm32f4xx/lib/syscfg.d \
./lib/stm32f4xx/lib/usart.d 


# Each subdirectory must supply rules for building sources it contributes
lib/stm32f4xx/lib/GPIO_Helper.o: C:/svn/libarmpit/lib/stm32f4xx/lib/GPIO_Helper.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g3 -DSTM32F407xx -DLOAD_FONT4 -DSEMIHOSTING -DUSE_SPI -DUSE_TIMER -DHSE_VALUE=8000000 -I"C:\svn\libarmpit\examples\LCD\LCD_ILI9341\eclipse_f407_2\src" -I"C:\svn\libarmpit\lib\mcortex_core" -I"C:\svn\libarmpit\lib" -I"C:\svn\libarmpit\lib\mcortex_core\armv7m" -I"C:\svn\libarmpit\lib\stm32_common" -I"C:\svn\libarmpit\lib\stm32f4xx\lib" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/stm32f4xx/lib/RCC_Helper.o: C:/svn/libarmpit/lib/stm32f4xx/lib/RCC_Helper.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g3 -DUSE_SPI -DSEMIHOSTING -DUSE_TIMER -DSTM32F407xx -DHSE_VALUE=8000000 -I"C:\svn\libarmpit\examples\LCD\LCD_ILI9341\eclipse_f407_2\src" -I"C:\svn\libarmpit\lib\mcortex_core" -I"C:\svn\libarmpit\lib" -I"C:\svn\libarmpit\lib\mcortex_core\armv7m" -I"C:\svn\libarmpit\lib\stm32_common" -I"C:\svn\libarmpit\lib\stm32f4xx\lib" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/stm32f4xx/lib/adc.o: C:/svn/libarmpit/lib/stm32f4xx/lib/adc.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g3 -DSTM32F407xx -DLOAD_FONT4 -DSEMIHOSTING -DUSE_SPI -DUSE_TIMER -DHSE_VALUE=8000000 -I"C:\svn\libarmpit\examples\LCD\LCD_ILI9341\eclipse_f407_2\src" -I"C:\svn\libarmpit\lib\mcortex_core" -I"C:\svn\libarmpit\lib" -I"C:\svn\libarmpit\lib\mcortex_core\armv7m" -I"C:\svn\libarmpit\lib\stm32_common" -I"C:\svn\libarmpit\lib\stm32f4xx\lib" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/stm32f4xx/lib/dma.o: C:/svn/libarmpit/lib/stm32f4xx/lib/dma.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g3 -DSTM32F407xx -DLOAD_FONT4 -DSEMIHOSTING -DUSE_SPI -DUSE_TIMER -DHSE_VALUE=8000000 -I"C:\svn\libarmpit\examples\LCD\LCD_ILI9341\eclipse_f407_2\src" -I"C:\svn\libarmpit\lib\mcortex_core" -I"C:\svn\libarmpit\lib" -I"C:\svn\libarmpit\lib\mcortex_core\armv7m" -I"C:\svn\libarmpit\lib\stm32_common" -I"C:\svn\libarmpit\lib\stm32f4xx\lib" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/stm32f4xx/lib/dma_c.o: C:/svn/libarmpit/lib/stm32f4xx/lib/dma_c.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g3 -DUSE_SPI -DSEMIHOSTING -DUSE_TIMER -DSTM32F407xx -DHSE_VALUE=8000000 -I"C:\svn\libarmpit\examples\LCD\LCD_ILI9341\eclipse_f407_2\src" -I"C:\svn\libarmpit\lib\mcortex_core" -I"C:\svn\libarmpit\lib" -I"C:\svn\libarmpit\lib\mcortex_core\armv7m" -I"C:\svn\libarmpit\lib\stm32_common" -I"C:\svn\libarmpit\lib\stm32f4xx\lib" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/stm32f4xx/lib/flash.o: C:/svn/libarmpit/lib/stm32f4xx/lib/flash.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g3 -DUSE_SPI -DSEMIHOSTING -DUSE_TIMER -DSTM32F407xx -DHSE_VALUE=8000000 -I"C:\svn\libarmpit\examples\LCD\LCD_ILI9341\eclipse_f407_2\src" -I"C:\svn\libarmpit\lib\mcortex_core" -I"C:\svn\libarmpit\lib" -I"C:\svn\libarmpit\lib\mcortex_core\armv7m" -I"C:\svn\libarmpit\lib\stm32_common" -I"C:\svn\libarmpit\lib\stm32f4xx\lib" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/stm32f4xx/lib/gpio.o: C:/svn/libarmpit/lib/stm32f4xx/lib/gpio.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g3 -DSTM32F407xx -DLOAD_FONT4 -DSEMIHOSTING -DUSE_SPI -DUSE_TIMER -DHSE_VALUE=8000000 -I"C:\svn\libarmpit\examples\LCD\LCD_ILI9341\eclipse_f407_2\src" -I"C:\svn\libarmpit\lib\mcortex_core" -I"C:\svn\libarmpit\lib" -I"C:\svn\libarmpit\lib\mcortex_core\armv7m" -I"C:\svn\libarmpit\lib\stm32_common" -I"C:\svn\libarmpit\lib\stm32f4xx\lib" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/stm32f4xx/lib/i2c.o: C:/svn/libarmpit/lib/stm32f4xx/lib/i2c.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g3 -DSTM32F407xx -DLOAD_FONT4 -DSEMIHOSTING -DUSE_SPI -DUSE_TIMER -DHSE_VALUE=8000000 -I"C:\svn\libarmpit\examples\LCD\LCD_ILI9341\eclipse_f407_2\src" -I"C:\svn\libarmpit\lib\mcortex_core" -I"C:\svn\libarmpit\lib" -I"C:\svn\libarmpit\lib\mcortex_core\armv7m" -I"C:\svn\libarmpit\lib\stm32_common" -I"C:\svn\libarmpit\lib\stm32f4xx\lib" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/stm32f4xx/lib/i2c_c.o: C:/svn/libarmpit/lib/stm32f4xx/lib/i2c_c.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g3 -DUSE_SPI -DSEMIHOSTING -DUSE_TIMER -DSTM32F407xx -DHSE_VALUE=8000000 -I"C:\svn\libarmpit\examples\LCD\LCD_ILI9341\eclipse_f407_2\src" -I"C:\svn\libarmpit\lib\mcortex_core" -I"C:\svn\libarmpit\lib" -I"C:\svn\libarmpit\lib\mcortex_core\armv7m" -I"C:\svn\libarmpit\lib\stm32_common" -I"C:\svn\libarmpit\lib\stm32f4xx\lib" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/stm32f4xx/lib/pwr.o: C:/svn/libarmpit/lib/stm32f4xx/lib/pwr.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g3 -DUSE_SPI -DSEMIHOSTING -DUSE_TIMER -DSTM32F407xx -DHSE_VALUE=8000000 -I"C:\svn\libarmpit\examples\LCD\LCD_ILI9341\eclipse_f407_2\src" -I"C:\svn\libarmpit\lib\mcortex_core" -I"C:\svn\libarmpit\lib" -I"C:\svn\libarmpit\lib\mcortex_core\armv7m" -I"C:\svn\libarmpit\lib\stm32_common" -I"C:\svn\libarmpit\lib\stm32f4xx\lib" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/stm32f4xx/lib/rcc.o: C:/svn/libarmpit/lib/stm32f4xx/lib/rcc.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g3 -DUSE_SPI -DSEMIHOSTING -DUSE_TIMER -DSTM32F407xx -DHSE_VALUE=8000000 -I"C:\svn\libarmpit\examples\LCD\LCD_ILI9341\eclipse_f407_2\src" -I"C:\svn\libarmpit\lib\mcortex_core" -I"C:\svn\libarmpit\lib" -I"C:\svn\libarmpit\lib\mcortex_core\armv7m" -I"C:\svn\libarmpit\lib\stm32_common" -I"C:\svn\libarmpit\lib\stm32f4xx\lib" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/stm32f4xx/lib/rtc.o: C:/svn/libarmpit/lib/stm32f4xx/lib/rtc.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g3 -DUSE_SPI -DSEMIHOSTING -DUSE_TIMER -DSTM32F407xx -DHSE_VALUE=8000000 -I"C:\svn\libarmpit\examples\LCD\LCD_ILI9341\eclipse_f407_2\src" -I"C:\svn\libarmpit\lib\mcortex_core" -I"C:\svn\libarmpit\lib" -I"C:\svn\libarmpit\lib\mcortex_core\armv7m" -I"C:\svn\libarmpit\lib\stm32_common" -I"C:\svn\libarmpit\lib\stm32f4xx\lib" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/stm32f4xx/lib/syscfg.o: C:/svn/libarmpit/lib/stm32f4xx/lib/syscfg.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g3 -DSTM32F407xx -DLOAD_FONT4 -DSEMIHOSTING -DUSE_SPI -DUSE_TIMER -DHSE_VALUE=8000000 -I"C:\svn\libarmpit\examples\LCD\LCD_ILI9341\eclipse_f407_2\src" -I"C:\svn\libarmpit\lib\mcortex_core" -I"C:\svn\libarmpit\lib" -I"C:\svn\libarmpit\lib\mcortex_core\armv7m" -I"C:\svn\libarmpit\lib\stm32_common" -I"C:\svn\libarmpit\lib\stm32f4xx\lib" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/stm32f4xx/lib/system_init.o: C:/svn/libarmpit/lib/stm32f4xx/lib/system_init.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g3 -DUSE_SPI -DSEMIHOSTING -DUSE_TIMER -DSTM32F407xx -DHSE_VALUE=8000000 -I"C:\svn\libarmpit\examples\LCD\LCD_ILI9341\eclipse_f407_2\src" -I"C:\svn\libarmpit\lib\mcortex_core" -I"C:\svn\libarmpit\lib" -I"C:\svn\libarmpit\lib\mcortex_core\armv7m" -I"C:\svn\libarmpit\lib\stm32_common" -I"C:\svn\libarmpit\lib\stm32f4xx\lib" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/stm32f4xx/lib/usart.o: C:/svn/libarmpit/lib/stm32f4xx/lib/usart.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g3 -DSTM32F407xx -DLOAD_FONT4 -DSEMIHOSTING -DUSE_SPI -DUSE_TIMER -DHSE_VALUE=8000000 -I"C:\svn\libarmpit\examples\LCD\LCD_ILI9341\eclipse_f407_2\src" -I"C:\svn\libarmpit\lib\mcortex_core" -I"C:\svn\libarmpit\lib" -I"C:\svn\libarmpit\lib\mcortex_core\armv7m" -I"C:\svn\libarmpit\lib\stm32_common" -I"C:\svn\libarmpit\lib\stm32f4xx\lib" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


