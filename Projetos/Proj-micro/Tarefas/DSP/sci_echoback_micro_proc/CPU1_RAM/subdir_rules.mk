################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'C2000 Compiler - building file: "$<"'
	"C:/ti/ccs2050/ccs/tools/compiler/ti-cgt-c2000_25.11.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --include_path="C:/dev/C-FOR-SIM-LIB/Projetos/Proj-micro/Tarefas/DSP/sci_echoback_micro_proc" --include_path="C:/dev/C-FOR-SIM-LIB/Projetos/Proj-micro/Tarefas/DSP/sci_echoback_micro_proc/device" --include_path="C:/ti/C2000Ware_26_00_00_00/driverlib/f2837xd/driverlib/" --include_path="C:/ti/ccs2050/ccs/tools/compiler/ti-cgt-c2000_25.11.0.LTS/include" --define=DEBUG --define=CPU1 --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="C:/dev/C-FOR-SIM-LIB/Projetos/Proj-micro/Tarefas/DSP/sci_echoback_micro_proc/CPU1_RAM/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


