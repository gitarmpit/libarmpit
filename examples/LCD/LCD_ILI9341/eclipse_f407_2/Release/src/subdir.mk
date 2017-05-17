################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/glcdfont.c 

CPP_SRCS += \
../src/Adafruit_GFX.cpp \
../src/Adafruit_ILI9341.cpp \
../src/main.cpp 

OBJS += \
./src/Adafruit_GFX.o \
./src/Adafruit_ILI9341.o \
./src/glcdfont.o \
./src/main.o 

C_DEPS += \
./src/glcdfont.d 

CPP_DEPS += \
./src/Adafruit_GFX.d \
./src/Adafruit_ILI9341.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g3 -DSTM32F407xx -DLOAD_FONT4 -DSEMIHOSTING -DUSE_SPI -DUSE_TIMER -DHSE_VALUE=8000000 -I"C:\svn\libarmpit\examples\LCD\LCD_ILI9341\eclipse_f407_2\src" -I"C:\svn\libarmpit\lib\mcortex_core" -I"C:\svn\libarmpit\lib" -I"C:\svn\libarmpit\lib\mcortex_core\armv7m" -I"C:\svn\libarmpit\lib\stm32_common" -I"C:\svn\libarmpit\lib\stm32f4xx\lib" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g3 -DUSE_SPI -DSEMIHOSTING -DUSE_TIMER -DSTM32F407xx -DHSE_VALUE=8000000 -I"C:\svn\libarmpit\examples\LCD\LCD_ILI9341\eclipse_f407_2\src" -I"C:\svn\libarmpit\lib\mcortex_core" -I"C:\svn\libarmpit\lib" -I"C:\svn\libarmpit\lib\mcortex_core\armv7m" -I"C:\svn\libarmpit\lib\stm32_common" -I"C:\svn\libarmpit\lib\stm32f4xx\lib" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


