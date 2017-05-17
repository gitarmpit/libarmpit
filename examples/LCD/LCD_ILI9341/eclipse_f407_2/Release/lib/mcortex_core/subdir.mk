################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/svn/libarmpit/lib/mcortex_core/systick.c 

OBJS += \
./lib/mcortex_core/systick.o 

C_DEPS += \
./lib/mcortex_core/systick.d 


# Each subdirectory must supply rules for building sources it contributes
lib/mcortex_core/systick.o: C:/svn/libarmpit/lib/mcortex_core/systick.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra  -g3 -DUSE_SPI -DSEMIHOSTING -DUSE_TIMER -DSTM32F407xx -DHSE_VALUE=8000000 -I"C:\svn\libarmpit\examples\LCD\LCD_ILI9341\eclipse_f407_2\src" -I"C:\svn\libarmpit\lib\mcortex_core" -I"C:\svn\libarmpit\lib" -I"C:\svn\libarmpit\lib\mcortex_core\armv7m" -I"C:\svn\libarmpit\lib\stm32_common" -I"C:\svn\libarmpit\lib\stm32f4xx\lib" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


