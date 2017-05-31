################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../PLM/Source/IIC/IIC.c" \

C_SRCS += \
../PLM/Source/IIC/IIC.c \

OBJS += \
./PLM/Source/IIC/IIC_c.obj \

OBJS_QUOTED += \
"./PLM/Source/IIC/IIC_c.obj" \

C_DEPS += \
./PLM/Source/IIC/IIC_c.d \

C_DEPS_QUOTED += \
"./PLM/Source/IIC/IIC_c.d" \

OBJS_OS_FORMAT += \
./PLM/Source/IIC/IIC_c.obj \


# Each subdirectory must supply rules for building sources it contributes
PLM/Source/IIC/IIC_c.obj: ../PLM/Source/IIC/IIC.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #26 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/IIC/IIC.args" -ObjN="PLM/Source/IIC/IIC_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/Source/IIC/%.d: ../PLM/Source/IIC/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


