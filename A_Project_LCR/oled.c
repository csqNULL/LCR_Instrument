#include "oled.h"
#include "oledfont.h"
#include "iic.h"
#include "ti_msp_dl_config.h"
#include "delay.h"

uint8_t OLED_GRAM[144][8];

/**
 * @brief 切换OLED显示颜色模式
 *
 * 该函数用于切换OLED屏幕的显示颜色模式。根据输入参数的不同，可以选择正常颜色模式或反转颜色模式。
 *
 * @param i 模式切换控制参数
 *        - 当 i 为 0 时，切换为正常颜色模式；
 *        - 当 i 为 1 时，切换为反转颜色模式。
 */
void OLED_ColorTurn(uint8_t i)
{
    // 当i为0时，发送指令切换到正常颜色模式
    if (i == 0)
    {
        OLED_WR_Byte(0xA6, OLED_CMD);
    }
    // 当i为1时，发送指令切换到反转颜色模式
    if (i == 1)
    {
        OLED_WR_Byte(0xA7, OLED_CMD);
    }
}

/**
 * @brief 实现OLED屏幕的翻转
 *
 * 该函数用于根据输入参数i的值，将OLED屏幕显示内容进行180度的翻转。
 *
 * @param i 控制翻转方向的参数，取值为0或1。
 *        - 当i为0时，执行屏幕顺时针翻转。
 *        - 当i为1时，执行屏幕逆时针翻转。
 */
void OLED_DisplayTurn(uint8_t i)
{
    // 当参数i为0时，执行顺时针翻转的命令
    if (i == 0)
    {
        OLED_WR_Byte(0xC8, OLED_CMD); // 设置显示翻转，使能屏幕反向显示
        OLED_WR_Byte(0xA1, OLED_CMD);
    }
    // 当参数i为1时，执行逆时针翻转的命令
    if (i == 1)
    {
        OLED_WR_Byte(0xC0, OLED_CMD); // 设置显示翻转，使能屏幕正向显示
        OLED_WR_Byte(0xA0, OLED_CMD);
    }
}

// ����һ���ֽ�
/**
 * @brief 向OLED设备写入一个字节数据。
 *
 * 此函数用于根据指定的模式，向OLED设备写入一个字节的数据。模式决定数据写入的地址是显示内存的起始地址
 * 还是命令的寄存器。在模式为0时，写入的是显示内存的起始地址0x00；在模式为1时，写入的是显示内存的起始地址0x40。
 *
 * @param dat 需要写入的字节数据。
 * @param mode 写入模式，0表示写入显示内存地址0x00，1表示写入显示内存地址0x40。
 */
void OLED_WR_Byte(uint8_t dat, uint8_t mode)
{
    IIC_Start();        // 开始IIC通信
    IIC_SendByte(0x78); // 发送OLED设备地址
    IIC_WaitAck();      // 等待应答
    if (mode)           // 判断写入模式
    {
        IIC_SendByte(0x40); // 如果是模式1，发送显示内存的起始地址0x40
    }
    else // 模式0，发送显示内存的起始地址0x00
    {
        IIC_SendByte(0x00);
    }
    IIC_WaitAck();     // 等待应答
    IIC_SendByte(dat); // 发送待写入的数据
    IIC_WaitAck();     // 等待应答
    IIC_Stop();        // 结束IIC通信
}

// OLED显示开启函数
// 该函数用于打开OLED显示器，使其处于显示状态。
// 参数：无
// 返回值：无
void OLED_DisPlay_On(void)
{
    OLED_WR_Byte(0x8D, OLED_CMD);
    OLED_WR_Byte(0x14, OLED_CMD);
    OLED_WR_Byte(0xAF, OLED_CMD);
}

// 关闭OLED显示
void OLED_DisPlay_Off(void)
{
    OLED_WR_Byte(0x8D, OLED_CMD);
    OLED_WR_Byte(0x10, OLED_CMD);
    OLED_WR_Byte(0xAE, OLED_CMD);
}

/*
 * 功能：刷新OLED显示
 * 参数：无
 * 返回值：无
 */
void OLED_Refresh(void)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)
    {
        // 设置当前页面起始地址
        OLED_WR_Byte(0xb0 + i, OLED_CMD);
        // 设置列地址起始点
        OLED_WR_Byte(0x00, OLED_CMD);
        // 设置列地址结束点
        OLED_WR_Byte(0x10, OLED_CMD);
        // 发起I2C开始信号
        IIC_Start();
        // 发送设备地址和写指令
        IIC_SendByte(0x78);
        IIC_WaitAck();
        IIC_SendByte(0x40);
        IIC_WaitAck();
        // 遍历并发送页面中的每个像素点数据
        for (n = 0; n < 128; n++)
        {
            IIC_SendByte(OLED_GRAM[n][i]);
            IIC_WaitAck();
        }
        // 发起I2C停止信号
        IIC_Stop();
    }
}

/**
 * @brief 清除OLED显示屏幕的内容。
 * 该函数遍历OLED显示缓冲区，将所有像素点设置为黑色（即关闭像素），从而实现屏幕清空的效果。
 * 清空后，调用OLED_Refresh()函数更新屏幕显示，确保清空操作的可见性。
 *
 */
void OLED_Clear(void)
{
    uint8_t i, n;
    // 遍历OLED显示缓冲区的每一行和每一列，将所有像素点设置为关闭状态
    for (i = 0; i < 8; i++)
    {
        for (n = 0; n < 128; n++)
        {
            OLED_GRAM[n][i] = 0; // 将像素点设置为关闭状态
        }
    }
    // 更新屏幕显示，确保清空操作的结果能够被视觉感知
    OLED_Refresh();
}

// x:0~127
// y:0~63
// OLED_DrawPoint: 在OLED屏幕上绘制一个点
// 参数:
//   x: 点的x坐标
//   y: 点的y坐标
//   t: 控制点的状态，1表示绘制亮点，0表示绘制暗点
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t)
{
    uint8_t i, m, n;
    // 计算点所在的页和行
    i = y / 8;
    m = y % 8;
    // 计算位掩码，以便对单个位进行操作
    n = 1 << m;
    if (t) // 如果绘制亮点
    {
        OLED_GRAM[x][i] |= n; // 在GRAM中设置相应位为1
    }
    else // 如果绘制暗点
    {
        // 先将位设置为1，然后设置为0，以确保该位被置为0
        OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
        OLED_GRAM[x][i] |= n;
        OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
    }
}

/**
 * @brief 在OLED屏幕上绘制一条线段。
 *
 * @param x1 起始点的X坐标。
 * @param y1 起始点的Y坐标。
 * @param x2 终点的X坐标。
 * @param y2 终点的Y坐标。
 * @param mode 绘制模式，具体模式含义根据实现而定。
 */
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t mode)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;

    // 计算线段的长度并确定起点
    delta_x = x2 - x1;
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;

    // 根据起点和终点的X坐标确定递增方向
    if (delta_x > 0)
        incx = 1;
    else if (delta_x == 0)
        incx = 0;
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    // 根据起点和终点的Y坐标确定递增方向
    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0;
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    // 根据斜率确定最长的边，用于Bresenham算法
    if (delta_x > delta_y)
        distance = delta_x;
    else
        distance = delta_y;

    // Bresenham算法绘制线段
    for (t = 0; t < distance + 1; t++)
    {
        OLED_DrawPoint(uRow, uCol, mode); // 在当前点绘制像素

        // 更新误差项并根据误差调整下个像素点的位置
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}

/**
 * 在OLED屏幕上绘制一个圆。
 *
 * @param x 圆心的x坐标。
 * @param y 圆心的y坐标。
 * @param r 圆的半径。
 *
 * 该函数使用Midpoint Circle Algorithm（中点圆算法）来绘制一个圆。
 * 它通过迭代的方式，从圆心向外部逐渐绘制圆的边缘。
 */
void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r)
{
    int a, b, num;
    a = 0;
    b = r;

    // 使用中点圆算法的核心循环，绘制圆的八分之一，然后通过对称性扩展到整个圆。
    while (2 * b * b >= r * r)
    {
        // 绘制圆的上、下、左、右四个方向的点
        OLED_DrawPoint(x + a, y - b, 1);
        OLED_DrawPoint(x - a, y - b, 1);
        OLED_DrawPoint(x - a, y + b, 1);
        OLED_DrawPoint(x + a, y + b, 1);

        // 绘制圆的四个斜向上的点
        OLED_DrawPoint(x + b, y + a, 1);
        OLED_DrawPoint(x + b, y - a, 1);
        OLED_DrawPoint(x - b, y - a, 1);
        OLED_DrawPoint(x - b, y + a, 1);

        a++;
        // 计算是否需要减小b的值，以确保绘制的点始终在圆内。
        num = (a * a + b * b) - r * r;
        if (num > 0)
        {
            b--;
            a--;
        }
    }
}

// x:0~127
// y:0~63
// size1:6x8/6x12/8x16/12x24
/**
 * 在OLED屏幕上显示一个字符。
 *
 * @param x 字符的起始x坐标。
 * @param y 字符的起始y坐标。
 * @param chr 要显示的字符。
 * @param size1 字符的大小。不同大小对应不同的字体表。
 * @param mode 显示模式：0表示正常显示，1表示反色显示。
 */
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size1, uint8_t mode)
{
    uint8_t i, m, temp, size2, chr1;
    uint8_t x0 = x, y0 = y;

    // 根据字符大小计算实际占用的行数或列数
    if (size1 == 8)
        size2 = 6;
    else
        size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2);

    // 将字符映射到对应的ASCII码值
    chr1 = chr - ' ';

    // 遍历字符的图形数据
    for (i = 0; i < size2; i++)
    {
        // 根据字符大小选择相应的字体表
        if (size1 == 8)
        {
            temp = asc2_0806[chr1][i];
        }
        else if (size1 == 12)
        {
            temp = asc2_1206[chr1][i];
        }
        else if (size1 == 16)
        {
            temp = asc2_1608[chr1][i];
        }
        else if (size1 == 24)
        {
            temp = asc2_2412[chr1][i];
        }
        else
            return; // 不支持的字符大小，直接返回

        // 在屏幕上绘制字符的每一个像素点
        for (m = 0; m < 8; m++)
        {
            if (temp & 0x01)
                OLED_DrawPoint(x, y, mode);
            else
                OLED_DrawPoint(x, y, !mode);
            temp >>= 1;
            y++;
        }
        x++;

        // 当绘制到字符的宽度时，换行
        if ((size1 != 8) && ((x - x0) == size1 / 2))
        {
            x = x0;
            y0 = y0 + 8;
        }
        y = y0;
    }
}

/**
 * 在OLED屏幕上显示一串字符。
 *
 * @param x 字符起始显示的X坐标。
 * @param y 字符起始显示的Y坐标。
 * @param chr 指向要显示的字符串的指针。
 * @param size1 字符的大小，决定每个字符占用的像素宽度。
 * @param mode 显示模式：0为正常显示，1为高亮显示。
 */
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t size1, uint8_t mode)
{
    // 遍历字符串中的每个字符，判断是否为可见字符（空格到波浪号）
    while ((*chr >= ' ') && (*chr <= '~'))
    {
        // 在指定位置显示当前字符
        OLED_ShowChar(x, y, *chr, size1, mode);
        // 根据字符大小更新x坐标，准备显示下一个字符
        if (size1 == 8)
            x += 6;
        else
            x += size1 / 2;
        // 移动到字符串的下一个字符
        chr++;
    }
}

/**
 * 计算 m 的 n 次方
 *
 * 该函数用于计算一个 uint8_t 类型的数 m 的另一个 uint8_t 类型的数 n 的次方，
 * 并将计算结果以 uint32_t 类型返回。该函数特别适用于 OLED 显示设备中的一些幂运算场景。
 *
 * @param m 底数，一个 uint8_t 类型的数。
 * @param n 指数，一个 uint8_t 类型的数。
 * @return 计算结果，一个 uint32_t 类型的数，即 m 的 n 次方。
 */
uint32_t OLED_Pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1; // 初始化结果为 1
    while (n--)          // 循环 n 次，每次将 m 乘以结果
    {
        result *= m; // 累乘 m 到结果上
    }
    return result; // 返回最终计算结果
}

/**
 * 在OLED屏幕上显示一个数字。
 *
 * @param x 字符显示的起始x坐标。
 * @param y 字符显示的起始y坐标。
 * @param num 要显示的数字。
 * @param len 要显示的数字的位数。如果数字位数少于len，则用0填充。
 * @param size1 字体的大小。8表示大字体，其他值表示小字体。
 * @param mode 显示模式：0表示正常显示，1表示高亮显示。
 */
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size1, uint8_t mode)
{
    uint8_t t, temp, m = 0; // 初始化变量
    // 根据字体大小设置间距调整参数m
    if (size1 == 8)
        m = 2;
    // 循环遍历要显示的数字的每一位
    for (t = 0; t < len; t++)
    {
        // 计算当前位的数字
        temp = (num / OLED_Pow(10, len - t - 1)) % 10;
        // 对于非零数字，调用函数显示；否则显示'0'
        if (temp == 0)
        {
            OLED_ShowChar(x + (size1 / 2 + m) * t, y, '0', size1, mode);
        }
        else
        {
            OLED_ShowChar(x + (size1 / 2 + m) * t, y, temp + '0', size1, mode);
        }
    }
}

/**
 * 在OLED屏幕上显示中文字符
 *
 * @param x 字符起始点的X坐标
 * @param y 字符起始点的Y坐标
 * @param num 要显示的中文字符的序号
 * @param size1 字符的大小，单位为像素。支持的大小有16, 24, 32, 64。
 * @param mode 显示模式：0表示黑色字符在白色背景上，1表示白色字符在黑色背景上。
 */
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t num, uint8_t size1, uint8_t mode)
{
    uint8_t m, temp;
    uint8_t x0 = x, y0 = y;
    // 计算字符占用的总像素数，以确定循环次数
    uint16_t i, size3 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * size1;
    for (i = 0; i < size3; i++)
    {
        // 根据字符大小选择相应的字符数组
        if (size1 == 16)
        {
            temp = Hzk_16[num][i];
        }
        else if (size1 == 24)
        {
            temp = Hzk2[num][i];
        }
        else if (size1 == 32)
        {
            temp = Hzk3[num][i];
        }
        else if (size1 == 64)
        {
            temp = Hzk4[num][i];
        }
        else
            return; // 不支持的字符大小，直接返回
        // 在屏幕上绘制像素点，完成字符的一行绘制
        for (m = 0; m < 8; m++)
        {
            if (temp & 0x01)
                OLED_DrawPoint(x, y, mode);
            else
                OLED_DrawPoint(x, y, !mode);
            temp >>= 1; // 处理下一个位
            y++;        // 移动到下一行
        }
        x++; // 移动到下一列
        // 当绘制完一个字符的宽度时，换行
        if ((x - x0) == size1)
        {
            x = x0;
            y0 = y0 + 8;
        }
        y = y0; // 重置Y坐标，准备绘制下一行
    }
}

/**
 * @brief OLED屏幕滚动显示函数
 *
 * 该函数用于在OLED屏幕上滚动显示一段中文文本。可以通过调整参数来控制显示的速度和模式。
 *
 * @param num 要显示的中文字符数量
 * @param space 滚动之间的空白行数
 * @param mode 显示模式：0表示单行显示，1表示多行显示
 */
void OLED_ScrollDisplay(uint8_t num, uint8_t space, uint8_t mode)
{
    uint8_t i, n, t = 0, m = 0, r;
    while (1) // 持续循环以实现滚动显示效果
    {
        if (m == 0)
        {
            OLED_ShowChinese(128, 24, t, 16, mode); // 显示第t个中文字符
            t++;
        }
        if (t == num) // 当显示完所有指定的中文字符后，开始进行滚动
        {
            for (r = 0; r < 16 * space; r++) // 用于控制滚动过程中的空白区域
            {
                for (i = 1; i < 144; i++) // 将屏幕内容逐行向前移动
                {
                    for (n = 0; n < 8; n++) // 单行数据向前移动
                    {
                        OLED_GRAM[i - 1][n] = OLED_GRAM[i][n];
                    }
                }
                OLED_Refresh(); // 刷新屏幕显示
            }
            t = 0; // 重置显示字符的索引
        }
        m++;
        if (m == 16) // 控制滚动频率，每16次更新完成一次滚动
        {
            m = 0;
        }
        for (i = 1; i < 144; i++) // 实际执行屏幕内容的向前移动
        {
            for (n = 0; n < 8; n++)
            {
                OLED_GRAM[i - 1][n] = OLED_GRAM[i][n];
            }
        }
        OLED_Refresh(); // 刷新屏幕显示以更新滚动后的结果
    }
}

/**
 * 在OLED屏幕上显示一个图片。
 *
 * @param x 图片显示的起始横坐标。
 * @param y 图片显示的起始纵坐标。
 * @param sizex 图片的宽度。
 * @param sizey 图片的高度。注意：由于像素点位的限制，图片高度需要是8的倍数。
 * @param BMP 包含图片数据的数组。
 * @param mode 显示模式：0表示正常显示，1表示反色显示。
 */
void OLED_ShowPicture(uint8_t x, uint8_t y, uint8_t sizex, uint8_t sizey, uint8_t BMP[], uint8_t mode)
{
    uint16_t j = 0;        // 用于遍历BMP数组的索引
    uint8_t i, n, temp, m; // 用于循环和临时数据存储的变量

    uint8_t x0 = x, y0 = y; // 图片显示的起始坐标

    // 计算实际需要遍历的行数，确保高度是8的倍数
    sizey = sizey / 8 + ((sizey % 8) ? 1 : 0);

    for (n = 0; n < sizey; n++) // 遍历图片的每一行
    {
        for (i = 0; i < sizex; i++) // 遍历每一行中的每个像素点
        {
            temp = BMP[j]; // 从BMP数组中获取当前处理的数据字节
            j++;
            for (m = 0; m < 8; m++) // 遍历当前字节中的每个比特位（像素点）
            {
                // 根据mode参数决定是正常显示还是反色显示
                if (temp & 0x01)
                    OLED_DrawPoint(x, y, mode);
                else
                    OLED_DrawPoint(x, y, !mode);
                temp >>= 1; // 处理下一个比特位
                y++;        // 移动到下一像素行
            }
            x++; // 移动到下一列
            // 当达到图片宽度时，换行继续显示
            if ((x - x0) == sizex)
            {
                x = x0;
                y0 = y0 + 8;
            }
            y = y0; // 重置Y坐标，准备下一行的显示
        }
    }
}

void OLED_Init(void)
{
    //    IIC_Init();
    //    GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_15);

    delay_ms(200);

    OLED_WR_Byte(0xAE, OLED_CMD); //---turn off oled panel
    OLED_WR_Byte(0x00, OLED_CMD); //---set low column address
    OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
    OLED_WR_Byte(0x40, OLED_CMD); //---set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_Byte(0x81, OLED_CMD); //---set contrast control register
    OLED_WR_Byte(0xCF, OLED_CMD); //---Set SEG Output Current Brightness
    OLED_WR_Byte(0xA1, OLED_CMD); //---Set SEG/Column Mapping
    OLED_WR_Byte(0xC8, OLED_CMD); //---Set COM/Row Scan Direction
    OLED_WR_Byte(0xA6, OLED_CMD); //---set normal display
    OLED_WR_Byte(0xA8, OLED_CMD); //---set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3f, OLED_CMD); //---1/64 duty
    OLED_WR_Byte(0xD3, OLED_CMD); //---set display offset   Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_Byte(0x00, OLED_CMD); //---not offset
    OLED_WR_Byte(0xd5, OLED_CMD); //---set display clock divide ratio/oscillator frequency
    OLED_WR_Byte(0x80, OLED_CMD); //---set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WR_Byte(0xD9, OLED_CMD); //---set pre-charge period
    OLED_WR_Byte(0xF1, OLED_CMD); //---Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_Byte(0xDA, OLED_CMD); //---set com pins hardware configuration
    OLED_WR_Byte(0x12, OLED_CMD);
    OLED_WR_Byte(0xDB, OLED_CMD); //---set vcomh
    OLED_WR_Byte(0x40, OLED_CMD); //---Set VCOM Deselect Level
    OLED_WR_Byte(0x20, OLED_CMD); //---Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WR_Byte(0x02, OLED_CMD); //
    OLED_WR_Byte(0x8D, OLED_CMD); //---set Charge Pump enable/disable
    OLED_WR_Byte(0x14, OLED_CMD); //---set(0x10) disable
    OLED_WR_Byte(0xA4, OLED_CMD); //---Disable Entire Display On (0xa4/0xa5)
    OLED_WR_Byte(0xA6, OLED_CMD); //---Disable Inverse Display On (0xa6/a7)
    OLED_Clear();
    OLED_WR_Byte(0xAF, OLED_CMD);
}
