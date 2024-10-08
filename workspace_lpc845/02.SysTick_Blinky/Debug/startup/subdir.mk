################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../startup/startup_lpc845.c 

C_DEPS += \
./startup/startup_lpc845.d 

OBJS += \
./startup/startup_lpc845.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.c startup/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_LPC845M301JBD48 -DCPU_LPC845M301JBD48_cm0plus -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\estudiante\Documents\GitHub\lpc 2\curso-lse\workspace_lpc845\02.SysTick_Blinky\board" -I"C:\Users\estudiante\Documents\GitHub\lpc 2\curso-lse\workspace_lpc845\02.SysTick_Blinky\source" -I"C:\Users\estudiante\Documents\GitHub\lpc 2\curso-lse\workspace_lpc845\02.SysTick_Blinky\component\uart" -I"C:\Users\estudiante\Documents\GitHub\lpc 2\curso-lse\workspace_lpc845\02.SysTick_Blinky\drivers" -I"C:\Users\estudiante\Documents\GitHub\lpc 2\curso-lse\workspace_lpc845\02.SysTick_Blinky\CMSIS" -I"C:\Users\estudiante\Documents\GitHub\lpc 2\curso-lse\workspace_lpc845\02.SysTick_Blinky\device" -I"C:\Users\estudiante\Documents\GitHub\lpc 2\curso-lse\workspace_lpc845\02.SysTick_Blinky\utilities" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-startup

clean-startup:
	-$(RM) ./startup/startup_lpc845.d ./startup/startup_lpc845.o

.PHONY: clean-startup

