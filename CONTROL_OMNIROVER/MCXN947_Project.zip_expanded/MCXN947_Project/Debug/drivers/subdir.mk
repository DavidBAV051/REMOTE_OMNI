################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_common_arm.c \
../drivers/fsl_ctimer.c \
../drivers/fsl_gpio.c \
../drivers/fsl_lpadc.c \
../drivers/fsl_lpflexcomm.c \
../drivers/fsl_lpi2c.c \
../drivers/fsl_lpspi.c \
../drivers/fsl_lptmr.c \
../drivers/fsl_lpuart.c \
../drivers/fsl_pwm.c \
../drivers/fsl_reset.c \
../drivers/fsl_spc.c \
../drivers/fsl_vref.c \
../drivers/omnidriver.c 

C_DEPS += \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_common_arm.d \
./drivers/fsl_ctimer.d \
./drivers/fsl_gpio.d \
./drivers/fsl_lpadc.d \
./drivers/fsl_lpflexcomm.d \
./drivers/fsl_lpi2c.d \
./drivers/fsl_lpspi.d \
./drivers/fsl_lptmr.d \
./drivers/fsl_lpuart.d \
./drivers/fsl_pwm.d \
./drivers/fsl_reset.d \
./drivers/fsl_spc.d \
./drivers/fsl_vref.d \
./drivers/omnidriver.d 

OBJS += \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_common_arm.o \
./drivers/fsl_ctimer.o \
./drivers/fsl_gpio.o \
./drivers/fsl_lpadc.o \
./drivers/fsl_lpflexcomm.o \
./drivers/fsl_lpi2c.o \
./drivers/fsl_lpspi.o \
./drivers/fsl_lptmr.o \
./drivers/fsl_lpuart.o \
./drivers/fsl_pwm.o \
./drivers/fsl_reset.o \
./drivers/fsl_spc.o \
./drivers/fsl_vref.o \
./drivers/omnidriver.o 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -DSERIAL_PORT_TYPE_UART=1 -DSDK_DEBUGCONSOLE=1 -DPRINTF_FLOAT_ENABLE=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/drivers" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/device" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/utilities/debug_console" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/component/uart" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/utilities/debug_console/config" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/component/serial_manager" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/component/lists" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/device/periph" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/utilities" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/CMSIS" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/CMSIS/m-profile" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/utilities/str" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/board" -I"/Users/davidbav/Documents/FINAL EMBEDDED/MCXN947_Project/source" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-drivers

clean-drivers:
	-$(RM) ./drivers/fsl_clock.d ./drivers/fsl_clock.o ./drivers/fsl_common.d ./drivers/fsl_common.o ./drivers/fsl_common_arm.d ./drivers/fsl_common_arm.o ./drivers/fsl_ctimer.d ./drivers/fsl_ctimer.o ./drivers/fsl_gpio.d ./drivers/fsl_gpio.o ./drivers/fsl_lpadc.d ./drivers/fsl_lpadc.o ./drivers/fsl_lpflexcomm.d ./drivers/fsl_lpflexcomm.o ./drivers/fsl_lpi2c.d ./drivers/fsl_lpi2c.o ./drivers/fsl_lpspi.d ./drivers/fsl_lpspi.o ./drivers/fsl_lptmr.d ./drivers/fsl_lptmr.o ./drivers/fsl_lpuart.d ./drivers/fsl_lpuart.o ./drivers/fsl_pwm.d ./drivers/fsl_pwm.o ./drivers/fsl_reset.d ./drivers/fsl_reset.o ./drivers/fsl_spc.d ./drivers/fsl_spc.o ./drivers/fsl_vref.d ./drivers/fsl_vref.o ./drivers/omnidriver.d ./drivers/omnidriver.o

.PHONY: clean-drivers

