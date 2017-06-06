################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../PLM/Source/RST/RSTHal.c" \

C_SRCS += \
../PLM/Source/RST/RSTHal.c \

OBJS += \
./PLM/Source/RST/RSTHal_c.obj \

OBJS_QUOTED += \
"./PLM/Source/RST/RSTHal_c.obj" \

C_DEPS += \
./PLM/Source/RST/RSTHal_c.d \

C_DEPS_QUOTED += \
"./PLM/Source/RST/RSTHal_c.d" \

OBJS_OS_FORMAT += \
./PLM/Source/RST/RSTHal_c.obj \


# Each subdirectory must supply rules for building sources it contributes
PLM/Source/RST/RSTHal_c.obj: ../PLM/Source/RST/RSTHal.c pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #19 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/RST/RSTHal.args" -ObjN="PLM/Source/RST/RSTHal_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/Source/RST/%.d: ../PLM/Source/RST/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


