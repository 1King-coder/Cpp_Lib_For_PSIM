################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs2051/ccs/tools/compiler/ti-cgt-c2000_25.11.0.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./main.obj" \
"./TI/F2837xD_Adc.obj" \
"./TI/F2837xD_CodeStartBranch.obj" \
"./TI/F2837xD_CpuTimers.obj" \
"./TI/F2837xD_DBGIER.obj" \
"./TI/F2837xD_DefaultISR.obj" \
"./TI/F2837xD_Dma.obj" \
"./TI/F2837xD_ECap.obj" \
"./TI/F2837xD_EPwm.obj" \
"./TI/F2837xD_EQep.obj" \
"./TI/F2837xD_Emif.obj" \
"./TI/F2837xD_GlobalVariableDefs.obj" \
"./TI/F2837xD_Gpio.obj" \
"./TI/F2837xD_I2C.obj" \
"./TI/F2837xD_Ipc.obj" \
"./TI/F2837xD_Ipc_Driver.obj" \
"./TI/F2837xD_Ipc_Driver_Lite.obj" \
"./TI/F2837xD_Ipc_Driver_Util.obj" \
"./TI/F2837xD_Mcbsp.obj" \
"./TI/F2837xD_PieCtrl.obj" \
"./TI/F2837xD_PieVect.obj" \
"./TI/F2837xD_Sci.obj" \
"./TI/F2837xD_Spi.obj" \
"./TI/F2837xD_SysCtrl.obj" \
"./TI/F2837xD_TempSensorConv.obj" \
"./TI/F2837xD_Upp.obj" \
"./TI/F2837xD_can.obj" \
"./TI/F2837xD_sci_io.obj" \
"./TI/F2837xD_sdfm_drivers.obj" \
"./TI/F2837xD_struct.obj" \
"./TI/F2837xD_usDelay.obj" \
"./source/BLDCDriveAndControlProject.obj" \
"./source/BLDC_basis.obj" \
"./source/control_logics.obj" \
"./source/ecap.obj" \
"./source/global_definitions.obj" \
"./source/modbus.obj" \
"./source/pwm.obj" \
"./source/sci_com.obj" \
"../2837xD_RAM_lnk_cpu1.cmd" \
"../F2837xD_Headers_nonBIOS_cpu1.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include TI/subdir_vars.mk
-include source/subdir_vars.mk
-include subdir_rules.mk
-include TI/subdir_rules.mk
-include source/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
PROJETO-CTRL-BLDC-EEE614.out 

EXE_OUTPUTS__QUOTED += \
"PROJETO-CTRL-BLDC-EEE614.out" 


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "PROJETO-CTRL-BLDC-EEE614.out"

# Tool invocations
PROJETO-CTRL-BLDC-EEE614.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'C2000 Linker - building target: "$@"'
	"C:/ti/ccs2051/ccs/tools/compiler/ti-cgt-c2000_25.11.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --define=DEBUG --define=_DUAL_HEADERS --define=CPU1 --define=_LAUNCHXL_F28379D --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi -z -m"PROJETO-CTRL-BLDC-EEE614.map" --stack_size=0x100 --warn_sections -i"C:/ti/ccs2051/ccs/tools/compiler/ti-cgt-c2000_25.11.0.LTS/lib" -i"C:/ti/ccs2051/ccs/tools/compiler/ti-cgt-c2000_25.11.0.LTS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="PROJETO-CTRL-BLDC-EEE614_linkInfo.xml" --rom_model -o "PROJETO-CTRL-BLDC-EEE614.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "main.obj" "TI\F2837xD_Adc.obj" "TI\F2837xD_CodeStartBranch.obj" "TI\F2837xD_CpuTimers.obj" "TI\F2837xD_DBGIER.obj" "TI\F2837xD_DefaultISR.obj" "TI\F2837xD_Dma.obj" "TI\F2837xD_ECap.obj" "TI\F2837xD_EPwm.obj" "TI\F2837xD_EQep.obj" "TI\F2837xD_Emif.obj" "TI\F2837xD_GlobalVariableDefs.obj" "TI\F2837xD_Gpio.obj" "TI\F2837xD_I2C.obj" "TI\F2837xD_Ipc.obj" "TI\F2837xD_Ipc_Driver.obj" "TI\F2837xD_Ipc_Driver_Lite.obj" "TI\F2837xD_Ipc_Driver_Util.obj" "TI\F2837xD_Mcbsp.obj" "TI\F2837xD_PieCtrl.obj" "TI\F2837xD_PieVect.obj" "TI\F2837xD_Sci.obj" "TI\F2837xD_Spi.obj" "TI\F2837xD_SysCtrl.obj" "TI\F2837xD_TempSensorConv.obj" "TI\F2837xD_Upp.obj" "TI\F2837xD_can.obj" "TI\F2837xD_sci_io.obj" "TI\F2837xD_sdfm_drivers.obj" "TI\F2837xD_struct.obj" "TI\F2837xD_usDelay.obj" "source\BLDCDriveAndControlProject.obj" "source\BLDC_basis.obj" "source\control_logics.obj" "source\ecap.obj" "source\global_definitions.obj" "source\modbus.obj" "source\pwm.obj" "source\sci_com.obj" 
	-$(RM) "TI\F2837xD_Adc.d" "TI\F2837xD_CpuTimers.d" "TI\F2837xD_DefaultISR.d" "TI\F2837xD_Dma.d" "TI\F2837xD_ECap.d" "TI\F2837xD_EPwm.d" "TI\F2837xD_EQep.d" "TI\F2837xD_Emif.d" "TI\F2837xD_GlobalVariableDefs.d" "TI\F2837xD_Gpio.d" "TI\F2837xD_I2C.d" "TI\F2837xD_Ipc.d" "TI\F2837xD_Ipc_Driver.d" "TI\F2837xD_Ipc_Driver_Lite.d" "TI\F2837xD_Ipc_Driver_Util.d" "TI\F2837xD_Mcbsp.d" "TI\F2837xD_PieCtrl.d" "TI\F2837xD_PieVect.d" "TI\F2837xD_Sci.d" "TI\F2837xD_Spi.d" "TI\F2837xD_SysCtrl.d" "TI\F2837xD_TempSensorConv.d" "TI\F2837xD_Upp.d" "TI\F2837xD_can.d" "TI\F2837xD_sci_io.d" "TI\F2837xD_sdfm_drivers.d" "TI\F2837xD_struct.d" 
	-$(RM) "main.d" "source\BLDCDriveAndControlProject.d" "source\BLDC_basis.d" "source\control_logics.d" "source\ecap.d" "source\global_definitions.d" "source\modbus.d" "source\pwm.d" "source\sci_com.d" 
	-$(RM) "TI\F2837xD_CodeStartBranch.d" "TI\F2837xD_DBGIER.d" "TI\F2837xD_usDelay.d" 
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

