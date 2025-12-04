################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/board.c \
../board/clock_config.c \
../board/hardware_init.c \
../board/peripherals.c \
../board/pin_mux.c 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/hardware_init.d \
./board/peripherals.d \
./board/pin_mux.d 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/hardware_init.o \
./board/peripherals.o \
./board/pin_mux.o 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c board/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -DSERIAL_PORT_TYPE_UART=1 -DSDK_DEBUGCONSOLE=1 -DPRINTF_FLOAT_ENABLE=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\drivers" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\device" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\utilities\debug_console" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\component\uart" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\utilities\debug_console\config" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\component\serial_manager" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\component\lists" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\device\periph" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\utilities" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\CMSIS" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\CMSIS\m-profile" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\utilities\str" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\board" -I"C:\PROJECTS\EMBEDDED\FINAL_PROJECT\OMNIROVER\MCXN947_Project.zip_expanded\MCXN947_Project\source" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-board

clean-board:
	-$(RM) ./board/board.d ./board/board.o ./board/clock_config.d ./board/clock_config.o ./board/hardware_init.d ./board/hardware_init.o ./board/peripherals.d ./board/peripherals.o ./board/pin_mux.d ./board/pin_mux.o

.PHONY: clean-board

