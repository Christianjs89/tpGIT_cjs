################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/API/Src/API_debounce.c \
../Drivers/API/Src/API_delay.c \
../Drivers/API/Src/API_drv8825.c \
../Drivers/API/Src/API_lcd_i2c.c \
../Drivers/API/Src/API_menuMEF.c \
../Drivers/API/Src/API_uart.c 

OBJS += \
./Drivers/API/Src/API_debounce.o \
./Drivers/API/Src/API_delay.o \
./Drivers/API/Src/API_drv8825.o \
./Drivers/API/Src/API_lcd_i2c.o \
./Drivers/API/Src/API_menuMEF.o \
./Drivers/API/Src/API_uart.o 

C_DEPS += \
./Drivers/API/Src/API_debounce.d \
./Drivers/API/Src/API_delay.d \
./Drivers/API/Src/API_drv8825.d \
./Drivers/API/Src/API_lcd_i2c.d \
./Drivers/API/Src/API_menuMEF.d \
./Drivers/API/Src/API_uart.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/API/Src/%.o Drivers/API/Src/%.su Drivers/API/Src/%.cyclo: ../Drivers/API/Src/%.c Drivers/API/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/chris/STM32CubeIDE/workspace_1.13.2/PdM_repositorio/PdM-PCSE_TP_Final/Drivers/API/Inc" -I"C:/Users/chris/STM32CubeIDE/workspace_1.13.2/PdM_repositorio/PdM-PCSE_TP_Final/Drivers/API/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-API-2f-Src

clean-Drivers-2f-API-2f-Src:
	-$(RM) ./Drivers/API/Src/API_debounce.cyclo ./Drivers/API/Src/API_debounce.d ./Drivers/API/Src/API_debounce.o ./Drivers/API/Src/API_debounce.su ./Drivers/API/Src/API_delay.cyclo ./Drivers/API/Src/API_delay.d ./Drivers/API/Src/API_delay.o ./Drivers/API/Src/API_delay.su ./Drivers/API/Src/API_drv8825.cyclo ./Drivers/API/Src/API_drv8825.d ./Drivers/API/Src/API_drv8825.o ./Drivers/API/Src/API_drv8825.su ./Drivers/API/Src/API_lcd_i2c.cyclo ./Drivers/API/Src/API_lcd_i2c.d ./Drivers/API/Src/API_lcd_i2c.o ./Drivers/API/Src/API_lcd_i2c.su ./Drivers/API/Src/API_menuMEF.cyclo ./Drivers/API/Src/API_menuMEF.d ./Drivers/API/Src/API_menuMEF.o ./Drivers/API/Src/API_menuMEF.su ./Drivers/API/Src/API_uart.cyclo ./Drivers/API/Src/API_uart.d ./Drivers/API/Src/API_uart.o ./Drivers/API/Src/API_uart.su

.PHONY: clean-Drivers-2f-API-2f-Src

