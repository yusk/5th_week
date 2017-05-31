################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../PLM/Source/FunctionLib/FunctionLib.c" \

C_SRCS += \
../PLM/Source/FunctionLib/FunctionLib.c \

OBJS += \
./PLM/Source/FunctionLib/FunctionLib_c.obj \

OBJS_QUOTED += \
"./PLM/Source/FunctionLib/FunctionLib_c.obj" \

C_DEPS += \
./PLM/Source/FunctionLib/FunctionLib_c.d \

C_DEPS_QUOTED += \
"./PLM/Source/FunctionLib/FunctionLib_c.d" \

OBJS_OS_FORMAT += \
./PLM/Source/FunctionLib/FunctionLib_c.obj \


# Each subdirectory must supply rules for building sources it contributes
PLM/Source/FunctionLib/FunctionLib_c.obj: ../PLM/Source/FunctionLib/FunctionLib.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #27 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/FunctionLib/FunctionLib.args" -ObjN="PLM/Source/FunctionLib/FunctionLib_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/Source/FunctionLib/%.d: ../PLM/Source/FunctionLib/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


