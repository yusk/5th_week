################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../PLM/Source/Sound/Sound.c" \

C_SRCS += \
../PLM/Source/Sound/Sound.c \

OBJS += \
./PLM/Source/Sound/Sound_c.obj \

OBJS_QUOTED += \
"./PLM/Source/Sound/Sound_c.obj" \

C_DEPS += \
./PLM/Source/Sound/Sound_c.d \

C_DEPS_QUOTED += \
"./PLM/Source/Sound/Sound_c.d" \

OBJS_OS_FORMAT += \
./PLM/Source/Sound/Sound_c.obj \


# Each subdirectory must supply rules for building sources it contributes
PLM/Source/Sound/Sound_c.obj: ../PLM/Source/Sound/Sound.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #12 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/Sound/Sound.args" -ObjN="PLM/Source/Sound/Sound_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/Source/Sound/%.d: ../PLM/Source/Sound/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


