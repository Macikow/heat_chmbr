################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/EEPROM/EEPROM.c 

OBJS += \
./Src/EEPROM/EEPROM.o 

C_DEPS += \
./Src/EEPROM/EEPROM.d 


# Each subdirectory must supply rules for building sources it contributes
Src/EEPROM/EEPROM.o: ../Src/EEPROM/EEPROM.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/NTC" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/EEPROM" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/HD44780" -I../Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/TIME_COUNTER" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DS18B20" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/UI" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DELEYS" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/PWM_CTRL" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/EEPROM/EEPROM.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

