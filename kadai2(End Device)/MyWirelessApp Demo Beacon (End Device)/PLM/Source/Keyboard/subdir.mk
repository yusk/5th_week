################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../PLM/Source/Keyboard/Keyboard.c" \

C_SRCS += \
../PLM/Source/Keyboard/Keyboard.c \

OBJS += \
./PLM/Source/Keyboard/Keyboard_c.obj \

OBJS_QUOTED += \
"./PLM/Source/Keyboard/Keyboard_c.obj" \

C_DEPS += \
./PLM/Source/Keyboard/Keyboard_c.d \

C_DEPS_QUOTED += \
"./PLM/Source/Keyboard/Keyboard_c.d" \

OBJS_OS_FORMAT += \
./PLM/Source/Keyboard/Keyboard_c.obj \


# Each subdirectory must supply rules for building sources it contributes
PLM/Source/Keyboard/Keyboard_c.obj: ../PLM/Source/Keyboard/Keyboard.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #25 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/Keyboard/Keyboard.args" -ObjN="PLM/Source/Keyboard/Keyboard_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/Source/Keyboard/%.d: ../PLM/Source/Keyboard/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


