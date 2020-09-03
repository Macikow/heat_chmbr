################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/UI/subdir.mk
-include Src/TIME_COUNTER/subdir.mk
-include Src/NTC/subdir.mk
-include Src/HD44780/subdir.mk
-include Src/DS18B20/subdir.mk
-include Src/DELEYS/subdir.mk
-include Src/subdir.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
HEAT_CHMBR.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
HEAT_CHMBR.list \

OBJCOPY_BIN += \
HEAT_CHMBR.bin \


# All Target
all: HEAT_CHMBR.elf secondary-outputs

# Tool invocations
HEAT_CHMBR.elf: $(OBJS) $(USER_OBJS) C:\Users\kowma\stm_code\HEAT_CHMBR\STM32F103C8TX_FLASH.ld
	arm-none-eabi-gcc -o "HEAT_CHMBR.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\kowma\stm_code\HEAT_CHMBR\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="HEAT_CHMBR.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

HEAT_CHMBR.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "HEAT_CHMBR.list"
	@echo 'Finished building: $@'
	@echo ' '

HEAT_CHMBR.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "HEAT_CHMBR.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
