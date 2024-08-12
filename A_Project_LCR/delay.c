#include "delay.h"
#include "ti_msp_dl_config.h"


void delay_us(unsigned t) { delay_cycles(CPUCLK_FREQ / 1000000 * (t)); }

void delay_ms(unsigned t) { delay_cycles(CPUCLK_FREQ / 1000 * (t)); }
