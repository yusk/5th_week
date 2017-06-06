################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../MacPhy/Phy/MC1323X/PhyISR.c" \
"../MacPhy/Phy/MC1323X/PhyPacketProcessor.c" \
"../MacPhy/Phy/MC1323X/PhyPlmeData.c" \
"../MacPhy/Phy/MC1323X/PhyTime.c" \

C_SRCS += \
../MacPhy/Phy/MC1323X/PhyISR.c \
../MacPhy/Phy/MC1323X/PhyPacketProcessor.c \
../MacPhy/Phy/MC1323X/PhyPlmeData.c \
../MacPhy/Phy/MC1323X/PhyTime.c \

OBJS += \
./MacPhy/Phy/MC1323X/PhyISR_c.obj \
./MacPhy/Phy/MC1323X/PhyPacketProcessor_c.obj \
./MacPhy/Phy/MC1323X/PhyPlmeData_c.obj \
./MacPhy/Phy/MC1323X/PhyTime_c.obj \

OBJS_QUOTED += \
"./MacPhy/Phy/MC1323X/PhyISR_c.obj" \
"./MacPhy/Phy/MC1323X/PhyPacketProcessor_c.obj" \
"./MacPhy/Phy/MC1323X/PhyPlmeData_c.obj" \
"./MacPhy/Phy/MC1323X/PhyTime_c.obj" \

C_DEPS += \
./MacPhy/Phy/MC1323X/PhyISR_c.d \
./MacPhy/Phy/MC1323X/PhyPacketProcessor_c.d \
./MacPhy/Phy/MC1323X/PhyPlmeData_c.d \
./MacPhy/Phy/MC1323X/PhyTime_c.d \

C_DEPS_QUOTED += \
"./MacPhy/Phy/MC1323X/PhyISR_c.d" \
"./MacPhy/Phy/MC1323X/PhyPacketProcessor_c.d" \
"./MacPhy/Phy/MC1323X/PhyPlmeData_c.d" \
"./MacPhy/Phy/MC1323X/PhyTime_c.d" \

OBJS_OS_FORMAT += \
./MacPhy/Phy/MC1323X/PhyISR_c.obj \
./MacPhy/Phy/MC1323X/PhyPacketProcessor_c.obj \
./MacPhy/Phy/MC1323X/PhyPlmeData_c.obj \
./MacPhy/Phy/MC1323X/PhyTime_c.obj \


# Each subdirectory must supply rules for building sources it contributes
MacPhy/Phy/MC1323X/PhyISR_c.obj: ../MacPhy/Phy/MC1323X/PhyISR.c pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #37 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"MacPhy/Phy/MC1323X/PhyISR.args" -ObjN="MacPhy/Phy/MC1323X/PhyISR_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

MacPhy/Phy/MC1323X/%.d: ../MacPhy/Phy/MC1323X/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

MacPhy/Phy/MC1323X/PhyPacketProcessor_c.obj: ../MacPhy/Phy/MC1323X/PhyPacketProcessor.c pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #38 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"MacPhy/Phy/MC1323X/PhyPacketProcessor.args" -ObjN="MacPhy/Phy/MC1323X/PhyPacketProcessor_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

MacPhy/Phy/MC1323X/PhyPlmeData_c.obj: ../MacPhy/Phy/MC1323X/PhyPlmeData.c pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #39 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"MacPhy/Phy/MC1323X/PhyPlmeData.args" -ObjN="MacPhy/Phy/MC1323X/PhyPlmeData_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

MacPhy/Phy/MC1323X/PhyTime_c.obj: ../MacPhy/Phy/MC1323X/PhyTime.c pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #40 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"MacPhy/Phy/MC1323X/PhyTime.args" -ObjN="MacPhy/Phy/MC1323X/PhyTime_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '


