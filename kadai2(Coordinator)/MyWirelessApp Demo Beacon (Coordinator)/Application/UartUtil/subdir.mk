################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Application/UartUtil/UartUtil.c" \

C_SRCS += \
../Application/UartUtil/UartUtil.c \

OBJS += \
./Application/UartUtil/UartUtil_c.obj \

OBJS_QUOTED += \
"./Application/UartUtil/UartUtil_c.obj" \

C_DEPS += \
./Application/UartUtil/UartUtil_c.d \

C_DEPS_QUOTED += \
"./Application/UartUtil/UartUtil_c.d" \

OBJS_OS_FORMAT += \
./Application/UartUtil/UartUtil_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Application/UartUtil/UartUtil_c.obj: ../Application/UartUtil/UartUtil.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #41 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Application/UartUtil/UartUtil.args" -ObjN="Application/UartUtil/UartUtil_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Application/UartUtil/%.d: ../Application/UartUtil/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


