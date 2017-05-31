################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../PLM/Source/NVM/NV_Flash.c" \
"../PLM/Source/NVM/NV_FlashHAL.c" \

C_SRCS += \
../PLM/Source/NVM/NV_Flash.c \
../PLM/Source/NVM/NV_FlashHAL.c \

OBJS += \
./PLM/Source/NVM/NV_Flash_c.obj \
./PLM/Source/NVM/NV_FlashHAL_c.obj \

OBJS_QUOTED += \
"./PLM/Source/NVM/NV_Flash_c.obj" \
"./PLM/Source/NVM/NV_FlashHAL_c.obj" \

C_DEPS += \
./PLM/Source/NVM/NV_Flash_c.d \
./PLM/Source/NVM/NV_FlashHAL_c.d \

C_DEPS_QUOTED += \
"./PLM/Source/NVM/NV_Flash_c.d" \
"./PLM/Source/NVM/NV_FlashHAL_c.d" \

OBJS_OS_FORMAT += \
./PLM/Source/NVM/NV_Flash_c.obj \
./PLM/Source/NVM/NV_FlashHAL_c.obj \


# Each subdirectory must supply rules for building sources it contributes
PLM/Source/NVM/NV_Flash_c.obj: ../PLM/Source/NVM/NV_Flash.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #20 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/NVM/NV_Flash.args" -ObjN="PLM/Source/NVM/NV_Flash_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/Source/NVM/%.d: ../PLM/Source/NVM/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

PLM/Source/NVM/NV_FlashHAL_c.obj: ../PLM/Source/NVM/NV_FlashHAL.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #21 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/NVM/NV_FlashHAL.args" -ObjN="PLM/Source/NVM/NV_FlashHAL_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '


