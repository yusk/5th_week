################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 45
first : all
-include ../makefile.init

# This file contains definitions of environment variables used in the makefiles and .args files if exist.
-include makefile.local

RM := "$(GNU_Make_Install_DirEnv)/rm" -f

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include SSM/ZTC/Source/subdir.mk
-include SSM/TS/Source/subdir.mk
-include PLM/Source/UART/subdir.mk
-include PLM/Source/Touchpad/subdir.mk
-include PLM/Source/TempSensor/subdir.mk
-include PLM/Source/TMR/subdir.mk
-include PLM/Source/Sound/subdir.mk
-include PLM/Source/Sensor/subdir.mk
-include PLM/Source/SPI/subdir.mk
-include PLM/Source/RST/subdir.mk
-include PLM/Source/NVM/subdir.mk
-include PLM/Source/Led/subdir.mk
-include PLM/Source/LPM/subdir.mk
-include PLM/Source/Keyboard/subdir.mk
-include PLM/Source/IIC/subdir.mk
-include PLM/Source/FunctionLib/subdir.mk
-include PLM/Source/Display/subdir.mk
-include PLM/Source/Common/Sys/subdir.mk
-include PLM/Source/CMT/subdir.mk
-include PLM/Source/Accelerometer/subdir.mk
-include MacPhy/Phy/MC1323X/subdir.mk
-include Application/UartUtil/subdir.mk
-include Application/Source/subdir.mk
-include Application/Init/subdir.mk
-include Application/Configure/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \

EXECUTABLES_QUOTED += \

EXECUTABLES_OS_FORMAT += \

BURNER_OUTPUT_OUTPUTS += \

BURNER_OUTPUT_OUTPUTS_QUOTED += \

BURNER_OUTPUT_OUTPUTS_OS_FORMAT += \


# All Target
call-burner := 0
ifneq ($(strip $(EXECUTABLES)),)
ifneq ($(strip $(BBL_SRCS_QUOTED)),)
call-burner := 1
endif
endif
ifeq ($(call-burner),1)
all: pre-build main-build

# Main-build Target
main-build: MyWirelessApp\ Demo\ Beacon\ (Coordinator).abs MyWirelessApp\ Demo\ Beacon\ (Coordinator).abs.s19
else
all: pre-build main-build

# Main-build Target
main-build: MyWirelessApp\ Demo\ Beacon\ (Coordinator).abs
endif

# Tool invocations
MyWirelessApp\ Demo\ Beacon\ (Coordinator).abs: $(OBJS) $(USER_OBJS) pre.prm | pre-build
	@echo 'Building target: $@'
	@echo 'Executing target #46 $@'
	@echo 'Invoking: Linker'
	"$(HC08ToolsEnv)/linker" -ArgFile"MyWirelessApp Demo Beacon (Coordinator).args" -O"MyWirelessApp Demo Beacon (Coordinator).abs"
	@echo 'Finished building target: $@'
	@echo ' '

MyWirelessApp\ Demo\ Beacon\ (Coordinator).abs.s19: $(BBL_SRCS) $(EXECUTABLES) | pre-build
	@echo 'Executing target #47 $@'
	@echo 'Invoking: Burner'
	"$(HC08ToolsEnv)/burner" -ArgFile"MyWirelessApp Demo Beacon (Coordinator).abs.args" -f="$<" -env"ABS_FILE=$(strip $(EXECUTABLES_OS_FORMAT))"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) "./*/*/*.obj" "./*/*/*/*.obj" "./*/*/*/*/*.obj"  $(CPP_DEPS_QUOTED) $(CC_DEPS_QUOTED) $(CXX_DEPS_QUOTED) $(BURNER_OUTPUT_OUTPUTS_QUOTED) $(C_UPPER_DEPS_QUOTED) $(C++_DEPS_QUOTED) $(EXECUTABLES_QUOTED) "./*/*/*.d" "./*/*/*/*.d" "./*/*/*/*/*.d"  $(ProjDirPath)/MyWirelessApp Demo Beacon (Coordinator)/MyWirelessApp Demo Beacon (Coordinator).abs $(ProjDirPath)/MyWirelessApp Demo Beacon (Coordinator)/MyWirelessApp Demo Beacon (Coordinator).abs $(ProjDirPath)/MyWirelessApp Demo Beacon (Coordinator)/MyWirelessApp Demo Beacon (Coordinator).abs.args $(ProjDirPath)/MyWirelessApp Demo Beacon (Coordinator)/MyWirelessApp Demo Beacon (Coordinator).args $(ProjDirPath)/MyWirelessApp Demo Beacon (Coordinator)/MyWirelessApp Demo Beacon (Coordinator).map $(ProjDirPath)/MyWirelessApp Demo Beacon (Coordinator)/MyWirelessApp Demo Beacon (Coordinator).s19
	-@echo ' '

pre-build:
	-@echo 'Precompile the .prm file'
	-"C:\Freescale\CW MCU v10.3\eclipse\../MCU/prog/chc08.exe" -ArgFile"PLM\Source\Common\Sys\crt0.args" "C:/Users/kawasaki/codewarrior_workspace/5th week/kadai2(Coordinator)\PLM\PRM\Linker_MC1323x_far_banked.prm" -LpX -Lp=pre.prm
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
