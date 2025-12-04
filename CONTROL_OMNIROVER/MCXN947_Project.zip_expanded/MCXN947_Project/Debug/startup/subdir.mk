################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../startup/boot_multicore_slave.c \
../startup/startup_mcxn947_cm33_core0.c 

C_DEPS += \
./startup/boot_multicore_slave.d \
./startup/startup_mcxn947_cm33_core0.d 

OBJS += \
./startup/boot_multicore_slave.o \
./startup/startup_mcxn947_cm33_core0.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.c startup/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -DSERIAL_PORT_TYPE_UART=1 -DSDK_DEBUGCONSOLE=1 -DPRINTF_FLOAT_ENABLE=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/drivers" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/device" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/utilities/debug_console" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/component/uart" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/utilities/debug_console/config" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/component/serial_manager" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/component/lists" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/device/periph" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/utilities" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/CMSIS" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/CMSIS/m-profile" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/utilities/str" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/board" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/source" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-startup

clean-startup:
	-$(RM) ./startup/boot_multicore_slave.d ./startup/boot_multicore_slave.o ./startup/startup_mcxn947_cm33_core0.d ./startup/startup_mcxn947_cm33_core0.o

.PHONY: clean-startup

