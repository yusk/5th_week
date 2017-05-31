################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../PLM/Source/Accelerometer/Accelerometer.c" \

C_SRCS += \
../PLM/Source/Accelerometer/Accelerometer.c \

OBJS += \
./PLM/Source/Accelerometer/Accelerometer_c.obj \

OBJS_QUOTED += \
"./PLM/Source/Accelerometer/Accelerometer_c.obj" \

C_DEPS += \
./PLM/Source/Accelerometer/Accelerometer_c.d \

C_DEPS_QUOTED += \
"./PLM/Source/Accelerometer/Accelerometer_c.d" \

OBJS_OS_FORMAT += \
./PLM/Source/Accelerometer/Accelerometer_c.obj \


# Each subdirectory must supply rules for building sources it contributes
PLM/Source/Accelerometer/Accelerometer_c.obj: ../PLM/Source/Accelerometer/Accelerometer.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #36 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/Accelerometer/Accelerometer.args" -ObjN="PLM/Source/Accelerometer/Accelerometer_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/Source/Accelerometer/%.d: ../PLM/Source/Accelerometer/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


