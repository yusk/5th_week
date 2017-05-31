################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../PLM/Source/TMR/Timer.c" \

C_SRCS += \
../PLM/Source/TMR/Timer.c \

OBJS += \
./PLM/Source/TMR/Timer_c.obj \

OBJS_QUOTED += \
"./PLM/Source/TMR/Timer_c.obj" \

C_DEPS += \
./PLM/Source/TMR/Timer_c.d \

C_DEPS_QUOTED += \
"./PLM/Source/TMR/Timer_c.d" \

OBJS_OS_FORMAT += \
./PLM/Source/TMR/Timer_c.obj \


# Each subdirectory must supply rules for building sources it contributes
PLM/Source/TMR/Timer_c.obj: ../PLM/Source/TMR/Timer.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #11 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/TMR/Timer.args" -ObjN="PLM/Source/TMR/Timer_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/Source/TMR/%.d: ../PLM/Source/TMR/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


