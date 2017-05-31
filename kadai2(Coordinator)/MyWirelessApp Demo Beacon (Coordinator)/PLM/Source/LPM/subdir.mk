################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../PLM/Source/LPM/PWRLib.c" \
"../PLM/Source/LPM/PWR_MC1323x.c" \

C_SRCS += \
../PLM/Source/LPM/PWRLib.c \
../PLM/Source/LPM/PWR_MC1323x.c \

OBJS += \
./PLM/Source/LPM/PWRLib_c.obj \
./PLM/Source/LPM/PWR_MC1323x_c.obj \

OBJS_QUOTED += \
"./PLM/Source/LPM/PWRLib_c.obj" \
"./PLM/Source/LPM/PWR_MC1323x_c.obj" \

C_DEPS += \
./PLM/Source/LPM/PWRLib_c.d \
./PLM/Source/LPM/PWR_MC1323x_c.d \

C_DEPS_QUOTED += \
"./PLM/Source/LPM/PWRLib_c.d" \
"./PLM/Source/LPM/PWR_MC1323x_c.d" \

OBJS_OS_FORMAT += \
./PLM/Source/LPM/PWRLib_c.obj \
./PLM/Source/LPM/PWR_MC1323x_c.obj \


# Each subdirectory must supply rules for building sources it contributes
PLM/Source/LPM/PWRLib_c.obj: ../PLM/Source/LPM/PWRLib.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #23 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/LPM/PWRLib.args" -ObjN="PLM/Source/LPM/PWRLib_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/Source/LPM/%.d: ../PLM/Source/LPM/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

PLM/Source/LPM/PWR_MC1323x_c.obj: ../PLM/Source/LPM/PWR_MC1323x.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #24 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/LPM/PWR_MC1323x.args" -ObjN="PLM/Source/LPM/PWR_MC1323x_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '


