################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/UI/UI.c \
../Src/UI/indicators.c \
../Src/UI/infinite_heater.c \
../Src/UI/program_and_memory.c \
../Src/UI/sequance.c 

OBJS += \
./Src/UI/UI.o \
./Src/UI/indicators.o \
./Src/UI/infinite_heater.o \
./Src/UI/program_and_memory.o \
./Src/UI/sequance.o 

C_DEPS += \
./Src/UI/UI.d \
./Src/UI/indicators.d \
./Src/UI/infinite_heater.d \
./Src/UI/program_and_memory.d \
./Src/UI/sequance.d 


# Each subdirectory must supply rules for building sources it contributes
Src/UI/UI.o: ../Src/UI/UI.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu99 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/NTC" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/EEPROM" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/HD44780" -I../Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/PID" -I../Drivers/CMSIS/Include -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/TIME_COUNTER" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DS18B20" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/SERVIS" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/UI" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DELEYS" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/PWM_CTRL" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/UI/UI.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Src/UI/indicators.o: ../Src/UI/indicators.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu99 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/NTC" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/EEPROM" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/HD44780" -I../Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/PID" -I../Drivers/CMSIS/Include -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/TIME_COUNTER" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DS18B20" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/SERVIS" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/UI" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DELEYS" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/PWM_CTRL" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/UI/indicators.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Src/UI/infinite_heater.o: ../Src/UI/infinite_heater.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu99 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/NTC" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/EEPROM" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/HD44780" -I../Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/PID" -I../Drivers/CMSIS/Include -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/TIME_COUNTER" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DS18B20" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/SERVIS" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/UI" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DELEYS" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/PWM_CTRL" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/UI/infinite_heater.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Src/UI/program_and_memory.o: ../Src/UI/program_and_memory.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu99 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/NTC" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/EEPROM" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/HD44780" -I../Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/PID" -I../Drivers/CMSIS/Include -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/TIME_COUNTER" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DS18B20" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/SERVIS" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/UI" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DELEYS" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/PWM_CTRL" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/UI/program_and_memory.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Src/UI/sequance.o: ../Src/UI/sequance.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu99 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/NTC" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/EEPROM" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/HD44780" -I../Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/PID" -I../Drivers/CMSIS/Include -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/TIME_COUNTER" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DS18B20" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/SERVIS" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/UI" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DELEYS" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/PWM_CTRL" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/UI/sequance.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

