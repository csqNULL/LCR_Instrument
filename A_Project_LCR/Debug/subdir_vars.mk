################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
OPT_SRCS += \
../device.opt 

SYSCFG_SRCS += \
../uart_tx_multibyte_fifo_dma_interrupts.syscfg 

C_SRCS += \
../delay.c \
../fir.c \
../iic.c \
../key.c \
../main.c \
../measure.c \
../my_filter.c \
../oled.c \
../ti_msp_dl_config.c \
../tjc_usart_hmi.c \
./ti_msp_dl_config.c \
C:/ti/mspm0_sdk_1_30_00_03/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c 

GEN_CMDS += \
./device_linker.cmd 

GEN_FILES += \
./device_linker.cmd \
./device.opt \
./ti_msp_dl_config.c 

C_DEPS += \
./delay.d \
./fir.d \
./iic.d \
./key.d \
./main.d \
./measure.d \
./my_filter.d \
./oled.d \
./ti_msp_dl_config.d \
./tjc_usart_hmi.d \
./ti_msp_dl_config.d \
./startup_mspm0g350x_ticlang.d 

GEN_OPTS += \
./device.opt 

OBJS += \
./delay.o \
./fir.o \
./iic.o \
./key.o \
./main.o \
./measure.o \
./my_filter.o \
./oled.o \
./ti_msp_dl_config.o \
./tjc_usart_hmi.o \
./startup_mspm0g350x_ticlang.o 

GEN_MISC_FILES += \
./device.cmd.genlibs \
./ti_msp_dl_config.h \
./Event.dot 

OBJS__QUOTED += \
"delay.o" \
"fir.o" \
"iic.o" \
"key.o" \
"main.o" \
"measure.o" \
"my_filter.o" \
"oled.o" \
"ti_msp_dl_config.o" \
"tjc_usart_hmi.o" \
"startup_mspm0g350x_ticlang.o" 

GEN_MISC_FILES__QUOTED += \
"device.cmd.genlibs" \
"ti_msp_dl_config.h" \
"Event.dot" 

C_DEPS__QUOTED += \
"delay.d" \
"fir.d" \
"iic.d" \
"key.d" \
"main.d" \
"measure.d" \
"my_filter.d" \
"oled.d" \
"ti_msp_dl_config.d" \
"tjc_usart_hmi.d" \
"ti_msp_dl_config.d" \
"startup_mspm0g350x_ticlang.d" 

GEN_FILES__QUOTED += \
"device_linker.cmd" \
"device.opt" \
"ti_msp_dl_config.c" 

C_SRCS__QUOTED += \
"../delay.c" \
"../fir.c" \
"../iic.c" \
"../key.c" \
"../main.c" \
"../measure.c" \
"../my_filter.c" \
"../oled.c" \
"../ti_msp_dl_config.c" \
"../tjc_usart_hmi.c" \
"./ti_msp_dl_config.c" \
"C:/ti/mspm0_sdk_1_30_00_03/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c" 

OPT_SRCS__QUOTED += \
"../device.opt" 

SYSCFG_SRCS__QUOTED += \
"../uart_tx_multibyte_fifo_dma_interrupts.syscfg" 


