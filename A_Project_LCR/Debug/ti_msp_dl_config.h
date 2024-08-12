/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)



#define CPUCLK_FREQ                                                     32000000



/* Defines for TIMER_0 */
#define TIMER_0_INST                                                     (TIMG0)
#define TIMER_0_INST_IRQHandler                                 TIMG0_IRQHandler
#define TIMER_0_INST_INT_IRQN                                   (TIMG0_INT_IRQn)
#define TIMER_0_INST_LOAD_VALUE                                          (3839U)
/* Defines for TIMER_1 */
#define TIMER_1_INST                                                     (TIMG8)
#define TIMER_1_INST_IRQHandler                                 TIMG8_IRQHandler
#define TIMER_1_INST_INT_IRQN                                   (TIMG8_INT_IRQn)
#define TIMER_1_INST_LOAD_VALUE                                         (49999U)



/* Defines for UART_0 */
#define UART_0_INST                                                        UART0
#define UART_0_INST_IRQHandler                                  UART0_IRQHandler
#define UART_0_INST_INT_IRQN                                      UART0_INT_IRQn
#define GPIO_UART_0_RX_PORT                                                GPIOA
#define GPIO_UART_0_TX_PORT                                                GPIOA
#define GPIO_UART_0_RX_PIN                                        DL_GPIO_PIN_11
#define GPIO_UART_0_TX_PIN                                        DL_GPIO_PIN_10
#define GPIO_UART_0_IOMUX_RX                                     (IOMUX_PINCM22)
#define GPIO_UART_0_IOMUX_TX                                     (IOMUX_PINCM21)
#define GPIO_UART_0_IOMUX_RX_FUNC                      IOMUX_PINCM22_PF_UART0_RX
#define GPIO_UART_0_IOMUX_TX_FUNC                      IOMUX_PINCM21_PF_UART0_TX
#define UART_0_BAUD_RATE                                                (115200)
#define UART_0_IBRD_32_MHZ_115200_BAUD                                      (17)
#define UART_0_FBRD_32_MHZ_115200_BAUD                                      (23)
/* Defines for UART_1 */
#define UART_1_INST                                                        UART1
#define UART_1_INST_IRQHandler                                  UART1_IRQHandler
#define UART_1_INST_INT_IRQN                                      UART1_INT_IRQn
#define GPIO_UART_1_RX_PORT                                                GPIOA
#define GPIO_UART_1_TX_PORT                                                GPIOA
#define GPIO_UART_1_RX_PIN                                         DL_GPIO_PIN_9
#define GPIO_UART_1_TX_PIN                                         DL_GPIO_PIN_8
#define GPIO_UART_1_IOMUX_RX                                     (IOMUX_PINCM20)
#define GPIO_UART_1_IOMUX_TX                                     (IOMUX_PINCM19)
#define GPIO_UART_1_IOMUX_RX_FUNC                      IOMUX_PINCM20_PF_UART1_RX
#define GPIO_UART_1_IOMUX_TX_FUNC                      IOMUX_PINCM19_PF_UART1_TX
#define UART_1_BAUD_RATE                                                (115200)
#define UART_1_IBRD_32_MHZ_115200_BAUD                                      (17)
#define UART_1_FBRD_32_MHZ_115200_BAUD                                      (23)





/* Defines for ADC12_0 */
#define ADC12_0_INST                                                        ADC0
#define ADC12_0_INST_IRQHandler                                  ADC0_IRQHandler
#define ADC12_0_INST_INT_IRQN                                    (ADC0_INT_IRQn)
#define ADC12_0_ADCMEM_0                                      DL_ADC12_MEM_IDX_0
#define ADC12_0_ADCMEM_0_REF                     DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADC12_0_ADCMEM_0_REF_VOLTAGE                                          -1 // VDDA cannot be determined
#define ADC12_0_ADCMEM_1                                      DL_ADC12_MEM_IDX_1
#define ADC12_0_ADCMEM_1_REF                     DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADC12_0_ADCMEM_1_REF_VOLTAGE                                          -1 // VDDA cannot be determined

/* Defines for ADC12_1 */
#define ADC12_1_INST                                                        ADC1
#define ADC12_1_INST_IRQHandler                                  ADC1_IRQHandler
#define ADC12_1_INST_INT_IRQN                                    (ADC1_INT_IRQn)
#define ADC12_1_ADCMEM_0                                      DL_ADC12_MEM_IDX_0
#define ADC12_1_ADCMEM_0_REF                     DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADC12_1_ADCMEM_0_REF_VOLTAGE                                          -1 // VDDA cannot be determined
#define GPIO_ADC12_1_C2_PORT                                               GPIOA
#define GPIO_ADC12_1_C2_PIN                                       DL_GPIO_PIN_17


/* Defines for VREF */
#define VREF_VOLTAGE_MV                                                     1400
#define GPIO_VREF_VREFPOS_PORT                                             GPIOA
#define GPIO_VREF_VREFPOS_PIN                                     DL_GPIO_PIN_23
#define GPIO_VREF_IOMUX_VREFPOS                                  (IOMUX_PINCM53)
#define GPIO_VREF_IOMUX_VREFPOS_FUNC                IOMUX_PINCM53_PF_UNCONNECTED



/* Defines for OPA_0 */
#define OPA_0_INST                                                          OPA0
#define GPIO_OPA_0_IN1POS_PORT                                             GPIOA
#define GPIO_OPA_0_IN1POS_PIN                                     DL_GPIO_PIN_25
#define GPIO_OPA_0_IOMUX_IN1POS                                  (IOMUX_PINCM55)
#define GPIO_OPA_0_IOMUX_IN1POS_FUNC                IOMUX_PINCM55_PF_UNCONNECTED
#define GPIO_OPA_0_IN1NEG_PORT                                             GPIOA
#define GPIO_OPA_0_IN1NEG_PIN                                     DL_GPIO_PIN_24
#define GPIO_OPA_0_IOMUX_IN1NEG                                  (IOMUX_PINCM54)
#define GPIO_OPA_0_IOMUX_IN1NEG_FUNC                IOMUX_PINCM54_PF_UNCONNECTED
/* Defines for OPA_1 */
#define OPA_1_INST                                                          OPA1
#define GPIO_OPA_1_IN1POS_PORT                                             GPIOA
#define GPIO_OPA_1_IN1POS_PIN                                     DL_GPIO_PIN_18
#define GPIO_OPA_1_IOMUX_IN1POS                                  (IOMUX_PINCM40)
#define GPIO_OPA_1_IOMUX_IN1POS_FUNC                IOMUX_PINCM40_PF_UNCONNECTED
#define GPIO_OPA_1_OUT_PORT                                                GPIOA
#define GPIO_OPA_1_OUT_PIN                                        DL_GPIO_PIN_16
#define GPIO_OPA_1_IOMUX_OUT                                     (IOMUX_PINCM38)
#define GPIO_OPA_1_IOMUX_OUT_FUNC                   IOMUX_PINCM38_PF_UNCONNECTED


/* Defines for GPAMP_0 */
#define GPIO_GPAMP_0_INPOS_PORT                                            GPIOA
#define GPIO_GPAMP_0_INPOS_PIN                                    DL_GPIO_PIN_26
#define GPIO_GPAMP_0_IOMUX_INPOS                                 (IOMUX_PINCM59)
#define GPIO_GPAMP_0_IOMUX_INPOS_FUNC               IOMUX_PINCM59_PF_UNCONNECTED



/* Defines for DMA_CH1 */
#define DMA_CH1_CHAN_ID                                                      (2)
#define DAC12_INST_DMA_TRIGGER                          (DMA_DAC0_EVT_BD_1_TRIG)

/* Defines for DMA_CH0 */
#define DMA_CH0_CHAN_ID                                                      (0)
#define UART_0_INST_DMA_TRIGGER_0                            (DMA_UART0_TX_TRIG)

/* Defines for DMA_CH2 */
#define DMA_CH2_CHAN_ID                                                      (1)
#define UART_0_INST_DMA_TRIGGER_1                            (DMA_UART0_RX_TRIG)



/* Port definition for Pin Group GPIO_KEY */
#define GPIO_KEY_PORT                                                    (GPIOB)

/* Defines for key0: GPIOB.16 with pinCMx 33 on package pin 4 */
// pins affected by this interrupt request:["key0"]
#define GPIO_KEY_INT_IRQN                                       (GPIOB_INT_IRQn)
#define GPIO_KEY_INT_IIDX                       (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
#define GPIO_KEY_key0_IIDX                                  (DL_GPIO_IIDX_DIO16)
#define GPIO_KEY_key0_PIN                                       (DL_GPIO_PIN_16)
#define GPIO_KEY_key0_IOMUX                                      (IOMUX_PINCM33)
/* Port definition for Pin Group GPIO_Relay */
#define GPIO_Relay_PORT                                                  (GPIOB)

/* Defines for PIN_2: GPIOB.7 with pinCMx 24 on package pin 59 */
#define GPIO_Relay_PIN_2_PIN                                     (DL_GPIO_PIN_7)
#define GPIO_Relay_PIN_2_IOMUX                                   (IOMUX_PINCM24)
/* Defines for USER_LED_1: GPIOA.0 with pinCMx 1 on package pin 33 */
#define GPIO_LEDS_USER_LED_1_PORT                                        (GPIOA)
#define GPIO_LEDS_USER_LED_1_PIN                                 (DL_GPIO_PIN_0)
#define GPIO_LEDS_USER_LED_1_IOMUX                                (IOMUX_PINCM1)
/* Defines for LED_Sleep: GPIOB.4 with pinCMx 17 on package pin 52 */
#define GPIO_LEDS_LED_Sleep_PORT                                         (GPIOB)
#define GPIO_LEDS_LED_Sleep_PIN                                  (DL_GPIO_PIN_4)
#define GPIO_LEDS_LED_Sleep_IOMUX                                (IOMUX_PINCM17)
/* Port definition for Pin Group GPIO_IIC */
#define GPIO_IIC_PORT                                                    (GPIOB)

/* Defines for SCL: GPIOB.13 with pinCMx 30 on package pin 1 */
#define GPIO_IIC_SCL_PIN                                        (DL_GPIO_PIN_13)
#define GPIO_IIC_SCL_IOMUX                                       (IOMUX_PINCM30)
/* Defines for SDA: GPIOB.15 with pinCMx 32 on package pin 3 */
#define GPIO_IIC_SDA_PIN                                        (DL_GPIO_PIN_15)
#define GPIO_IIC_SDA_IOMUX                                       (IOMUX_PINCM32)
/* Port definition for Pin Group GPIO_Measure */
#define GPIO_Measure_PORT                                                (GPIOA)

/* Defines for PIN_High: GPIOA.13 with pinCMx 35 on package pin 6 */
#define GPIO_Measure_PIN_High_PIN                               (DL_GPIO_PIN_13)
#define GPIO_Measure_PIN_High_IOMUX                              (IOMUX_PINCM35)
/* Defines for PIN_Low: GPIOA.12 with pinCMx 34 on package pin 5 */
#define GPIO_Measure_PIN_Low_PIN                                (DL_GPIO_PIN_12)
#define GPIO_Measure_PIN_Low_IOMUX                               (IOMUX_PINCM34)
/* Port definition for Pin Group GPIO_SW */
#define GPIO_SW_PORT                                                     (GPIOB)

/* Defines for PIN_SW1: GPIOB.2 with pinCMx 15 on package pin 50 */
#define GPIO_SW_PIN_SW1_PIN                                      (DL_GPIO_PIN_2)
#define GPIO_SW_PIN_SW1_IOMUX                                    (IOMUX_PINCM15)
/* Defines for PIN_SW0: GPIOB.3 with pinCMx 16 on package pin 51 */
#define GPIO_SW_PIN_SW0_PIN                                      (DL_GPIO_PIN_3)
#define GPIO_SW_PIN_SW0_IOMUX                                    (IOMUX_PINCM16)



/* Defines for DAC12 */
#define DAC12_IRQHandler                                         DAC0_IRQHandler
#define DAC12_INT_IRQN                                           (DAC0_INT_IRQn)
#define GPIO_DAC12_OUT_PORT                                                GPIOA
#define GPIO_DAC12_OUT_PIN                                        DL_GPIO_PIN_15
#define GPIO_DAC12_IOMUX_OUT                                     (IOMUX_PINCM37)
#define GPIO_DAC12_IOMUX_OUT_FUNC                   IOMUX_PINCM37_PF_UNCONNECTED



/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_TIMER_0_init(void);
void SYSCFG_DL_TIMER_1_init(void);
void SYSCFG_DL_UART_0_init(void);
void SYSCFG_DL_UART_1_init(void);
void SYSCFG_DL_ADC12_0_init(void);
void SYSCFG_DL_ADC12_1_init(void);
void SYSCFG_DL_VREF_init(void);
void SYSCFG_DL_OPA_0_init(void);
void SYSCFG_DL_OPA_1_init(void);
void SYSCFG_DL_GPAMP_0_init(void);
void SYSCFG_DL_DMA_init(void);

void SYSCFG_DL_DAC12_init(void);


bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
