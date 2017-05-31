################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../PLM/Source/SPI/SPI.c" \

C_SRCS += \
../PLM/Source/SPI/SPI.c \

OBJS += \
./PLM/Source/SPI/SPI_c.obj \

OBJS_QUOTED += \
"./PLM/Source/SPI/SPI_c.obj" \

C_DEPS += \
./PLM/Source/SPI/SPI_c.d \

C_DEPS_QUOTED += \
"./PLM/Source/SPI/SPI_c.d" \

OBJS_OS_FORMAT += \
./PLM/Source/SPI/SPI_c.obj \


# Each subdirectory must supply rules for building sources it contributes
PLM/Source/SPI/SPI_c.obj: ../PLM/Source/SPI/SPI.c | pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #18 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/Source/SPI/SPI.args" -ObjN="PLM/Source/SPI/SPI_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/Source/SPI/%.d: ../PLM/Source/SPI/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


