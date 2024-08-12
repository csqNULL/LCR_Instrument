#ifndef __IIC_H
#define __IIC_H

#include "ti_msp_dl_config.h"

// PA12
#define IIC_SCL_L DL_GPIO_clearPins(GPIO_IIC_PORT, GPIO_IIC_SCL_PIN);//SCL IIC接口的时钟信号
#define IIC_SCL_H DL_GPIO_setPins(GPIO_IIC_PORT, GPIO_IIC_SCL_PIN);

// PA13
#define IIC_SDA_L DL_GPIO_clearPins(GPIO_IIC_PORT, GPIO_IIC_SDA_PIN);//SDA IIC接口的数据信号
#define IIC_SDA_H DL_GPIO_setPins(GPIO_IIC_PORT, GPIO_IIC_SDA_PIN);

#define  I2C_SDA_READ()  DL_GPIO_readPins(GPIO_IIC_PORT, GPIO_IIC_SDA_PIN)  //读SDA口线状态

void IIC_Init(void);         //IIC初始化函数
void IIC_Idle_State(void);   //IIC空闲状态
void IIC_Start(void);        //IIC开始信号函数
void IIC_Stop(void);         //IIC停止信号函数
void IIC_SendByte(uint8_t data);  //IIC发送一个字节
uint8_t   IIC_ReadByte(void);     //IIC读取一个字节
uint8_t   IIC_WaitAck(void);      //等待响应信号（ACK或者NACK）
void IIC_ACK(void);          //IIC发出ACK信号
void IIC_NACK(void);         //IIC发出NACK信号


#endif
