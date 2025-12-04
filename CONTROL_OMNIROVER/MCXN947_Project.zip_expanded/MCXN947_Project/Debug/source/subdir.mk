################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/ADC_DRIVER.c \
../source/ESP_SPI.c \
../source/GPIO_DRIVER.c \
../source/MCXN947_Project.c \
../source/PWM_DRIVER.c \
../source/RobotTelemetry.c \
../source/TIMER_DRIVER.c \
../source/semihost_hardfault.c 

C_DEPS += \
./source/ADC_DRIVER.d \
./source/ESP_SPI.d \
./source/GPIO_DRIVER.d \
./source/MCXN947_Project.d \
./source/PWM_DRIVER.d \
./source/RobotTelemetry.d \
./source/TIMER_DRIVER.d \
./source/semihost_hardfault.d 

OBJS += \
./source/ADC_DRIVER.o \
./source/ESP_SPI.o \
./source/GPIO_DRIVER.o \
./source/MCXN947_Project.o \
./source/PWM_DRIVER.o \
./source/RobotTelemetry.o \
./source/TIMER_DRIVER.o \
./source/semihost_hardfault.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -DSERIAL_PORT_TYPE_UART=1 -DSDK_DEBUGCONSOLE=1 -DPRINTF_FLOAT_ENABLE=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\drivers" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\device" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\utilities\debug_console" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\component\uart" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\utilities\debug_console\config" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\component\serial_manager" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\component\lists" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\device\periph" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\utilities" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\CMSIS" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\CMSIS\m-profile" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\utilities\str" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\board" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\source" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/ADC_DRIVER.d ./source/ADC_DRIVER.o ./source/ESP_SPI.d ./source/ESP_SPI.o ./source/GPIO_DRIVER.d ./source/GPIO_DRIVER.o ./source/MCXN947_Project.d ./source/MCXN947_Project.o ./source/PWM_DRIVER.d ./source/PWM_DRIVER.o ./source/RobotTelemetry.d ./source/RobotTelemetry.o ./source/TIMER_DRIVER.d ./source/TIMER_DRIVER.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o

.PHONY: clean-source

