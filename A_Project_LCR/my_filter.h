#ifndef __MY_FILTER_H
#define __MY_FILTER_H

#include "ti_msp_dl_config.h"

#define MVF_LENGTH 16
#define vppCompensate 1.1

void moving_average_filter_double(double *data, int length);
void moving_average_filter_u16(uint16_t *data, int length);
void moving_average_filter_u32(uint32_t *data, int length);

#endif
