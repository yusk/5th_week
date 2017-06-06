################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../PLM/Source/TempSensor/LM61B.c" \

C_SRCS += \
../PLM/Source/TempSensor/LM61B.c \

OBJS += \
./PLM/Source/TempSensor/LM61B_c.obj \

OBJS_QUOTED += \
"./PLM/Source/TempSensor/LM61B_c.obj" \

C_DEPS += \
./PLM/Source/TempSensor/LM61B_c.d \

C_DEPS_QUOTED += \
"./PLM/Source/TempSensor/LM61B_c.d" \

OBJS_OS_FORMAT += \
./PLM/Source/TempSensor/LM61B_c.obj \


# Each subdirectory must supply rules for building sources it contributes
PLM/Source/TempSensor/LM61B_c.obj: ../PLM/Source/TempSensor/LM61B.c pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/TempSensor/LM61B.args" -ObjN="PLM/Source/TempSensor/LM61B_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/Source/TempSensor/%.d: ../PLM/Source/TempSensor/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


