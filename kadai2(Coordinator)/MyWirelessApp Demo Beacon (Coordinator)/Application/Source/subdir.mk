################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Application/Source/MApp.c" \

C_SRCS += \
../Application/Source/MApp.c \

OBJS += \
./Application/Source/MApp_c.obj \

OBJS_QUOTED += \
"./Application/Source/MApp_c.obj" \

C_DEPS += \
./Application/Source/MApp_c.d \

C_DEPS_QUOTED += \
"./Application/Source/MApp_c.d" \

OBJS_OS_FORMAT += \
./Application/Source/MApp_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Application/Source/MApp_c.obj: ../Application/Source/MApp.c pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #42 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Application/Source/MApp.args" -ObjN="Application/Source/MApp_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Application/Source/%.d: ../Application/Source/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


