#include "ti_msp_dl_config.h"

#define MVF_LENGTH 16

void moving_average_filter_double(double *data, int length)
{
    uint8_t p1 = 0, p2 = MVF_LENGTH - 1;
    double sumWindow = 0;

    for (int j = p1; j <= p2; j++)
    {
        sumWindow += data[j];
        data[MVF_LENGTH / 2 - 1] = sumWindow / MVF_LENGTH;
    }
    for (int i = MVF_LENGTH / 2; i < length - MVF_LENGTH / 2; i++)
    {
        sumWindow -= data[p1++];
        sumWindow += data[++p2];
        data[i] = sumWindow / MVF_LENGTH;
    }
}

void moving_average_filter_u16(uint16_t *data, int length)
{
    uint8_t p1 = 0, p2 = MVF_LENGTH - 1;
    uint16_t sumWindow = 0;

    for (int j = p1; j <= p2; j++)
    {
        sumWindow += data[j];
        data[MVF_LENGTH / 2 - 1] = sumWindow / MVF_LENGTH;
    }
    for (int i = MVF_LENGTH / 2; i < length - MVF_LENGTH / 2; i++)
    {
        sumWindow -= data[p1++];
        sumWindow += data[++p2];
        data[i] = sumWindow / MVF_LENGTH;
    }
}

/**
 * 对给定的整数数组应用移动平均滤波器。
 *
 * @param data 指向要处理的整数数组的指针。
 * @param length 数组的长度。
 *
 * 该函数通过计算一个固定大小的窗口内的数值总和，然后将这个总和平均分配到窗口外的元素中，
 * 以此来平滑数据序列。窗口的大小由MVF_LENGTH定义。
 */
void moving_average_filter_u32(uint32_t *data, int length)
{
    /* 初始化两个指针，p1指向窗口的起始位置，p2指向窗口的结束位置。 */
    uint8_t p1 = 0, p2 = MVF_LENGTH - 1;
    /* 初始化窗口内数值的总和。 */
    uint32_t sumWindow = 0;

    /* 对窗口内的初始数值进行累加，为后续的平均计算做准备。 */
    for (int j = p1; j <= p2; j++)
    {
        sumWindow += data[j];
        /* 将窗口内的总和平均分配到特定的位置。注释掉的代码表示另一种平均方法。 */
        // data[MVF_LENGTH / 2 - 1] = sumWindow / MVF_LENGTH;
        data[MVF_LENGTH / 2 - 1] = sumWindow >> 4;
    }

    /* 对窗口外的数值，每次移动窗口，更新总和，并将新的平均值赋给当前位置。 */
    for (int i = MVF_LENGTH / 2; i < length - MVF_LENGTH / 2; i++)
    {
        /* 移动窗口前，从窗口总和中减去窗口前的数值。 */
        sumWindow -= data[p1++];
        /* 移动窗口后，将新加入的数值加到窗口总和中。 */
        sumWindow += data[++p2];
        /* 将更新后的窗口总和平均分配到当前位置。注释掉的代码表示另一种平均方法。 */
        // data[i] = sumWindow / MVF_LENGTH;
        data[i] = sumWindow >> 4;
    }
}
