#include "arm_const_structs.h"
#include "arm_math.h"
#include "bmp.h"
#include "dac_data.h"
#include "delay.h"
#include "key.h"
#include "locale.h"
#include "math.h"
#include "my_filter.h"
#include "oled.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ti_msp_dl_config.h"
#include "tjc_usart_hmi.h"

#define ADC_SAMPLE_SIZE (1024)
#define ADC_MAX (4096)
#define ADC_SAMPLE_SIZE_1 (10)

#define COMP_INST_REF_VOLTAGE_mV (1400)

#define FLAG_RG0 (0)
#define FLAG_RG1 (1)
#define FLAG_RG2 (2)
#define FLAG_OPEN (0)
#define FLAG_L (1)
#define FLAG_C (2)
#define FLAG_R (3)
#define FLAG_D_FORWARD (4)
#define FLAG_D_INVERSE (5)

#define DAC 1
#define OPA 0
#define delay_usart delay_ms(5)

#define MODE_MEASURE (1)
#define MODE_CALIBRATION (2)
#define MODE_CONTINUE (3)

uint16_t ADC_Mean(uint16_t *data);
double phaseDetecter(uint16_t *data, uint16_t *dacSquare, double dataMean);
double calculateVpp(uint16_t *data, int flag);
void sortBubble(double *data, int length);
double DataMean(uint16_t *data);
double phaseDetecter(uint16_t *data, uint16_t *dacSquare, double dataMean);
void calculate_LCR();
uint8_t judRange();
uint8_t adjustGain();
void judRLC();
void controlRG();
void controlRelay(uint8_t flag);
void measureDiode();
void display();
void uartPrintf(const char *str, ...);
void uartPrintfLong(const char *str, ...);
void printfFireWater_ADC(uint16_t *ADC_data);
void calculateVpp_FFT(float32_t *data, float32_t *maxValue,
                      uint32_t *testIndex);
double calculatPhase_FFT();
double phaseDetecter_FFT(uint16_t *dataDac, uint16_t *dataOpa);
void phaseDetecter_MPY(uint16_t *opaData, uint16_t *dacData, double dataMean);
double calculatPhase_FFT();

// 测量模式标志位
uint8_t modeFlag = 0;

const uint16_t dutyDelta = 31;

// ADC的采样频率为125kHz，1kHz的信号一个周期内采集125个点
const double dutyCycle = 125;
// ADC采样前后差一个点的时间，把这个相位补上
const double phaseCompensate = 2 * M_PI / dutyCycle;

// OPA模拟信号链的相位补偿
double phaseCompensateOPA = 0.04;
// OPA模拟信号链的直流电压
uint16_t opaVdc = 2048;
// 二极管测量数据
uint16_t diodeData[ADC_SAMPLE_SIZE_1];
uint16_t diodeVdcInverse, diodeVdcForward;

// ADC采样变量
int gADCIndex = 0;
volatile uint16_t dacData[ADC_SAMPLE_SIZE];
volatile uint16_t opaData[ADC_SAMPLE_SIZE];
volatile bool gCheckADC = false;

/* 鉴相过程的各种变量 */
uint16_t opaData0[ADC_SAMPLE_SIZE];
uint16_t opaData90[ADC_SAMPLE_SIZE];

double opaData_Mean;
double dacData_Mean;
double opaData_Vpp;
double dacData_Vpp;
double opaData_Vmax;

double opa0, opa90;

double phase;
double phaseSin;
double phaseCos;
/* -------------------------------------- */

// 增益变量
double gain0 = 2, gain1 = 2;
double gain = 4;

// 电路状态
uint8_t flagElement = 0;

// 分压电阻阻值
const double RG_Set[3] = {100.4, 5100, 510000};

// 分压档位标志位
uint8_t flagRG = 0;

// 角频率
double w = 2 * M_PI * 1000;

// 单次测量值
double R, C, L, ESR, V;
// 多次测量，计算得到的最终值
double midData, meanData, midDataESR;
// 缓冲区设置
const int bufferLength = 20;
double bufferData[20];
double bufferDataESR[20];
double phaseBuffer[20];
double VppBuffer[20];

/* Delay for 5ms to ensure UART TX is idle before starting transmission */
#define UART_TX_DELAY (160000)

/* Data for UART to transmit */
// const uint8_t gTxPacket[20] = {'M', 'S', 'P', '!'};
#define USART_STR_LENGTH_LARGE 500
#define USART_STR_LENGTH_SMALL 20
#define LARGE 0
#define SMALL 1
uint8_t gTxPacket[USART_STR_LENGTH_LARGE + 1] = {};
uint8_t gTxBuffer[USART_STR_LENGTH_SMALL + 1] = {};

volatile bool gCheckUART, gDMADone;

/* Number of bytes for UART packet size */
#define UART_PACKET_SIZE (5)

/* Data received from UART */
volatile uint8_t gRxPacket[UART_PACKET_SIZE];

volatile bool gCheckUARTreceive;

struct Key key0 = {0, 0, 0, 0, 0};

const DL_MathACL_operationConfig gMpyConfig = {
    .opType = DL_MATHACL_OP_TYPE_MPY_32,
    .opSign = DL_MATHACL_OPSIGN_UNSIGNED,
    .iterations = 1,
    .scaleFactor = 0,
    .qType = DL_MATHACL_Q_TYPE_Q31};

const DL_MathACL_operationConfig gDivConfig = {.opType = DL_MATHACL_OP_TYPE_DIV,
                                               .opSign =
                                                   DL_MATHACL_OPSIGN_UNSIGNED,
                                               .iterations = 0,
                                               .scaleFactor = 1,
                                               .qType = DL_MATHACL_Q_TYPE_Q18};

void sortBubble(double *data, int length)
{
  double t;

  for (int i = 0; i < length - 1; i++)
  {
    for (int j = 0; j < length - 1 - i; j++)
    {
      if (data[j] > data[j + 1])
      {
        t = data[j + 1];
        data[j + 1] = data[j];
        data[j] = t;
      }
    }
  }
}
/**
 * 计算给定数据数组的平均值
 *
 * @param data 指向包含ADC采样数据的数组的指针
 * @return 返回计算得到的平均值
 */
double DataMean(uint16_t *data)
{
  double mean_res, sum = 0; // 初始化计算变量

  // 遍历ADC采样数据数组，并累加到sum
  for (int i = 0; i < ADC_SAMPLE_SIZE; i++)
    sum += (double)data[i];

  // 计算平均值
  mean_res = sum / ADC_SAMPLE_SIZE;

  return mean_res;
}

/**
 * 计算给定数据数组的平均值
 *
 * @param data 指向要计算平均值的数据数组的指针
 * @param length 数据数组的长度
 * @return 返回计算得到的平均值
 */
double findMean(double *data, int length)
{
  double mean_res, sum = 0; // 初始化计算平均值所需的变量

  sortBubble(data, length);
  for (int i = 1; i < length - 1; i++)
    sum += (double)data[i];
  // 计算平均值
  if (length > 2)
    mean_res = sum / (length - 2);
  else
    return 0;

  return mean_res; // 返回平均值
}
/**
 * 查找给定数据数组的中位数
 * @param data 指向包含待查找中位数的一维数组的指针
 * @param length 数组的长度
 * @return 返回数组的中位数
 */
double findMedian(double *data, int length)
{
  double mid;
  // 使用冒泡排序对数组进行排序
  sortBubble(data, length);
  // 计算并返回中位数
  mid = data[length / 2];
  return mid;
}
/**
 * @brief 对ADC采样数据进行波形整形
 *
 * 该函数遍历输入的ADC采样数据数组，并根据数据与平均值的比较结果，
 * 将每个采样点转换为二进制形式。如果采样数据大于平均值，则对应点设置为1，
 * 否则设置为0。此过程旨在将连续的模拟信号转换为离散的数字信号。
 *
 * @param data 指向ADC采样数据数组的指针
 * @param dataMean 采样数据的平均值，用于作为比较基准
 */
void waveShaping(uint16_t *data, uint16_t dataMean)
{
  // 遍历ADC采样数据数组，将每个数据点转换为0或1
  for (int i = 0; i < ADC_SAMPLE_SIZE; i++)
  {
    // 根据数据点与平均值的比较结果，设置数据点为0或1
    if (data[i] > dataMean)
      data[i] = 1;
    else
      data[i] = 0;
  }
}

/**
 * @brief 正交鉴相
 *
 * 该函数用于通过比较输入的模拟数据与DAC产生的方波信号（整形后的）来检测相位差。
 * 具体做法是将输入数据分为两组，分别对应DAC方波的“高电平”和“低电平”时间段，
 * 然后计算这两组数据的均值并进行相位差的计算。
 *
 * @param data 指向包含ADC采样数据的数组的指针。
 * @param dacSquare 指向包含DAC产生的方波信号（整形后的）的数组的指针。
 * @param dataMean 输入数据的平均值，用于调整计算均值时的参考点。
 * @return double 返回相位差，单位为弧度。
 */
double phaseDetecter(uint16_t *data, uint16_t *dacSquare, double dataMean)
{
  for (int i = 0; i < ADC_SAMPLE_SIZE; i++)
  {
    if (dacSquare[i])
      opaData0[i] = data[i];
    else
      opaData0[i] = 0;
  }

  // 将DAC方波信号向后移动90度，并保留原始数据的一部分用于后续处理
  uint16_t dataDuty[dutyDelta];
  int j = 0;
  for (int i = 0; i < dutyDelta; i++)
    dataDuty[i] = dacSquare[i];
  for (int i = dutyDelta; i < ADC_SAMPLE_SIZE; i++, j++)
    dacSquare[j] = dacSquare[i];
  for (int i = 0; i < dutyDelta; i++, j++)
    dacSquare[j] = dataDuty[i];

  // 处理数据，仅保留与DAC方波低电平对应的ADC采样值
  for (int i = 0; i < ADC_SAMPLE_SIZE; i++)
  {
    if (dacSquare[i])
      opaData90[i] = data[i];
    else
      opaData90[i] = 0;
  }

  opa0 = DataMean(opaData0) - dataMean / 2;
  opa90 = DataMean(opaData90) - dataMean / 2;

  // 返回相位差，使用反正切函数计算
  return atan2(opa90, opa0);
}

/**
 * 计算电压峰-峰值（Vpp）
 *
 * 本函数通过对传入的ADC采样数据进行平滑处理，然后计算出平滑后的数据的峰-峰值，并对此峰-峰值进行一定的补偿后返回。
 *
 * @param data 指向包含ADC采样数据的数组的指针。
 * @return 返回计算得到的电压峰-峰值（Vpp）。
 */
float32_t maxValue = 0;
double calculateVpp(uint16_t *data, int flag)
{
  int index = 0;
  // double smoothedData[ADC_SAMPLE_SIZE] = {0}; // 存储平滑处理后的数据
  uint32_t smoothedData[ADC_SAMPLE_SIZE] = {0};
  float32_t inputFFT[ADC_SAMPLE_SIZE / 2] = {0};

  uint32_t testIndex = 0;
  double Vpp;

  if (flag == 0)
  {
    for (int i = 0; i < ADC_SAMPLE_SIZE; i++)
    {
      smoothedData[i] = (uint32_t)data[i];
    }
  }
  else if (flag == 1)
  {
    for (int i = 0; i < ADC_SAMPLE_SIZE; i++)
    {
      smoothedData[i] = (uint32_t)data[i];
    }
    // 对传入的数据进行平滑处理，使用移动平均滤波器
    moving_average_filter_u32(smoothedData, ADC_SAMPLE_SIZE);
    moving_average_filter_u32(smoothedData, ADC_SAMPLE_SIZE);
    // moving_average_filter_double(smoothedData, ADC_SAMPLE_SIZE); //
    // 应用移动平均滤波器 moving_average_filter_double(smoothedData,
    // ADC_SAMPLE_SIZE); // 应用移动平均滤波器
  }
  // else if (flag == 2)
  // {
  //     for (int i = 0; i < ADC_SAMPLE_SIZE; i++)
  //     {
  //         inputFFT[i] = (float32_t)data[i];
  //     }
  //     calculateVpp_FFT(inputFFT, &maxValue, &testIndex);
  //     Vpp = (double)maxValue;

  //     return Vpp;
  // }

  // 计算峰值和谷值
  // double peak, valley;
  // double compensate = 1.5; // 补偿值，用于调整计算出的Vpp
  uint32_t peak, valley;
  double compensate = 0; // 补偿值，用于调整计算出的Vpp

  peak = smoothedData[MVF_LENGTH + 1 + ADC_SAMPLE_SIZE / 2];
  valley = smoothedData[MVF_LENGTH + 1 + ADC_SAMPLE_SIZE / 2];

  // 遍历平滑后的数据，找出峰值和谷值
  //    for (int j = 0; j < ADC_SAMPLE_SIZE / (int)dutyCycle - 1; j++)
  {
    for (int i = MVF_LENGTH + 1 + ADC_SAMPLE_SIZE / 2;
         i < MVF_LENGTH + 1 + ADC_SAMPLE_SIZE / 2 + dutyCycle; i++)
    {
      if (peak < smoothedData[i])
      {
        peak = smoothedData[i];
        // peak = (smoothedData[i] + smoothedData[i + 1]) / 2; // 确定峰值
      }
      if (valley > smoothedData[i])
      {
        valley = smoothedData[i];
        // valley = (smoothedData[i] + smoothedData[i + 1]) / 2; // 确定谷值
      }
    }
    opaData_Vmax = (double)peak;              // 存储最大电压值，可能用于其他地方
    Vpp = (double)peak - valley - compensate; // 计算峰-峰值并进行补偿
  }
  return Vpp;
}

uint32_t fftSize = ADC_SAMPLE_SIZE;
uint32_t ifftFlag = 0;
uint32_t doBitReverse = 1;
float32_t outputFFT[ADC_SAMPLE_SIZE / 2] = {0};
void calculateVpp_FFT(float32_t *data, float32_t *maxValue,
                      uint32_t *testIndex)
{
  /* Process the data through the CFFT/CIFFT module */
  arm_cfft_f32(&arm_cfft_sR_f32_len1024, data, ifftFlag, doBitReverse);

  /* Process the data through the Complex Magnitude Module for
  calculating the magnitude at each bin */
  arm_cmplx_mag_f32(data, outputFFT, fftSize);
  // 去掉直流
  outputFFT[0] = 0;
  /* Calculates maxValue and returns corresponding BIN value */
  arm_max_f32(outputFFT, fftSize, maxValue, testIndex);

  *maxValue /= fftSize;
  *maxValue *= 2;
}

float32_t fftData[ADC_SAMPLE_SIZE * 2] = {0};
float32_t dd = 10;
double phaseDetecter_FFT(uint16_t *dataDac, uint16_t *dataOpa)
{

  double ph1 = 0, ph2 = 0;

  // uartPrintf("%d\n", dataDac[1]);

  // printf("%d\n",dataDac[0]);
  for (int i = 0; i < ADC_SAMPLE_SIZE; i++)
  {
    fftData[i * 2] = dataDac[i];
    fftData[i * 2 + 1] = 0;
  }

  ph1 = calculatPhase_FFT(fftData);

  for (int i = 0; i < ADC_SAMPLE_SIZE; i++)
    fftData[i * 2] = (float32_t)dataOpa[i];
  for (int i = ADC_SAMPLE_SIZE; i < ADC_SAMPLE_SIZE * 2 - 1; i++)
    fftData[i] = 0;
  ph2 = calculatPhase_FFT(fftData);

  return ph1 - ph2;
}
// float32_t fftDataa[ADC_SAMPLE_SIZE * 2] = {0};
// float32_t fftDataa2[ADC_SAMPLE_SIZE * 2] = {0};
double calculatPhase_FFT()
{

  arm_cfft_f32(&arm_cfft_sR_f32_len1024, fftData, ifftFlag, doBitReverse);

  return atan2(fftData[9 * 2 + 1], fftData[9 * 2]);
}

/*
 * 计算LCR组件的值
 * 无参数
 * 无返回值
 */
void calculate_LCR()
{
  double deg2, deg3, UR, Z_test; // 定义计算过程中用到的变量

  switch (flagElement) // 根据flagElement的值选择计算类型
  {
  case FLAG_L: // 电感计算
    dacData_Vpp = 800.0 / 3300 * 4096;
    UR = sqrt(dacData_Vpp * dacData_Vpp + opaData_Vpp * opaData_Vpp -
              2 * dacData_Vpp * opaData_Vpp * cos(phase));
    deg2 =
        acos((dacData_Vpp * dacData_Vpp + UR * UR - opaData_Vpp * opaData_Vpp) /
             (2 * dacData_Vpp * UR));
    deg3 = M_PI / 2 - phase - deg2;
    Z_test = cos(deg3) * opaData_Vpp * RG_Set[flagRG] / UR;
    L = Z_test / w;
    // 通过调整计算值以优化结果的准确性
    if (opaData_Vpp * gain / (opaData_Vmax - 2048) <= 1.8)
    {
      // opaData_Vpp = 2 * (opaData_Vmax - 2048) / gain;
      opaData_Vpp *= 1.3;
      UR = sqrt(dacData_Vpp * dacData_Vpp + opaData_Vpp * opaData_Vpp -
                2 * dacData_Vpp * opaData_Vpp * cos(phase));
      deg2 = acos(
          (dacData_Vpp * dacData_Vpp + UR * UR - opaData_Vpp * opaData_Vpp) /
          (2 * dacData_Vpp * UR));
      deg3 = M_PI / 2 - phase - deg2;
      Z_test = cos(deg3) * opaData_Vpp * RG_Set[flagRG] / UR;
      L = Z_test / w;
    }
    ESR = sin(deg3) * opaData_Vpp * RG_Set[flagRG] / UR;
    break;
  case FLAG_C: // 电容计算
    dacData_Vpp = 800.0 / 3300 * 4096;
    UR = sqrt(dacData_Vpp * dacData_Vpp + opaData_Vpp * opaData_Vpp -
              2 * dacData_Vpp * opaData_Vpp * cos(phase));
    deg2 =
        acos((dacData_Vpp * dacData_Vpp + UR * UR - opaData_Vpp * opaData_Vpp) /
             (2 * dacData_Vpp * UR));
    deg3 = M_PI / 2 + phase - deg2;
    Z_test = cos(deg3) * opaData_Vpp * RG_Set[flagRG] / UR;
    // 拟合结果
    C = 1 / (w * Z_test) - 125e-12;
    ESR = sin(deg3) * opaData_Vpp * RG_Set[flagRG] / UR;
    break;
  case FLAG_R: // 电阻计算
    dacData_Vpp = 800.0 / 3300 * 4096;
    R = RG_Set[flagRG] * opaData_Vpp / (dacData_Vpp - opaData_Vpp);
    // 拟合结果
    R = 6.25e6 * R / (6.25e6 - R);
    break;
  }
}

void controlRG()
{
  // 根据 flagRG 的值设置 GPIO 管脚状态
  switch (flagRG)
  {
  case FLAG_RG0:
    // 当 flagRG 为 FLAG_RG0 时，设置 SW0 管脚为高电平，SW1 管脚为低电平
    DL_GPIO_setPins(GPIO_SW_PORT, GPIO_SW_PIN_SW0_PIN);
    DL_GPIO_clearPins(GPIO_SW_PORT, GPIO_SW_PIN_SW1_PIN);
    break;
  case FLAG_RG1:
    // 当 flagRG 为 FLAG_RG1 时，设置 SW0 管脚为低电平，SW1 管脚为高电平
    DL_GPIO_clearPins(GPIO_SW_PORT, GPIO_SW_PIN_SW0_PIN);
    DL_GPIO_setPins(GPIO_SW_PORT, GPIO_SW_PIN_SW1_PIN);
    break;
  case FLAG_RG2:
    // 当 flagRG 为 FLAG_RG2 时，同时设置 SW0 和 SW1 管脚为高电平
    DL_GPIO_setPins(GPIO_SW_PORT, GPIO_SW_PIN_SW0_PIN | GPIO_SW_PIN_SW1_PIN);
    break;
  }
}

/**
 * @brief 判断电压范围
 *
 * 该函数用于根据输入的模拟数据判断当前电压范围，并据此进行相应的控制调整。
 * 不接受任何参数。
 *
 * @return uint8_t 返回电压范围判断的结果：
 *         - 1: 表示需要增加增益（或降低电压范围）；
 *         - 2: 表示当前电压范围适宜；
 *         - 其他值：当前未使用，预留作为未来可能的扩展。
 */
uint8_t judRange()
{
  // 初始化默认电压范围为range/4096*3300
  double range = 700.0 / 4096 * 3300;

  // 判断当前模拟数据是否超过设定的电压范围
  if (opaData_Vpp >= range * gain)
  {
    // 如果当前增益设置不是最大值，则增加增益并返回1
    if (flagRG != FLAG_RG2)
    {
      flagRG++;
      controlRG(); // 控制增益
      return 1;
    }
    // 如果当前增益已经是最大值，则返回0，表示不需要进一步增加增益
    else
      return 0;
  }
  // 当前电压范围适宜，返回2
  return 2;
}

/**
 * 调整增益函数
 * 该函数用于根据ADC测量值与opaData_Vpp的比值调整运放OPA1的增益。
 * 通过计算比值，选择合适的增益值以保持信号在合适的范围内。
 *
 * @return uint8_t 返回值为0表示增益未调整，为1表示增益已调整。
 */
uint8_t adjustGain()
{
  // 计算当前温度对应的增益调整值
  double MAX = 800 / 3300 * 4096;
  double temp = (double)ADC_MAX / opaData_Vpp;
  double t = gain0;

  // 根据温度比值选择合适的运放增益
  if (temp >= 2 && temp < 4)
  {
    gain0 = 4;
    DL_OPA_setGain(OPA_0_INST, DL_OPA_GAIN_N3_P4);
  }
  else if (temp >= 4 && temp < 8)
  {
    gain0 = 8;
    DL_OPA_setGain(OPA_0_INST, DL_OPA_GAIN_N7_P8);
  }
  else if (temp >= 8 && temp < 16)
  {
    gain0 = 16;
    DL_OPA_setGain(OPA_0_INST, DL_OPA_GAIN_N15_P16);
  }
  else if (temp >= 16 && temp < 32)
  {
    gain0 = 32;
    DL_OPA_setGain(OPA_0_INST, DL_OPA_GAIN_N31_P32);
  }
  else if (temp >= 32 && temp < 64)
  {
    gain0 = 32;
    DL_OPA_setGain(OPA_0_INST, DL_OPA_GAIN_N31_P32);
    gain1 = 4;
    DL_OPA_setGain(OPA_1_INST, DL_OPA_GAIN_N3_P4);
  }
  else if (temp >= 64 && temp < 128)
  {
    gain0 = 32;
    DL_OPA_setGain(OPA_0_INST, DL_OPA_GAIN_N31_P32);
    gain1 = 8;
    DL_OPA_setGain(OPA_1_INST, DL_OPA_GAIN_N7_P8);
  }
  else if (temp >= 128 && temp < 256)
  {
    gain0 = 32;
    DL_OPA_setGain(OPA_0_INST, DL_OPA_GAIN_N31_P32);
    gain1 = 16;
    DL_OPA_setGain(OPA_1_INST, DL_OPA_GAIN_N15_P16);
  }
  else if (temp >= 256 && temp < 512)
  {
    gain0 = 32;
    DL_OPA_setGain(OPA_0_INST, DL_OPA_GAIN_N31_P32);
    gain1 = 32;
    DL_OPA_setGain(OPA_1_INST, DL_OPA_GAIN_N31_P32);
  }
  // 更新总增益值
  gain = gain1 * gain0;

  // 判断增益是否被调整
  // 如果增益值没有变化，则返回0；否则返回1
  if (gain0 == t)
    return 0;
  else
    return 1;
}
/**
 * @brief 判断RLC电路当前状态
 *
 * 本函数用于根据相位值判断RLC电路的当前元素类型（电感L、电容C或电阻R）。
 *
 * @param 无 输入参数
 * @return 无 返回值
 */
void judRLC()
{
  // phase += 0.335;
  // 根据相位值判断电路元素类型
  if (phase <= -0.25)
    flagElement = FLAG_C; // 当相位小于等于-0.25时，设为电容C
  else if (phase >= 0.15)
    flagElement = FLAG_L; // 当相位大于等于0.2时，设为电感L
  else
    flagElement = FLAG_R; // 其他情况设为电阻R
                          // phase -= 0.335;
                          // uartPrintf("\n%lf\n", phase);
}
/**
 * @brief 控制继电器的状态
 *
 * 该函数用于根据输入的标志位控制继电器的开关状态。如果标志位为0，则打开继电器，否则关闭继电器。
 *
 * @param flag 控制继电器的标志位，0表示打开继电器，非0表示关闭继电器
 */
void controlRelay(uint8_t flag)
{
  // 根据flag的值控制继电器的开关
  if (!flag)
  {
    DL_GPIO_setPins(GPIO_Relay_PORT, GPIO_Relay_PIN_2_PIN);
    // 设置GPIO端口，打开继电器
  }
  else
  {
    DL_GPIO_clearPins(GPIO_Relay_PORT, GPIO_Relay_PIN_2_PIN);
    // 清除GPIO端口，关闭继电器
  }
}

/**
 * @brief 测量二极管的特性
 *
 * 该函数通过对二极管进行正向和反向偏压测量，以评估其导通和截止特性。
 * 使用ADC采集数据，首先对二极管施加正向电压，记录数据，
 * 然后施加反向电压，再次记录数据。通过分析记录的数据，
 * 可以确定二极管的导通和截止状态，以及评估其性能。
 *
 * @note 该函数没有参数和返回值，但会修改全局变量，如flagElement和V。
 */
void measureDiode()
{
  double resForward = 0, resInverse = 0,
         res = 0; // 定义用于存储adc测量结果的变量
  int i = 0;

  NVIC_EnableIRQ(ADC12_1_INST_INT_IRQN); // 启用ADC中断
  DL_GPIO_setPins(GPIO_Measure_PORT,
                  GPIO_Measure_PIN_High_PIN); // 设置GPIO端口为高电平
  DL_GPIO_clearPins(GPIO_Measure_PORT,
                    GPIO_Measure_PIN_Low_PIN); // 清除GPIO端口的低电平
  delay_ms(10);                                // 延迟以稳定电路

  // 启动ADC转换并等待转换完成，将结果存储到数组中

  DL_ADC12_enableConversions(ADC12_1_INST); // 启用ADC转换
  DL_ADC12_startConversion(ADC12_1_INST);   // 开始ADC转换

  while (1)
  {
    /* 等待所有数据通道加载完成 */
    while (gCheckADC == false)
    {
      __WFE();
    }

    /* 将ADC结果存储到各自的缓冲区 */
    diodeData[i] = DL_ADC12_getMemResult(ADC12_1_INST, DL_ADC12_MEM_IDX_0);

    i++;
    gCheckADC = false;

    if (i >= ADC_SAMPLE_SIZE_1) // 当采集到足够的样本时
    {
      // 计算正向偏压下的平均电阻值

      for (int j = 0; j < ADC_SAMPLE_SIZE_1; j++)
        resForward += (double)diodeData[j];
      resForward /= ADC_SAMPLE_SIZE_1;
      break;
    }
  }

  // 改变GPIO状态以进行反向偏压测量

  DL_GPIO_clearPins(GPIO_Measure_PORT,
                    GPIO_Measure_PIN_High_PIN); // 清除高电平GPIO
  DL_GPIO_setPins(GPIO_Measure_PORT,
                  GPIO_Measure_PIN_Low_PIN); // 设置低电平GPIO
  delay_ms(10);                              // 延迟以稳定电路
  i = 0;
  DL_ADC12_enableConversions(ADC12_1_INST); // 启用ADC转换
  DL_ADC12_startConversion(ADC12_1_INST);   // 开始ADC转换

  while (1)
  {
    /* 等待所有数据通道加载完成 */
    while (gCheckADC == false)
    {
      __WFE();
    }

    /* 将ADC结果存储到各自的缓冲区 */
    diodeData[i] = DL_ADC12_getMemResult(ADC12_1_INST, DL_ADC12_MEM_IDX_0);

    i++;
    gCheckADC = false;

    if (i >= ADC_SAMPLE_SIZE_1) // 当采集到足够的样本时
    {
      // 计算反向偏压下的平均电阻值

      for (int j = 0; j < ADC_SAMPLE_SIZE_1; j++)
        resInverse += (double)diodeData[j];
      resInverse /= ADC_SAMPLE_SIZE_1;
      break;
    }
  }

  res = resForward - resInverse; // 计算电压差
  // 检测测量过程中电压是否有较大变化
  if (diodeData[0] - diodeData[ADC_SAMPLE_SIZE_1 - 1] > 50)
  {
    flagElement = 0; // 如果数据差异过大，则标记为非二极管
    return;
  }
  if (resForward < 4050 && resForward > 50 && resInverse < 4050 &&
      resInverse > 50)
  {
    flagElement = 0;
    return;
  }
  if (res > 124 && res < 3723)
  {
    double temp = resForward + resInverse; // 计算总电压
    if (temp > 4050 && temp < 4110)        // 判断是否在特定的电压范围内
      flagElement = 0;
    else if (abs((int)resForward - (int)res) <= 5) // 判断是正向电阻还是反向电阻
      flagElement = FLAG_D_FORWARD;
    else
      flagElement = FLAG_D_INVERSE;
  }
  gCheckADC = false;
  V = (double)res / 4096 * 3.3; // 计算电压值
  DL_GPIO_clearPins(GPIO_Measure_PORT, GPIO_Measure_PIN_High_PIN);
  DL_GPIO_clearPins(GPIO_Measure_PORT, GPIO_Measure_PIN_Low_PIN);
  delay_ms(10); // 延迟以准备下一次测量
}

/**
 * @brief 显示功能函数
 *
 * 该函数根据不同的元素类型（电容、电阻、电感、二极管）显示相应的值和单位。
 * 使用 OLED 屏幕进行显示。
 */

void display()
{
  //     定义用于显示的各类变量
  uint16_t intNum, floatNum;
  int a;                             // 用于临时存储计算结果
  char str1[8] = {0}, str2[8] = {0}; // 用于构建显示字符串
  char units[8] = {0};               // 存储单位
  char string[32] = "";              // 最终的显示字符串
  char title[16];                    // 元件标题
  char ESR_String[32] = "";          // 用于显示电感或电容的等效串联电阻（ESR）

  OLED_Clear();
  OLED_ColorTurn(0);   // 设置颜色
  OLED_DisplayTurn(0); // 设置显示方向
  if (flagElement == 0)
  {
    strcpy(title, "Open Circuit"); // 标题为开路
    OLED_ShowString(16, 16, (uint8_t *)title, 16, 1);
    OLED_Refresh();
    uartPrintf("Open Circuit");
    return;
  }
  else if (flagElement == FLAG_C)
  {
    OLED_ShowPicture(8, 8, 28, 48, BMP_C, 1);
    strcpy(title, "C"); // 标题为电容器

    // 根据电容值转换单位
    if (midData <= 1e-9)
    {
      midData *= pow(10, 12);
      midData *= 1.13;
      strcpy(units, "pF");
      uartPrintf("C:%.2lfpF", midData);
    }
    else if (midData <= 1e-6)
    {
      if (flagRG == FLAG_RG1)
      {
        midData = pow(midData, 2) * 6.2121e3 + 0.9552 * midData + 6.1279e-9;
      }
      midData *= pow(10, 9);
      strcpy(units, "nF");
      uartPrintf("C:%.2lfnF", midData);
    }
    else
    {
      midData *= pow(10, 6);
      strcpy(units, "uF");
      uartPrintf("C:%.2lfuF", midData);
    }
  }
  else if (flagElement == FLAG_R)
  {
    OLED_ShowPicture(8, 8, 28, 48, BMP_R, 1);
    strcpy(title, "R"); // 标题为电阻器

    // 根据电阻值转换单位
    if (midData <= 1e3)
    {
      strcpy(units, "ohm");
      uartPrintf("R:%.2lfohm", midData);
    }
    else if (midData <= 1e6)
    {
      midData /= pow(10, 3);
      strcpy(units, "kohm");
      uartPrintf("R:%.2lfkohm", midData);
    }
    else
    {
      midData /= pow(10, 6);
      strcpy(units, "Mohm");
      uartPrintf("R:%.2lfMohm", midData);
    }
  }
  else if (flagElement == FLAG_L)
  {
    OLED_ShowPicture(8, 8, 28, 48, BMP_L, 1);
    strcpy(title, "I"); // 标题为电感器

    // 根据电感值转换单位
    if (midData <= 1e-3)
    {
      midData *= pow(10, 6);
      strcpy(units, "uH");
      uartPrintf("L:%.2lfuH", midData);
    }
    else if (midData <= 1)
    {
      midData *= pow(10, 3);
      strcpy(units, "mH");
      uartPrintf("L:%.2lfmH", midData);
    }
  }
  else if (flagElement == FLAG_D_FORWARD)
  {
    OLED_ShowPicture(8, 8, 28, 48, BMP_D_FORWARD, 1);
    strcpy(title, "D"); // 正向二极管
    strcpy(units, "V");
    midData = V;
    uartPrintf("F:%.2lfV", midData);
  }
  else if (flagElement == FLAG_D_INVERSE)
  {
    OLED_ShowPicture(8, 8, 28, 48, BMP_D_BACKWARD, 1);
    strcpy(title, "D"); // 反向二极管
    strcpy(units, "V");
    midData = V;
    uartPrintf("B:%.2lfV", midData);
  }

  // 将中间数据转换为整数和小数部分
  intNum = (int)midData;
  floatNum = (int)((midData - intNum) * 1e2);
  sprintf(str1, "%d", intNum);
  if (floatNum < 10)
    sprintf(str2, "0%d", floatNum);
  else
    sprintf(str2, "%d", floatNum);

  // 生成最终的显示字符串

  strcat(string, str1);
  strcat(string, ".");
  strcat(string, str2);
  strcat(string, units);

  OLED_ShowString(78, 16, (uint8_t *)title, 16, 1);
  OLED_ShowString(50, 32, (uint8_t *)string, 16, 1);

  // 如果是电容或电感，显示ESR
  if (flagElement == FLAG_C || flagElement == FLAG_L)
  {
    // 处理并显示ESR
    if (midDataESR >= 0 && midDataESR < 8)
    {
    }
    else if (midDataESR >= 0 && midDataESR < 55)
    {
    }
    else
    {
      OLED_Refresh();
      return;
    }
    for (int i = 0; i < bufferLength; i++)
    {
      if (bufferDataESR[i] < 0)
      {
        OLED_Refresh();
        return;
      }
    }
    // strcpy(ESR_String, "ESR:");
    intNum = (int)midDataESR;
    floatNum = (int)((midDataESR - intNum) * 1e2);
    sprintf(str1, "%d", intNum);
    if (floatNum < 10)
      sprintf(str2, "0%d", floatNum);
    else
      sprintf(str2, "%d", floatNum);

    strcat(ESR_String, str1);
    strcat(ESR_String, ".");
    strcat(ESR_String, str2);
    strcat(ESR_String, "ohm");
    OLED_ShowString(56, 48, (uint8_t *)ESR_String, 16, 1);
    uartPrintf("ESR:%.2lf", midDataESR);
  }

  // OLED_ShowPicture(8, 8, 28, 48, BMP_C, 1);
  OLED_Refresh(); // 刷新OLED屏幕显示
}

/**
 * 使用DMA和UART通过格式化字符串向串口发送数据。
 *
 * @param str 格式化字符串，类似于printf函数中的格式化字符串。
 * @param ... 可变参数，与格式化字符串中的占位符匹配。
 *
 * 该函数首先使用vsnprintf将格式化后的字符串存入全局传输缓冲区gTxBuffer中，
 * 然后配置DMA以将该缓冲区中的数据传输到UART的TXDATA寄存器中。
 * 使用DMA进行数据传输可以解放CPU，同时通过DMA中断来完成数据传输。
 * 在传输完成后，函数会清空传输缓冲区并再次配置DMA以准备下一次传输。
 */
void uartPrintf(const char *str, ...)
{
  uint8_t i = 0;
  va_list arg_ptr;
  va_start(arg_ptr, str); // 初始化可变参数。
  vsnprintf((char *)gTxBuffer, USART_STR_LENGTH_SMALL + 1, str,
            arg_ptr); // 格式化字符串到缓冲区。
  va_end(arg_ptr);    // 结束可变参数处理。

  delay_usart; // 微小延迟，为DMA配置留出时间。

  // 配置DMA的源地址为gTxBuffer的起始地址。
  DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)&gTxBuffer[0]);
  // 配置DMA的目的地址为UART的TXDATA寄存器地址。
  DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)(&UART_0_INST->TXDATA));
  // 设置DMA传输的数据量为gTxBuffer的大小。
  DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID,
                         sizeof(gTxBuffer) / sizeof(gTxBuffer[0]));

  DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID); // 启用DMA通道。

  /* 等待DMA传输完成 */
  while (false == gDMADone)
  {
    __WFE(); // 进入低功耗模式，等待事件。
  }

  /* 等待UART传输完成，即TX FIFO为空 */
  while (false == gCheckUART)
  {
    __WFE(); // 同上。
  }

  // 重置标志位，准备下一次传输。
  gCheckUART = false;
  gDMADone = false;

  DL_DMA_disableChannel(DMA, DMA_CH0_CHAN_ID); // 禁用DMA通道。

  memset(gTxBuffer, 0, sizeof(gTxBuffer)); // 清空传输缓冲区。
  delay_usart;                             // 微小延迟，为后续操作留出时间。
}

// 同上，字符串长度加长
void uartPrintfLong(const char *str, ...)
{

  uint8_t i = 0;
  va_list arg_ptr;
  va_start(arg_ptr, str);
  vsnprintf((char *)gTxPacket, USART_STR_LENGTH_LARGE + 1, str, arg_ptr);
  va_end(arg_ptr);

  delay_usart;
  DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)&gTxPacket[0]);
  DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)(&UART_0_INST->TXDATA));
  DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID,
                         sizeof(gTxPacket) / sizeof(gTxPacket[0]));

  DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);

  /* Wait in SLEEP mode until DMA interrupt is triggered */
  while (false == gDMADone)
  {
    __WFE();
  }

  /* Wait until all bytes have been transmitted and the TX FIFO is empty */
  while (false == gCheckUART)
  {
    __WFE();
  }

  gCheckUART = false;
  gDMADone = false;

  DL_DMA_disableChannel(DMA, DMA_CH0_CHAN_ID);

  memset(gTxPacket, 0, sizeof(gTxPacket)); // 清空传输缓冲区。
  delay_usart;
}
/**
 * @brief 打印ADC数据
 *
 * 该函数通过UART串口逐个打印ADC采样数组中的数据。
 *
 * @param ADC_data 指向包含ADC采样数据的数组的指针。
 */
void printfFireWater_ADC(uint16_t *ADC_data)
{
  // 遍历ADC采样数组，并将每个数据通过串口打印出来
  for (int i = 0; i < ADC_SAMPLE_SIZE; i++)
  {
    uartPrintf("d1:%d\n", ADC_data[i]);
  }
}
void uartReceive()
{
  /* Configure DMA source, destination and size */
  DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)(&UART_0_INST->RXDATA));
  DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)&gRxPacket[0]);
  DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, UART_PACKET_SIZE);
  DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);

  /* Confirm DMA channel is enabled */
  while (false == DL_DMA_isChannelEnabled(DMA, DMA_CH0_CHAN_ID))
  {
  }
  NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
  gCheckUARTreceive = false;

  /* Wait in SLEEP mode until DMA interrupt is triggered */
  while (false == gCheckUARTreceive)
  {
    __WFE();
  }
}

/*
 * DAC8 Reference Voltage in mV
 *  Adjust this value according to the internal VREF configuration.
 */
#define OPA_0_INST_REF_VOLTAGE_mV (3300)
/*
 * DAC8 static output voltage in mV.
 *  Adjust output as needed.
 */
#define OPA_0_INST_DAC8_OUTPUT_VOLTAGE_mV (1000)

int main(void)
{
  // 初始化系统配置
  SYSCFG_DL_init();

  // 初始化OLED显示
  OLED_Init();

  // 系统进入休眠模式时保持DMA唤醒
  //    DL_SYSCTL_enableSleepOnExit();

  // 关闭继电器
  controlRelay(1);

  // 启用TIMER_0的中断
  NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
  // 启动TIMER_0计数器
  DL_TimerG_startCounter(TIMER_0_INST);

  // DAC配置：设置DMA源地址、目的地址和传输大小
  DL_DMA_setSrcAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t)&gOutputSignalSine1000[0]);
  DL_DMA_setDestAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t) & (DAC0->DATA0));
  DL_DMA_setTransferSize(DMA, DMA_CH1_CHAN_ID,
                         sizeof(gOutputSignalSine1000) / sizeof(uint16_t));
  // 启用DAC的中断
  NVIC_EnableIRQ(DAC12_INT_IRQN);
  // 启用DMA通道
  DL_DMA_enableChannel(DMA, DMA_CH1_CHAN_ID);
  /* -----------------------------------------------------------------------------------------
   */

  // 启用UART的中断
  NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
  gCheckUART = false;
  gDMADone = false;
  gCheckUARTreceive = false;

  //  启用GPIO按键和TIMER_1的中断
  NVIC_EnableIRQ(GPIO_KEY_INT_IRQN);
  NVIC_EnableIRQ(TIMER_1_INST_INT_IRQN);
  uint8_t qq = 1;
START:
  // UART接收DMA缓冲区
  /* Configure DMA source, destination and size */
  DL_DMA_setSrcAddr(DMA, DMA_CH2_CHAN_ID, (uint32_t)(&UART_0_INST->RXDATA));
  DL_DMA_setDestAddr(DMA, DMA_CH2_CHAN_ID, (uint32_t)&gRxPacket[0]);
  DL_DMA_setTransferSize(DMA, DMA_CH2_CHAN_ID, UART_PACKET_SIZE);
  DL_DMA_enableChannel(DMA, DMA_CH2_CHAN_ID);

  /* Confirm DMA channel is enabled */
  while (false == DL_DMA_isChannelEnabled(DMA, DMA_CH2_CHAN_ID))
    ;

  while (1)
  {
    delay_ms(50);
    while (1)
    {
      // 50ms周期性延时
      delay_ms(50);
      // 检测按键输入，根据按键状态进入不同模式
      if (key0.KeyAlready)
      {
        if (key0.Continuous)
          modeFlag = MODE_CALIBRATION;
        else if (key0.KeyNumberContinuous >= 2)
          modeFlag = MODE_CONTINUE;
        else
          modeFlag = MODE_MEASURE;
        key0.Continuous = 0;
        key0.KeyAlready = 0;
        key0.KeyState = 0;
        key0.KeyNumberContinuous = 0;
        key0.flagPeriod = 0;
        DL_GPIO_setPins(GPIO_LEDS_LED_Sleep_PORT, GPIO_LEDS_LED_Sleep_PIN);
        DL_TimerG_stopCounter(TIMER_0_INST);
        SYSCFG_DL_TIMER_0_init();
        break;
      }
      else if (true == gCheckUARTreceive)
      {
        gCheckUARTreceive = false;
        if (strcmp((char *)gRxPacket, "START"))
        {
          DL_GPIO_setPins(GPIO_LEDS_LED_Sleep_PORT, GPIO_LEDS_LED_Sleep_PIN);
          DL_TimerG_stopCounter(TIMER_0_INST);
          SYSCFG_DL_TIMER_0_init();
          break;
        }
      }
      else if (modeFlag == MODE_CONTINUE)
        break;
    }
    /* -----------------------------------------------------------------------------------------
     */
    // modeFlag = MODE_MEASURE;
    if (modeFlag == MODE_CALIBRATION)
    {
      OLED_Clear();
      OLED_Refresh();
      goto START;
    }
    else if (modeFlag == MODE_MEASURE || modeFlag == MODE_CONTINUE)
    {
      uint16_t i = 0;
      // 启用ADC的中断
      NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);

      gCheckADC = false;

      // 进行连续测量和数据处理
      while (1)
      {
        int kk = -1, jj = -1;

        // 控制继电器以切换测量电路
        controlRelay(0);

        // 测量二极管特性
        measureDiode();
        if (flagElement == FLAG_D_FORWARD)
        {
          display();
          flagElement = 0;
          DL_TimerG_startCounter(TIMER_0_INST);
          goto START;
        }
        else if (flagElement == FLAG_D_INVERSE)
        {
          display();
          flagElement = 0;
          DL_TimerG_startCounter(TIMER_0_INST);
          goto START;
        }
        else
        {
        LOOP:
          controlRelay(1);
          delay_ms(10);
          DL_ADC12_startConversion(ADC12_0_INST);

          while (1)
          {
            /* 等待所有数据通道加载完成 */
            while (gCheckADC == false)
            {
              __WFE();
            }

            /* 存储ADC结果到相应的缓冲区 */
            dacData[i] =
                DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);
            opaData[i] =
                DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_1);

            i++;
            gCheckADC = false;
            /* 重置缓冲区索引，设置断点检查缓冲区 */
            if (i >= ADC_SAMPLE_SIZE)
            {
              i = 0;
              // 计算电压峰-峰值

              if (jj == -1)
              {
                dacData_Vpp = calculateVpp((uint16_t *)dacData, 0);
                opaData_Vpp =
                    calculateVpp((uint16_t *)opaData, 1) * vppCompensate;
                uint8_t flag = judRange();
                delay_ms(30);
                // 根据信号范围调整测量范围
                if (flag == 0)
                {
                  jj++;
                }
                else if (flag == 1)
                {
                  DL_ADC12_enableConversions(ADC12_0_INST);
                  continue;
                }
                else
                {
                  jj++;
                }
              }
              // 调整增益
              if (kk == -1 && adjustGain())
              {
                dacData_Vpp = calculateVpp((uint16_t *)dacData, 0);
                opaData_Vpp =
                    calculateVpp((uint16_t *)opaData, 1) * vppCompensate;
                kk++;
                delay_ms(30);
                DL_ADC12_enableConversions(ADC12_0_INST);
                continue;
              }
              if (gain > 16)
              {
                // opaData_Vpp = calculateVpp((uint16_t *)opaData, 2);
                opaData_Vpp =
                    calculateVpp((uint16_t *)opaData, 1) * vppCompensate;
              }
              else
              {
                opaData_Vpp = calculateVpp((uint16_t *)opaData, 0);
              }

              opaData_Vpp /= gain;
              if (opaData_Vpp < 100 && flagRG == FLAG_RG0)
              {
                opaData_Vpp = pow(opaData_Vpp, 2) * -0.0043 +
                              1.4194 * opaData_Vpp + 0.0028;
                // opaData_Vpp = pow(opaData_Vpp, 2) * -0.0025 + 1.2099 *
                // opaData_Vpp + 1.9851;
              }
              // 计算平均值，用于波形整形
              dacData_Mean = DataMean((uint16_t *)dacData);
              opaData_Mean = DataMean((uint16_t *)opaData);

              // 波形整形
              waveShaping((uint16_t *)dacData, dacData_Mean);

              // 相位检测

              phase = phaseDetecter((uint16_t *)opaData, (uint16_t *)dacData,
                                    opaData_Mean) -
                      phaseCompensate;
              phase -= gain0 / 1000 * 2 * M_PI * 2;

              // 计算正弦、余弦相位
              phaseSin = asin(M_PI * opa90 / (opaData_Vpp * gain / 2)) -
                         phaseCompensate - phaseCompensateOPA;
              phaseCos = acos(M_PI * opa0 / (opaData_Vpp * gain / 2)) -
                         phaseCompensate - phaseCompensateOPA;

              // 相位补偿：拟合
              phase = phase - (0.0198 * pow(phase, 3) - 0.0548 * pow(phase, 2) +
                               0.0147 * phase + 0.0187);

              phaseBuffer[kk] = phase;
              VppBuffer[kk++] = opaData_Vpp;

              if (kk == bufferLength)
              {
                phase = findMean(phaseBuffer, bufferLength);
                opaData_Vpp = findMean(VppBuffer, bufferLength);

                judRLC();
                // __BKPT(0);
                // uartPrintf("phase = %lf\n", phase);
                // uartPrintf("vpp = %lf\n", opaData_Vpp);
                // flagElement = FLAG_C;
                for (int i = 0; i < bufferLength; i++)
                {
                  phase = phaseBuffer[i];
                  opaData_Vpp = VppBuffer[i];
                  calculate_LCR();
                  if (flagElement == FLAG_L)
                    bufferData[i] = L;
                  else if (flagElement == FLAG_C)
                    bufferData[i] = C;
                  else
                    bufferData[i] = R;
                  bufferDataESR[i] = ESR;
                  // uartPrintf("opaVpp = %lf\n", opaData_Vpp);
                }
                // 重新初始化变量
                kk = -1;
                jj = -1;

                // 计算缓冲区数据的中值
                midData = findMean(bufferData, bufferLength);
                // meanData = findMean(bufferData, bufferLength); //
                // 注：未使用计算的平均值

                // 计算ESR数据的中值
                midDataESR = findMedian(bufferDataESR, bufferLength);

                if (flagElement == FLAG_C && midData < 20e-12)
                  flagElement = 0;
                // if (flagElement == FLAG_L && midData < 1e-3)
                //     midData = bufferData[bufferLength - 1];
                display();

                // 重置测量档位标志
                flagRG = FLAG_RG0;
                // 重置档位
                controlRG();
                // 设置OPA的增益为N1_P2
                DL_OPA_setGain(OPA_0_INST, DL_OPA_GAIN_N1_P2);
                DL_OPA_setGain(OPA_1_INST, DL_OPA_GAIN_N1_P2);
                // 重置增益值
                gain0 = 2;
                gain1 = 2;
                gain = gain1 * gain0;

                // 切换至下一个测量阶段
                flagElement = 0;

                // 启用ADC转换
                DL_ADC12_enableConversions(ADC12_0_INST);

                // 关闭继电器
                controlRelay(0);
                // 重新启动TIMER_0计数器，开始新的测量循环，计时30s
                DL_TimerG_startCounter(TIMER_0_INST);
                delay_ms(10);

                if (modeFlag == MODE_MEASURE)
                  goto START;
                else
                {
                  delay_ms(2000);
                  goto START;
                }
                // goto LOOP;
              }

              delay_ms(10);
            }
            else
            {
              ; /*No action required*/
            }
          }
        }
      }
    }
  }
}
void UART_0_INST_IRQHandler(void)
{
  switch (DL_UART_Main_getPendingInterrupt(UART_0_INST))
  {
  case DL_UART_MAIN_IIDX_EOT_DONE:
    gCheckUART = true;
    break;
  case DL_UART_MAIN_IIDX_DMA_DONE_TX:
    gDMADone = true;
    break;
  case DL_UART_MAIN_IIDX_DMA_DONE_RX:
    gCheckUARTreceive = true;
    break;
  default:
    break;
  }
}

void ADC12_0_INST_IRQHandler(void)
{
  switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST))
  {
  case DL_ADC12_IIDX_DMA_DONE:
    gCheckADC = true;
    break;

  case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
    gCheckADC = true;
    break;

  case DL_ADC12_IIDX_MEM1_RESULT_LOADED:
    gCheckADC = true;
    break;

  default:
    break;
  }
}

void ADC12_1_INST_IRQHandler(void)
{
  switch (DL_ADC12_getPendingInterrupt(ADC12_1_INST))
  {
  case DL_ADC12_IIDX_DMA_DONE:
    gCheckADC = true;
    break;

  case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
    gCheckADC = true;
    break;

  case DL_ADC12_IIDX_MEM1_RESULT_LOADED:
    gCheckADC = true;
    break;

  default:
    break;
  }
}

void DAC12_IRQHandler(void)
{
  switch (DL_DAC12_getPendingInterrupt(DAC0))
  {
  case DL_DAC12_IIDX_FIFO_1_2_EMPTY:
  case DL_DAC12_IIDX_FIFO_3_4_EMPTY:
  case DL_DAC12_IIDX_NO_INT:
  case DL_DAC12_IIDX_MODULE_READY:
  case DL_DAC12_IIDX_FIFO_FULL:
  case DL_DAC12_IIDX_FIFO_1_4_EMPTY:
  case DL_DAC12_IIDX_FIFO_EMPTY:
  case DL_DAC12_IIDX_FIFO_UNDERRUN:
  case DL_DAC12_IIDX_DMA_DONE:

    break;
  }
}

/**
 * @brief 处理GROUP1中断请求
 *
 * 该函数用于处理来自GROUP1的中断请求。具体来说，它首先检查是哪个中断源触发了中断，
 * 然后针对不同的中断源执行相应的处理逻辑。这里的中断源主要指GPIO键盘中断。
 */

void GROUP1_IRQHandler(void)
{
  // 检查GROUP1中哪个中断源触发了中断
  switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1))
  {
  case GPIO_KEY_INT_IIDX:
    // 处理GPIO键盘中断
    switch (DL_GPIO_getPendingInterrupt(GPIO_KEY_PORT))
    {
    case GPIO_KEY_key0_IIDX:
      // 处理键盘键0的按下和释放中断
      /* 检测到键0按下 */
      delay_ms(10); // 延时去抖动
      if (DL_GPIO_readPins(GPIO_KEY_PORT, GPIO_KEY_key0_PIN))
      {
        delay_ms(10); // 延时去抖动
        if (!DL_GPIO_readPins(GPIO_KEY_PORT, GPIO_KEY_key0_PIN))
          return; // 确认键0确实被按下

        if (key0.KeyNumberContinuous == 0)
          key0.KeyNumberContinuous++;
      }
      /* 检测到键0释放 */
      else
      {
        delay_ms(10); // 延时去抖动
        if (DL_GPIO_readPins(GPIO_KEY_PORT, GPIO_KEY_key0_PIN))
          return; // 确认键0确实被释放
        // 更新按键状态和计数，双击
        if (key0.flagPeriod < DOUBLE_CLICK && key0.KeyState)
        {
          // 停止定时器1的计数
          DL_TimerG_stopCounter(TIMER_1_INST);
          // 重置定时器1的加载值
          DL_TimerG_setLoadValue(TIMER_1_INST,
                                 DL_TimerG_getLoadValue(TIMER_1_INST));
          key0.KeyNumberContinuous++; // 持续按下的计数
          key0.KeyAlready = 1;
          key0.KeyState = 0;
        }
        else
        {
          key0.KeyState = 1;                    // 设置为按下状态
          DL_TimerG_startCounter(TIMER_1_INST); // 启动计时器
        }
      }
      return;
    default:
      // 对于其他中断源，这里不做处理
      break;
    }
  }
}

/**
 * @brief TIMER_1_INST_IRQHandler函数是定时器1的中断服务函数。
 * 这个函数用于处理定时器1产生的中断，其中主要逻辑是检测按键0的状态，
 * 并根据按键0的状态执行相应的操作。
 */

void TIMER_1_INST_IRQHandler(void)
{
  // 根据定时器1的中断源进行处理
  switch (DL_TimerG_getPendingInterrupt(TIMER_1_INST))
  {
  case DL_TIMER_IIDX_ZERO:
    // 处理定时器计数到零的中断
    // 检查是否检测到key0按键按下
    if (key0.KeyState)
    {
      // 如果按键按下，增加按键0的周期计数
      key0.flagPeriod++;

      // 检查是否满足长按条件
      if (key0.flagPeriod >= LONG_CLICK)
      {
        // 停止定时器1的计数
        DL_TimerG_stopCounter(TIMER_1_INST);
        // 重置定时器1的加载值
        DL_TimerG_setLoadValue(TIMER_1_INST,
                               DL_TimerG_getLoadValue(TIMER_1_INST));
        if (key0.KeyState)
        {
          // 重置周期计数和设置按键已按下标志
          key0.flagPeriod = 0;
          key0.KeyState = 0;
          key0.KeyAlready = 1;
          key0.Continuous = 1;
        }
      }
      // 单击情况
      else if (key0.flagPeriod >= DOUBLE_CLICK &&
               key0.KeyNumberContinuous == 1)
      {
        // 停止定时器1的计数
        DL_TimerG_stopCounter(TIMER_1_INST);
        // 重置定时器1的加载值
        DL_TimerG_setLoadValue(TIMER_1_INST,
                               DL_TimerG_getLoadValue(TIMER_1_INST));
        key0.flagPeriod = 0;
        key0.KeyState = 0;
        key0.KeyAlready = 1;
      }
    }
    else
    {
      // 按键未按下时的空操作
    }
    break;
  default:
    // 处理其他未定义的中断情况
    break;
  }
}

void TIMER_0_INST_IRQHandler(void)
{
  switch (DL_TimerG_getPendingInterrupt(TIMER_0_INST))
  {
  case DL_TIMER_IIDX_ZERO:
    DL_GPIO_clearPins(GPIO_LEDS_LED_Sleep_PORT, GPIO_LEDS_LED_Sleep_PIN);

    // DL_SYSCTL_RESET_POR;

    break;
  default:
    break;
  }
}
