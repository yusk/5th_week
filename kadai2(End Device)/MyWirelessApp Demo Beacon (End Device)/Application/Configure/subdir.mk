################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Application/Configure/GlobalVars.c" \

C_SRCS += \
../Application/Configure/GlobalVars.c \

OBJS += \
./Application/Configure/GlobalVars_c.obj \

OBJS_QUOTED += \
"./Application/Configure/GlobalVars_c.obj" \

C_DEPS += \
./Application/Configure/GlobalVars_c.d \

C_DEPS_QUOTED += \
"./Application/Configure/GlobalVars_c.d" \

OBJS_OS_FORMAT += \
./Application/Configure/GlobalVars_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Application/Configure/GlobalVars_c.obj: ../Application/Configure/GlobalVars.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #45 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Application/Configure/GlobalVars.args" -ObjN="Application/Configure/GlobalVars_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Application/Configure/%.d: ../Application/Configure/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


