################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../PLM/Source/UART/Uart.c" \

C_SRCS += \
../PLM/Source/UART/Uart.c \

OBJS += \
./PLM/Source/UART/Uart_c.obj \

OBJS_QUOTED += \
"./PLM/Source/UART/Uart_c.obj" \

C_DEPS += \
./PLM/Source/UART/Uart_c.d \

C_DEPS_QUOTED += \
"./PLM/Source/UART/Uart_c.d" \

OBJS_OS_FORMAT += \
./PLM/Source/UART/Uart_c.obj \


# Each subdirectory must supply rules for building sources it contributes
PLM/Source/UART/Uart_c.obj: ../PLM/Source/UART/Uart.c pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/UART/Uart.args" -ObjN="PLM/Source/UART/Uart_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/Source/UART/%.d: ../PLM/Source/UART/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


