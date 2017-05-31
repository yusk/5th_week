################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../PLM/Source/Common/Sys/NV_Data.c" \
"../PLM/Source/Common/Sys/PlatformInit.c" \
"../PLM/Source/Common/Sys/crt0.c" \
"../PLM/Source/Common/Sys/dummyisr.c" \
"../PLM/Source/Common/Sys/icg.c" \
"../PLM/Source/Common/Sys/isrvectors.c" \

C_SRCS += \
../PLM/Source/Common/Sys/NV_Data.c \
../PLM/Source/Common/Sys/PlatformInit.c \
../PLM/Source/Common/Sys/crt0.c \
../PLM/Source/Common/Sys/dummyisr.c \
../PLM/Source/Common/Sys/icg.c \
../PLM/Source/Common/Sys/isrvectors.c \

OBJS += \
./PLM/Source/Common/Sys/NV_Data_c.obj \
./PLM/Source/Common/Sys/PlatformInit_c.obj \
./PLM/Source/Common/Sys/crt0_c.obj \
./PLM/Source/Common/Sys/dummyisr_c.obj \
./PLM/Source/Common/Sys/icg_c.obj \
./PLM/Source/Common/Sys/isrvectors_c.obj \

OBJS_QUOTED += \
"./PLM/Source/Common/Sys/NV_Data_c.obj" \
"./PLM/Source/Common/Sys/PlatformInit_c.obj" \
"./PLM/Source/Common/Sys/crt0_c.obj" \
"./PLM/Source/Common/Sys/dummyisr_c.obj" \
"./PLM/Source/Common/Sys/icg_c.obj" \
"./PLM/Source/Common/Sys/isrvectors_c.obj" \

C_DEPS += \
./PLM/Source/Common/Sys/NV_Data_c.d \
./PLM/Source/Common/Sys/PlatformInit_c.d \
./PLM/Source/Common/Sys/crt0_c.d \
./PLM/Source/Common/Sys/dummyisr_c.d \
./PLM/Source/Common/Sys/icg_c.d \
./PLM/Source/Common/Sys/isrvectors_c.d \

C_DEPS_QUOTED += \
"./PLM/Source/Common/Sys/NV_Data_c.d" \
"./PLM/Source/Common/Sys/PlatformInit_c.d" \
"./PLM/Source/Common/Sys/crt0_c.d" \
"./PLM/Source/Common/Sys/dummyisr_c.d" \
"./PLM/Source/Common/Sys/icg_c.d" \
"./PLM/Source/Common/Sys/isrvectors_c.d" \

OBJS_OS_FORMAT += \
./PLM/Source/Common/Sys/NV_Data_c.obj \
./PLM/Source/Common/Sys/PlatformInit_c.obj \
./PLM/Source/Common/Sys/crt0_c.obj \
./PLM/Source/Common/Sys/dummyisr_c.obj \
./PLM/Source/Common/Sys/icg_c.obj \
./PLM/Source/Common/Sys/isrvectors_c.obj \


# Each subdirectory must supply rules for building sources it contributes
PLM/Source/Common/Sys/NV_Data_c.obj: ../PLM/Source/Common/Sys/NV_Data.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #29 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/Common/Sys/NV_Data.args" -ObjN="PLM/Source/Common/Sys/NV_Data_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/Source/Common/Sys/%.d: ../PLM/Source/Common/Sys/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

PLM/Source/Common/Sys/PlatformInit_c.obj: ../PLM/Source/Common/Sys/PlatformInit.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #30 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/Common/Sys/PlatformInit.args" -ObjN="PLM/Source/Common/Sys/PlatformInit_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/Source/Common/Sys/crt0_c.obj: ../PLM/Source/Common/Sys/crt0.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #31 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/Common/Sys/crt0.args" -ObjN="PLM/Source/Common/Sys/crt0_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/Source/Common/Sys/dummyisr_c.obj: ../PLM/Source/Common/Sys/dummyisr.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #32 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/Common/Sys/dummyisr.args" -ObjN="PLM/Source/Common/Sys/dummyisr_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/Source/Common/Sys/icg_c.obj: ../PLM/Source/Common/Sys/icg.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #33 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/Common/Sys/icg.args" -ObjN="PLM/Source/Common/Sys/icg_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/Source/Common/Sys/isrvectors_c.obj: ../PLM/Source/Common/Sys/isrvectors.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #34 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/Common/Sys/isrvectors.args" -ObjN="PLM/Source/Common/Sys/isrvectors_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '


