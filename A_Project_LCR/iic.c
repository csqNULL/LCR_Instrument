
#include "iic.h"
#include "delay.h"

/**
 * @brief 进入IIC空闲状态
 *
 * 该函数用于将IIC总线设置为空闲状态，通过拉高SDA和SCL线，并且加入一定的延时来实现。
 * 不接受任何参数。
 * 不返回任何值。
 */
void IIC_Idle_State()
{
    // 拉高SDA和SCL线，进入空闲状态
    IIC_SDA_H;
    IIC_SCL_H;
    // 加入4us的延时，确保总线状态稳定
    delay_us(4);
}

/**
 * @brief 初始化IIC通信，发送起始信号。
 *
 * 该函数负责在IIC总线上发出起始信号，为后续的IIC通信做好准备。
 * 起始信号的规格是在SCL为高电平时，SDA由高电平向低电平的跳变。
 *
 * @note 函数中使用了延时来确保信号的稳定，延时的长度对IIC通信的成功与否有一定影响。
 */
void IIC_Start()
{
    // 首先将SDA和SCL线都设置为高电平
    IIC_SDA_H;
    IIC_SCL_H;

    // 短延时，确保电平稳定
    delay_us(4);

    // 发起始信号：SDA线首先下降到低电平
    IIC_SDA_L;
    delay_us(4); // 确保SDA线低电平稳定

    // SCL线随后下降到低电平，完成起始信号的发送
    IIC_SCL_L;
    delay_us(4); // 延时保证信号稳定
}

/**
 * @brief 发送IIC停止条件
 *
 * 该函数用于在IIC通信中生成停止条件，即SDA线和SCL线的特定时序。停止条件是IIC通信中必须的组成部分，用于结束一次数据传输。
 *
 * 参数：无
 *
 * 返回值：无
 */
void IIC_Stop()
{
    // 置低SDA线，保持SCL线为高，产生IIC停止条件的前半部分
    IIC_SDA_L;
    IIC_SCL_H;

    // 微小延迟，确保时序的正确性
    delay_us(4);

    // 将SDA线拉高，完成停止条件的生成
    IIC_SDA_H;
}

/**
 * @brief 发送一个字节的数据通过IIC总线
 * @param data 要发送的字节数据
 *
 * 该函数通过IIC总线发送一个字节的数据。数据发送是逐位进行的，每发送一位，都会经历一个IIC时钟周期。
 * 在每一位的发送过程中，首先根据当前位的数据（bit7）决定SDA线是保持高电平还是低电平。
 * 然后，通过控制SCL线的高电平和低电平来完成一位数据的传输。在最后一位数据发送完成后，
 * 需要将SDA线拉高，以准备后续的IIC操作。
 */
void IIC_SendByte(uint8_t data)
{
    uint8_t i;
    // 循环发送数据的8位
    for (i = 0; i < 8; i++)
    {
        if (data & 0x80) // 检查当前位是否为1
        {
            IIC_SDA_H; // SDA线置高
        }
        else
        {
            IIC_SDA_L; // SDA线置低
        }

        delay_us(1); // 等待足够的时间保证信号稳定

        IIC_SCL_H; // SCL线置高，开始一位数据的传输
        delay_us(1);
        IIC_SCL_L; // SCL线置低，结束一位数据的传输

        if (i == 7) // 在最后一位数据发送完成后，准备释放SDA线
        {
            IIC_SDA_H; // 将SDA线拉高
        }

        data <<= 1;  // 将数据左移一位，准备发送下一位
        delay_us(1); // 等待足够的时间保证信号稳定
    }
}

/**
 * 从IIC总线上读取一个字节的数据。
 * 该函数不接受参数。
 *
 * @return 返回从IIC总线上读取的8位数据。
 */
uint8_t IIC_ReadByte(void)
{
    uint8_t i;
    uint8_t value;

    // 初始化读取值为0，并依次左移以接收每个位的数据
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;        // 左移一位，准备接收下一位数据
        IIC_SCL_H;          // 将SCL线置高电平，开始一个时钟周期
        delay_us(4);        // 等待一定时间，保证时钟周期
        if (I2C_SDA_READ()) // 读取SDA线上的数据
        {
            value++; // 如果SDA为高，则接收的位为1
        }
        IIC_SCL_L;   // 将SCL线置低电平，结束一个时钟周期
        delay_us(4); // 等待一定时间，进行时钟周期间的间隔
    }
    return value; // 返回接收到的8位数据
}

/**
 * @brief 等待I2C从机发送ACK信号
 *
 * 此函数用于在I2C通信中，主机发送一个字节数据后，等待从机发送ACK信号，并根据ACK信号的情况返回相应的值。
 *
 * @return uint8_t 返回等待ACK的结果：
 *         - 1: 表示从机未发送ACK信号（ACK位为高电平）。
 *         - 0: 表示从机正常发送ACK信号（ACK位为低电平）。
 */
uint8_t IIC_WaitAck(void)
{
    uint8_t rvalue;

    // 设置SDA为高电平，预释放总线
    IIC_SDA_H;
    delay_us(4); // 等待SDA信号稳定

    // 将SCL设为高电平，开始ACK检测阶段
    IIC_SCL_H;
    delay_us(4); // 等待足够时间以确保时序正确

    // 读取SDA线的状态，以判断从机是否发送了ACK
    if (I2C_SDA_READ())
    {
        // 如果SDA为高电平，则表示从机未发送ACK
        rvalue = 1;
    }
    else
    {
        // 如果SDA为低电平，则表示从机发送了ACK
        rvalue = 0;
    }

    // 将SCL设为低电平，结束ACK检测阶段
    IIC_SCL_L;
    delay_us(4); // 等待时序完成

    return rvalue;
}

/**
 * @brief 发送I2C应答信号
 *
 * 该函数用于在I2C通信中发送一个应答信号。在I2C通信协议中，接收到数据的一方在接收到数据后，需要通过SDA线拉低来发送一个ACK(应答)信号，
 * 表示数据已被正确接收。这个函数实现了这个功能。
 *
 * @note 该函数不接受参数，也不返回任何值。
 */
void IIC_ACK(void)
{
    // 将SDA线拉低，准备发送ACK信号
    IIC_SDA_L;
    delay_us(4); // 等待一段时间，保证SDA线稳定在低电平
    // 将SCL线拉高，开始ACK信号的传输
    IIC_SCL_H;
    delay_us(4); // 短暂延迟，确保时钟信号被拉高并稳定
    // 将SCL线拉低，结束ACK信号的传输
    IIC_SCL_L;
    delay_us(4); // 再次等待一段时间，以满足时序要求
    // 将SDA线拉高，结束ACK信号的发送
    IIC_SDA_H;
}

/**
 * @brief 发送I2C总线的NACK(Not Acknowledge)信号
 *
 * 该函数用于在I2C通信中向对方设备发送一个NACK信号，表明数据未被正确接收或不进行进一步的数据传输。
 * 这是通过控制I2C总线的SDA和SCL线的电平来实现的。
 *
 * @note 该函数不接受参数，也不返回任何值。
 */
void IIC_NACK(void)
{
    // 将SDA线拉高，预准备发送NACK信号
    IIC_SDA_H;
    delay_us(4); // 等待稳定时间

    // 将SCL线拉高，开始一个时钟周期
    IIC_SCL_H;
    delay_us(4); // 等待一个时钟周期的时间

    // 将SCL线拉低，结束当前时钟周期，表示NACK信号
    IIC_SCL_L;
    delay_us(4); // 等待稳定时间，为下一个操作做准备
}
