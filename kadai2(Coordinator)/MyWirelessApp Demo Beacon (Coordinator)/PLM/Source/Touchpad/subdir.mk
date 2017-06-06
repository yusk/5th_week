################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../PLM/Source/Touchpad/Touchpad.c" \

C_SRCS += \
../PLM/Source/Touchpad/Touchpad.c \

OBJS += \
./PLM/Source/Touchpad/Touchpad_c.obj \

OBJS_QUOTED += \
"./PLM/Source/Touchpad/Touchpad_c.obj" \

C_DEPS += \
./PLM/Source/Touchpad/Touchpad_c.d \

C_DEPS_QUOTED += \
"./PLM/Source/Touchpad/Touchpad_c.d" \

OBJS_OS_FORMAT += \
./PLM/Source/Touchpad/Touchpad_c.obj \


# Each subdirectory must supply rules for building sources it contributes
PLM/Source/Touchpad/Touchpad_c.obj: ../PLM/Source/Touchpad/Touchpad.c pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/Touchpad/Touchpad.args" -ObjN="PLM/Source/Touchpad/Touchpad_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/Source/Touchpad/%.d: ../PLM/Source/Touchpad/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


