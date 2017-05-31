################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../SSM/ZTC/Source/ZtcClientCommunication.c" \
"../SSM/ZTC/Source/ZtcCopyData.c" \
"../SSM/ZTC/Source/ZtcIBManagement.c" \
"../SSM/ZTC/Source/ZtcMain.c" \
"../SSM/ZTC/Source/ZtcMsgTypeInfo.c" \
"../SSM/ZTC/Source/ZtcSAPHandlerInfo.c" \

C_SRCS += \
../SSM/ZTC/Source/ZtcClientCommunication.c \
../SSM/ZTC/Source/ZtcCopyData.c \
../SSM/ZTC/Source/ZtcIBManagement.c \
../SSM/ZTC/Source/ZtcMain.c \
../SSM/ZTC/Source/ZtcMsgTypeInfo.c \
../SSM/ZTC/Source/ZtcSAPHandlerInfo.c \

OBJS += \
./SSM/ZTC/Source/ZtcClientCommunication_c.obj \
./SSM/ZTC/Source/ZtcCopyData_c.obj \
./SSM/ZTC/Source/ZtcIBManagement_c.obj \
./SSM/ZTC/Source/ZtcMain_c.obj \
./SSM/ZTC/Source/ZtcMsgTypeInfo_c.obj \
./SSM/ZTC/Source/ZtcSAPHandlerInfo_c.obj \

OBJS_QUOTED += \
"./SSM/ZTC/Source/ZtcClientCommunication_c.obj" \
"./SSM/ZTC/Source/ZtcCopyData_c.obj" \
"./SSM/ZTC/Source/ZtcIBManagement_c.obj" \
"./SSM/ZTC/Source/ZtcMain_c.obj" \
"./SSM/ZTC/Source/ZtcMsgTypeInfo_c.obj" \
"./SSM/ZTC/Source/ZtcSAPHandlerInfo_c.obj" \

C_DEPS += \
./SSM/ZTC/Source/ZtcClientCommunication_c.d \
./SSM/ZTC/Source/ZtcCopyData_c.d \
./SSM/ZTC/Source/ZtcIBManagement_c.d \
./SSM/ZTC/Source/ZtcMain_c.d \
./SSM/ZTC/Source/ZtcMsgTypeInfo_c.d \
./SSM/ZTC/Source/ZtcSAPHandlerInfo_c.d \

C_DEPS_QUOTED += \
"./SSM/ZTC/Source/ZtcClientCommunication_c.d" \
"./SSM/ZTC/Source/ZtcCopyData_c.d" \
"./SSM/ZTC/Source/ZtcIBManagement_c.d" \
"./SSM/ZTC/Source/ZtcMain_c.d" \
"./SSM/ZTC/Source/ZtcMsgTypeInfo_c.d" \
"./SSM/ZTC/Source/ZtcSAPHandlerInfo_c.d" \

OBJS_OS_FORMAT += \
./SSM/ZTC/Source/ZtcClientCommunication_c.obj \
./SSM/ZTC/Source/ZtcCopyData_c.obj \
./SSM/ZTC/Source/ZtcIBManagement_c.obj \
./SSM/ZTC/Source/ZtcMain_c.obj \
./SSM/ZTC/Source/ZtcMsgTypeInfo_c.obj \
./SSM/ZTC/Source/ZtcSAPHandlerInfo_c.obj \


# Each subdirectory must supply rules for building sources it contributes
SSM/ZTC/Source/ZtcClientCommunication_c.obj: ../SSM/ZTC/Source/ZtcClientCommunication.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"SSM/ZTC/Source/ZtcClientCommunication.args" -ObjN="SSM/ZTC/Source/ZtcClientCommunication_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

SSM/ZTC/Source/%.d: ../SSM/ZTC/Source/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

SSM/ZTC/Source/ZtcCopyData_c.obj: ../SSM/ZTC/Source/ZtcCopyData.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"SSM/ZTC/Source/ZtcCopyData.args" -ObjN="SSM/ZTC/Source/ZtcCopyData_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

SSM/ZTC/Source/ZtcIBManagement_c.obj: ../SSM/ZTC/Source/ZtcIBManagement.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"SSM/ZTC/Source/ZtcIBManagement.args" -ObjN="SSM/ZTC/Source/ZtcIBManagement_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

SSM/ZTC/Source/ZtcMain_c.obj: ../SSM/ZTC/Source/ZtcMain.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"SSM/ZTC/Source/ZtcMain.args" -ObjN="SSM/ZTC/Source/ZtcMain_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

SSM/ZTC/Source/ZtcMsgTypeInfo_c.obj: ../SSM/ZTC/Source/ZtcMsgTypeInfo.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"SSM/ZTC/Source/ZtcMsgTypeInfo.args" -ObjN="SSM/ZTC/Source/ZtcMsgTypeInfo_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

SSM/ZTC/Source/ZtcSAPHandlerInfo_c.obj: ../SSM/ZTC/Source/ZtcSAPHandlerInfo.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"SSM/ZTC/Source/ZtcSAPHandlerInfo.args" -ObjN="SSM/ZTC/Source/ZtcSAPHandlerInfo_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '


