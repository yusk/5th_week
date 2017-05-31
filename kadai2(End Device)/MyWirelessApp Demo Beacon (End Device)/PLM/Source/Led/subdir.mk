################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../PLM/Source/Led/Led.c" \

C_SRCS += \
../PLM/Source/Led/Led.c \

OBJS += \
./PLM/Source/Led/Led_c.obj \

OBJS_QUOTED += \
"./PLM/Source/Led/Led_c.obj" \

C_DEPS += \
./PLM/Source/Led/Led_c.d \

C_DEPS_QUOTED += \
"./PLM/Source/Led/Led_c.d" \

OBJS_OS_FORMAT += \
./PLM/Source/Led/Led_c.obj \


# Each subdirectory must supply rules for building sources it contributes
PLM/Source/Led/Led_c.obj: ../PLM/Source/Led/Led.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #22 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/Led/Led.args" -ObjN="PLM/Source/Led/Led_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/Source/Led/%.d: ../PLM/Source/Led/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


