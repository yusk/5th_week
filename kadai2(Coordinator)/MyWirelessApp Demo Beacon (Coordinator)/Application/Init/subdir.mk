################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Application/Init/MApp_init.c" \
"../Application/Init/MacInit.c" \

C_SRCS += \
../Application/Init/MApp_init.c \
../Application/Init/MacInit.c \

OBJS += \
./Application/Init/MApp_init_c.obj \
./Application/Init/MacInit_c.obj \

OBJS_QUOTED += \
"./Application/Init/MApp_init_c.obj" \
"./Application/Init/MacInit_c.obj" \

C_DEPS += \
./Application/Init/MApp_init_c.d \
./Application/Init/MacInit_c.d \

C_DEPS_QUOTED += \
"./Application/Init/MApp_init_c.d" \
"./Application/Init/MacInit_c.d" \

OBJS_OS_FORMAT += \
./Application/Init/MApp_init_c.obj \
./Application/Init/MacInit_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Application/Init/MApp_init_c.obj: ../Application/Init/MApp_init.c pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #43 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Application/Init/MApp_init.args" -ObjN="Application/Init/MApp_init_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Application/Init/%.d: ../Application/Init/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Application/Init/MacInit_c.obj: ../Application/Init/MacInit.c pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #44 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Application/Init/MacInit.args" -ObjN="Application/Init/MacInit_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '


