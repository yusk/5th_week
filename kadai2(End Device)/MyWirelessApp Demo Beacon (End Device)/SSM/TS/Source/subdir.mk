################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../SSM/TS/Source/TS_kernel.c" \

C_SRCS += \
../SSM/TS/Source/TS_kernel.c \

OBJS += \
./SSM/TS/Source/TS_kernel_c.obj \

OBJS_QUOTED += \
"./SSM/TS/Source/TS_kernel_c.obj" \

C_DEPS += \
./SSM/TS/Source/TS_kernel_c.d \

C_DEPS_QUOTED += \
"./SSM/TS/Source/TS_kernel_c.d" \

OBJS_OS_FORMAT += \
./SSM/TS/Source/TS_kernel_c.obj \


# Each subdirectory must supply rules for building sources it contributes
SSM/TS/Source/TS_kernel_c.obj: ../SSM/TS/Source/TS_kernel.c pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"SSM/TS/Source/TS_kernel.args" -ObjN="SSM/TS/Source/TS_kernel_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

SSM/TS/Source/%.d: ../SSM/TS/Source/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


