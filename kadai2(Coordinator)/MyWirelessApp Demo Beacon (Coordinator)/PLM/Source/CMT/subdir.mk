################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../PLM/Source/CMT/CMT.c" \

C_SRCS += \
../PLM/Source/CMT/CMT.c \

OBJS += \
./PLM/Source/CMT/CMT_c.obj \

OBJS_QUOTED += \
"./PLM/Source/CMT/CMT_c.obj" \

C_DEPS += \
./PLM/Source/CMT/CMT_c.d \

C_DEPS_QUOTED += \
"./PLM/Source/CMT/CMT_c.d" \

OBJS_OS_FORMAT += \
./PLM/Source/CMT/CMT_c.obj \


# Each subdirectory must supply rules for building sources it contributes
PLM/Source/CMT/CMT_c.obj: ../PLM/Source/CMT/CMT.c pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #35 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/CMT/CMT.args" -ObjN="PLM/Source/CMT/CMT_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/Source/CMT/%.d: ../PLM/Source/CMT/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


