################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/UI/UI.c 

OBJS += \
./Src/UI/UI.o 

C_DEPS += \
./Src/UI/UI.d 


# Each subdirectory must supply rules for building sources it contributes
Src/UI/UI.o: ../Src/UI/UI.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DELEYS" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DS18B20" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/EEPROM" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/HD44780" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/NTC" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/TIME_COUNTER" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/UI" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/UI/UI.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
