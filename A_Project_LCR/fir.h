#ifndef __IIC_H
#define __IIC_H

#include "ti_msp_dl_config.h"

/* ----------------------------------------------------------------------
** Macro Defines
** ------------------------------------------------------------------- */

#define TEST_LENGTH_SAMPLES 320
#define SNR_THRESHOLD_F32 140.0f
#define BLOCK_SIZE 32
#define NUM_TAPS 36

/* -------------------------------------------------------------------
 * Declare Test output buffer
 * ------------------------------------------------------------------- */

static float32_t testOutput[TEST_LENGTH_SAMPLES];

/* -------------------------------------------------------------------
 * Declare State buffer of size (numTaps + blockSize - 1)
 * ------------------------------------------------------------------- */

static float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1];

/* ----------------------------------------------------------------------
** FIR Coefficients buffer generated using fir1() MATLAB function.
** fir1(28, 6/24)
** ------------------------------------------------------------------- */

const float32_t firCoeffs32_LPF1[NUM_TAPS] = {
    0.0003765290021, 0.0008159068529, 0.001624324243, 0.002866766416, 0.004646241199,
    0.007049663924, 0.01013551932, 0.01391998585, 0.01836655475, 0.02338001132,
    0.0288046021, 0.03442977369, 0.04000146687, 0.0452394262, 0.04985871539,
    0.05359351262, 0.0562196523, 0.05757512897, 0.05757512897, 0.0562196523,
    0.05359351262, 0.04985871539, 0.0452394262, 0.04000146687, 0.03442977369,
    0.0288046021, 0.02338001132, 0.01836655475, 0.01391998585, 0.01013551932,
    0.007049663924, 0.004646241199, 0.002866766416, 0.001624324243, 0.0008159068529,
    0.0003765290021};

#endif