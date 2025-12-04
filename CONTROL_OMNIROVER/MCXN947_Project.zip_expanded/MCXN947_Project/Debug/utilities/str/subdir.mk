################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/str/fsl_str.c 

C_DEPS += \
./utilities/str/fsl_str.d 

OBJS += \
./utilities/str/fsl_str.o 


# Each subdirectory must supply rules for building sources it contributes
utilities/str/%.o: ../utilities/str/%.c utilities/str/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -DSERIAL_PORT_TYPE_UART=1 -DSDK_DEBUGCONSOLE=1 -DPRINTF_FLOAT_ENABLE=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/drivers" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/device" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/utilities/debug_console" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/component/uart" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/utilities/debug_console/config" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/component/serial_manager" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/component/lists" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/device/periph" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/utilities" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/CMSIS" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/CMSIS/m-profile" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/utilities/str" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/board" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/source" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-utilities-2f-str

clean-utilities-2f-str:
	-$(RM) ./utilities/str/fsl_str.d ./utilities/str/fsl_str.o

.PHONY: clean-utilities-2f-str

