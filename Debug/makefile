################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.3.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./main.obj" \
"./startup_msp432p401r_ccs.obj" \
"./system_msp432p401r.obj" \
"./drivers/eMPL/inv_mpu.obj" \
"./drivers/eMPL/inv_mpu_dmp_motion_driver.obj" \
"./eMPL-hal/eMPL_outputs.obj" \
"./mllite/data_builder.obj" \
"./mllite/hal_outputs.obj" \
"./mllite/message_layer.obj" \
"./mllite/ml_math_func.obj" \
"./mllite/mlmath.obj" \
"./mllite/mpl.obj" \
"./mllite/results_holder.obj" \
"./mllite/start_manager.obj" \
"./mllite/storage_manager.obj" \
"./msp432Peripherals/mpuUart/log_msp432.obj" \
"./msp432Peripherals/mpuUart/mpu_Uart.obj" \
"./msp432Peripherals/mpu_setup.obj" \
"./msp432Peripherals/msp432_Clock.obj" \
"./msp432Peripherals/msp432_I2C.obj" \
"./msp432Peripherals/msp432_Interrupt.obj" \
"../driverlib_empty_project_ccs.cmd" \
"C:/ti/msp/MSP432Ware_3_50_00_02/driverlib/driverlib/MSP432P4xx/ccs/msp432p4xx_driverlib.lib" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include drivers/eMPL/subdir_vars.mk
-include eMPL-hal/subdir_vars.mk
-include mllite/subdir_vars.mk
-include mpl/subdir_vars.mk
-include msp432Peripherals/mpuUart/subdir_vars.mk
-include msp432Peripherals/subdir_vars.mk
-include subdir_rules.mk
-include drivers/eMPL/subdir_rules.mk
-include eMPL-hal/subdir_rules.mk
-include mllite/subdir_rules.mk
-include mpl/subdir_rules.mk
-include msp432Peripherals/mpuUart/subdir_rules.mk
-include msp432Peripherals/subdir_rules.mk
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
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
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
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
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
MPU6050_msp432.out \

EXE_OUTPUTS__QUOTED += \
"MPU6050_msp432.out" \

BIN_OUTPUTS += \
MPU6050_msp432.bin \

BIN_OUTPUTS__QUOTED += \
"MPU6050_msp432.bin" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(A_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "MPU6050_msp432.out"

# Tool invocations
MPU6050_msp432.out: $(OBJS) $(CMD_SRCS) $(A_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: MSP432 Linker'
	"D:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --advice:power="all" --define=__MSP432P401R__ --define=MPU6050 --define=MOTION_DRIVER_TARGET_MSP432 --define=TARGET_IS_MSP432P4XX --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number -z -m"MPU6050_msp432.map" --stack_size=512 -i"D:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.3.LTS/lib" -i"D:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.3.LTS/include" --reread_libs --diag_wrap=off --display_error_number --warn_sections --xml_link_info="MPU6050_msp432_linkInfo.xml" --rom_model -o "MPU6050_msp432.out" $(ORDERED_OBJS) $(A_SRCS)
	@echo 'Finished building target: $@'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

MPU6050_msp432.bin: $(EXE_OUTPUTS)
	@echo 'Invoking: MSP432 Hex Utility'
	"D:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.3.LTS/bin/armhex"  -o "MPU6050_msp432.bin" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "main.obj" "startup_msp432p401r_ccs.obj" "system_msp432p401r.obj" "drivers\eMPL\inv_mpu.obj" "drivers\eMPL\inv_mpu_dmp_motion_driver.obj" "eMPL-hal\eMPL_outputs.obj" "mllite\data_builder.obj" "mllite\hal_outputs.obj" "mllite\message_layer.obj" "mllite\ml_math_func.obj" "mllite\mlmath.obj" "mllite\mpl.obj" "mllite\results_holder.obj" "mllite\start_manager.obj" "mllite\storage_manager.obj" "msp432Peripherals\mpuUart\log_msp432.obj" "msp432Peripherals\mpuUart\mpu_Uart.obj" "msp432Peripherals\mpu_setup.obj" "msp432Peripherals\msp432_Clock.obj" "msp432Peripherals\msp432_I2C.obj" "msp432Peripherals\msp432_Interrupt.obj" 
	-$(RM) "main.d" "startup_msp432p401r_ccs.d" "system_msp432p401r.d" "drivers\eMPL\inv_mpu.d" "drivers\eMPL\inv_mpu_dmp_motion_driver.d" "eMPL-hal\eMPL_outputs.d" "mllite\data_builder.d" "mllite\hal_outputs.d" "mllite\message_layer.d" "mllite\ml_math_func.d" "mllite\mlmath.d" "mllite\mpl.d" "mllite\results_holder.d" "mllite\start_manager.d" "mllite\storage_manager.d" "msp432Peripherals\mpuUart\log_msp432.d" "msp432Peripherals\mpuUart\mpu_Uart.d" "msp432Peripherals\mpu_setup.d" "msp432Peripherals\msp432_Clock.d" "msp432Peripherals\msp432_I2C.d" "msp432Peripherals\msp432_Interrupt.d" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-D:/ti/ccsv7/utils/tiobj2bin/tiobj2bin MPU6050_msp432.out MPU6050_msp432.bin D:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.3.LTS/bin/armofd D:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.3.LTS/bin/armhex D:/ti/ccsv7/utils/tiobj2bin/mkhex4bin
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

