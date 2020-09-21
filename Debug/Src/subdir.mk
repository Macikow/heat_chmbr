################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/main.c \
../Src/stm32f1xx_hal_msp.c \
../Src/stm32f1xx_it.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32f1xx.c 

OBJS += \
./Src/main.o \
./Src/stm32f1xx_hal_msp.o \
./Src/stm32f1xx_it.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32f1xx.o 

C_DEPS += \
./Src/main.d \
./Src/stm32f1xx_hal_msp.d \
./Src/stm32f1xx_it.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/main.o: ../Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/NTC" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/EEPROM" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/HD44780" -I../Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/TIME_COUNTER" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DS18B20" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/UI" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DELEYS" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/PWM_CTRL" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/main.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Src/stm32f1xx_hal_msp.o: ../Src/stm32f1xx_hal_msp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/NTC" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/EEPROM" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/HD44780" -I../Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/TIME_COUNTER" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DS18B20" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/UI" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DELEYS" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/PWM_CTRL" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32f1xx_hal_msp.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Src/stm32f1xx_it.o: ../Src/stm32f1xx_it.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/NTC" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/EEPROM" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/HD44780" -I../Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/TIME_COUNTER" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DS18B20" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/UI" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DELEYS" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/PWM_CTRL" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32f1xx_it.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Src/syscalls.o: ../Src/syscalls.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/NTC" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/EEPROM" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/HD44780" -I../Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/TIME_COUNTER" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DS18B20" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/UI" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DELEYS" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/PWM_CTRL" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/syscalls.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Src/sysmem.o: ../Src/sysmem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/NTC" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/EEPROM" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/HD44780" -I../Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/TIME_COUNTER" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DS18B20" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/UI" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DELEYS" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/PWM_CTRL" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sysmem.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Src/system_stm32f1xx.o: ../Src/system_stm32f1xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/NTC" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/EEPROM" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/HD44780" -I../Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/TIME_COUNTER" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DS18B20" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/UI" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/DELEYS" -I"C:/Users/kowma/stm_code/HEAT_CHMBR/Src/PWM_CTRL" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/system_stm32f1xx.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

