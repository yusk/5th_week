################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../PLM/Source/Display/Display_MC1323x_RCM.c" \

C_SRCS += \
../PLM/Source/Display/Display_MC1323x_RCM.c \

OBJS += \
./PLM/Source/Display/Display_MC1323x_RCM_c.obj \

OBJS_QUOTED += \
"./PLM/Source/Display/Display_MC1323x_RCM_c.obj" \

C_DEPS += \
./PLM/Source/Display/Display_MC1323x_RCM_c.d \

C_DEPS_QUOTED += \
"./PLM/Source/Display/Display_MC1323x_RCM_c.d" \

OBJS_OS_FORMAT += \
./PLM/Source/Display/Display_MC1323x_RCM_c.obj \


# Each subdirectory must supply rules for building sources it contributes
PLM/Source/Display/Display_MC1323x_RCM_c.obj: ../PLM/Source/Display/Display_MC1323x_RCM.c pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #28 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/Display/Display_MC1323x_RCM.args" -ObjN="PLM/Source/Display/Display_MC1323x_RCM_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/Source/Display/%.d: ../PLM/Source/Display/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


