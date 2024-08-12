#ifndef __IIC_H
#define __IIC_H

#include "ti_msp_dl_config.h"

// PA12
#define IIC_SCL_L DL_GPIO_clearPins(GPIO_IIC_PORT, GPIO_IIC_SCL_PIN);//SCL IIC�ӿڵ�ʱ���ź�
#define IIC_SCL_H DL_GPIO_setPins(GPIO_IIC_PORT, GPIO_IIC_SCL_PIN);

// PA13
#define IIC_SDA_L DL_GPIO_clearPins(GPIO_IIC_PORT, GPIO_IIC_SDA_PIN);//SDA IIC�ӿڵ������ź�
#define IIC_SDA_H DL_GPIO_setPins(GPIO_IIC_PORT, GPIO_IIC_SDA_PIN);

#define  I2C_SDA_READ()  DL_GPIO_readPins(GPIO_IIC_PORT, GPIO_IIC_SDA_PIN)  //��SDA����״̬

void IIC_Init(void);         //IIC��ʼ������
void IIC_Idle_State(void);   //IIC����״̬
void IIC_Start(void);        //IIC��ʼ�źź���
void IIC_Stop(void);         //IICֹͣ�źź���
void IIC_SendByte(uint8_t data);  //IIC����һ���ֽ�
uint8_t   IIC_ReadByte(void);     //IIC��ȡһ���ֽ�
uint8_t   IIC_WaitAck(void);      //�ȴ���Ӧ�źţ�ACK����NACK��
void IIC_ACK(void);          //IIC����ACK�ź�
void IIC_NACK(void);         //IIC����NACK�ź�


#endif
