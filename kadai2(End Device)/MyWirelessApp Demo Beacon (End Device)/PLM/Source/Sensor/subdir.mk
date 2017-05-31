################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../PLM/Source/Sensor/FXAS21000.c" \
"../PLM/Source/Sensor/MAG3110.c" \
"../PLM/Source/Sensor/MMA8652.c" \
"../PLM/Source/Sensor/MMA9553.c" \
"../PLM/Source/Sensor/MPL3115A2.c" \

C_SRCS += \
../PLM/Source/Sensor/FXAS21000.c \
../PLM/Source/Sensor/MAG3110.c \
../PLM/Source/Sensor/MMA8652.c \
../PLM/Source/Sensor/MMA9553.c \
../PLM/Source/Sensor/MPL3115A2.c \

OBJS += \
./PLM/Source/Sensor/FXAS21000_c.obj \
./PLM/Source/Sensor/MAG3110_c.obj \
./PLM/Source/Sensor/MMA8652_c.obj \
./PLM/Source/Sensor/MMA9553_c.obj \
./PLM/Source/Sensor/MPL3115A2_c.obj \

OBJS_QUOTED += \
"./PLM/Source/Sensor/FXAS21000_c.obj" \
"./PLM/Source/Sensor/MAG3110_c.obj" \
"./PLM/Source/Sensor/MMA8652_c.obj" \
"./PLM/Source/Sensor/MMA9553_c.obj" \
"./PLM/Source/Sensor/MPL3115A2_c.obj" \

C_DEPS += \
./PLM/Source/Sensor/FXAS21000_c.d \
./PLM/Source/Sensor/MAG3110_c.d \
./PLM/Source/Sensor/MMA8652_c.d \
./PLM/Source/Sensor/MMA9553_c.d \
./PLM/Source/Sensor/MPL3115A2_c.d \

C_DEPS_QUOTED += \
"./PLM/Source/Sensor/FXAS21000_c.d" \
"./PLM/Source/Sensor/MAG3110_c.d" \
"./PLM/Source/Sensor/MMA8652_c.d" \
"./PLM/Source/Sensor/MMA9553_c.d" \
"./PLM/Source/Sensor/MPL3115A2_c.d" \

OBJS_OS_FORMAT += \
./PLM/Source/Sensor/FXAS21000_c.obj \
./PLM/Source/Sensor/MAG3110_c.obj \
./PLM/Source/Sensor/MMA8652_c.obj \
./PLM/Source/Sensor/MMA9553_c.obj \
./PLM/Source/Sensor/MPL3115A2_c.obj \


# Each subdirectory must supply rules for building sources it contributes
PLM/Source/Sensor/FXAS21000_c.obj: ../PLM/Source/Sensor/FXAS21000.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #13 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/Sensor/FXAS21000.args" -ObjN="PLM/Source/Sensor/FXAS21000_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/Source/Sensor/%.d: ../PLM/Source/Sensor/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

PLM/Source/Sensor/MAG3110_c.obj: ../PLM/Source/Sensor/MAG3110.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #14 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/Sensor/MAG3110.args" -ObjN="PLM/Source/Sensor/MAG3110_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/Source/Sensor/MMA8652_c.obj: ../PLM/Source/Sensor/MMA8652.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #15 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/Sensor/MMA8652.args" -ObjN="PLM/Source/Sensor/MMA8652_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/Source/Sensor/MMA9553_c.obj: ../PLM/Source/Sensor/MMA9553.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #16 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/Sensor/MMA9553.args" -ObjN="PLM/Source/Sensor/MMA9553_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/Source/Sensor/MPL3115A2_c.obj: ../PLM/Source/Sensor/MPL3115A2.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #17 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/Sensor/MPL3115A2.args" -ObjN="PLM/Source/Sensor/MPL3115A2_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '


