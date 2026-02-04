#include "my_filter.h"

/**
 * @brief 对double类型数组应用移动平均滤波器
 *
 * @param data 指向要处理的double数组的指针
 * @param length 数组的长度
 * 
 * @note 滤波器窗口大小为MVF_LENGTH，滤波后数组首尾各MVF_LENGTH/2个元素不变
 */
void moving_average_filter_double(double *data, int length)
{
    int p1 = 0, p2 = MVF_LENGTH - 1;
    double sumWindow = 0;
    const int halfWindow = MVF_LENGTH / 2;

    // 初始化窗口累加值
    for (int j = p1; j <= p2; j++)
    {
        sumWindow += data[j];
    }
    data[halfWindow - 1] = sumWindow / MVF_LENGTH;

    // 滑动窗口进行滤波
    for (int i = halfWindow; i < length - halfWindow; i++)
    {
        sumWindow -= data[p1++];
        sumWindow += data[++p2];
        data[i] = sumWindow / MVF_LENGTH;
    }
}

/**
 * @brief 对uint16_t类型数组应用移动平均滤波器
 *
 * @param data 指向要处理的uint16_t数组的指针
 * @param length 数组的长度
 * 
 * @note 使用uint32_t类型累加器避免溢出
 *       (16个uint16_t最大值65535相加 = 1,048,560，超出uint16_t范围)
 */
void moving_average_filter_u16(uint16_t *data, int length)
{
    int p1 = 0, p2 = MVF_LENGTH - 1;
    uint32_t sumWindow = 0;  // 使用uint32_t避免累加溢出
    const int halfWindow = MVF_LENGTH / 2;

    // 初始化窗口累加值
    for (int j = p1; j <= p2; j++)
    {
        sumWindow += data[j];
    }
    data[halfWindow - 1] = sumWindow / MVF_LENGTH;

    // 滑动窗口进行滤波
    for (int i = halfWindow; i < length - halfWindow; i++)
    {
        sumWindow -= data[p1++];
        sumWindow += data[++p2];
        data[i] = sumWindow / MVF_LENGTH;
    }
}

/**
 * @brief 对uint32_t类型数组应用移动平均滤波器（使用位移优化）
 *
 * @param data 指向要处理的uint32_t数组的指针
 * @param length 数组的长度
 *
 * @note 使用位移操作(>>4)代替除法(/16)以提高效率，要求MVF_LENGTH为16
 */
void moving_average_filter_u32(uint32_t *data, int length)
{
    int p1 = 0, p2 = MVF_LENGTH - 1;
    uint32_t sumWindow = 0;
    const int halfWindow = MVF_LENGTH / 2;

    // 初始化窗口累加值
    for (int j = p1; j <= p2; j++)
    {
        sumWindow += data[j];
    }
    // 使用位移代替除法（MVF_LENGTH=16时，>>4 等价于 /16）
    data[halfWindow - 1] = sumWindow >> 4;

    // 滑动窗口进行滤波
    for (int i = halfWindow; i < length - halfWindow; i++)
    {
        sumWindow -= data[p1++];
        sumWindow += data[++p2];
        data[i] = sumWindow >> 4;
    }
}
