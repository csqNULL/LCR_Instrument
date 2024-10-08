# LCR_Instrument

# 简易元件测试仪

## 项目简介

这是一款开源的简易元件测试仪,可以测量电阻、电容、电感和二极管等常见电子元件的基本参数。该测试仪具有以下特点:

- 基于MSPM0G3507 TI官方开发板，最大化利用MCU外设，仅额外使用一个芯片
- 测量范围广泛: 电阻 1Ω\~1MΩ，电容 100pF\~100μF，电感 0.1~10mH
- 测量准确度高: 2 位以上有效数字，典型误差≤±5%，最大误差≤±10%
- 自动识别元件类型
- 测量电容和电感的等效串联电阻(ESR)
- 5V-MicroUSB供电
- 显示方式选择：OLED屏幕、串口上位机（基于QT）

## 问题与改进

- 正负电源轨处理：本项目完全使用片内运放，因此理论上只能处理正电压，但是电阻与电感的串联分压结构形成高通滤波器，使得输出电压为纯交流电压；而模拟信号输入端又需要保证输入电阻极大（减少对前级的影响），前后两者矛盾。

  目前测量电感时会消掉一半波形（负电压），虽然可以测，但是精度受影响。

  **解决方案**

  最简单的方案是加入单电源转双电源模块（如LM27762或电荷泵+负电压LDO）+片外运放（CMOS或JFET运放，保证输入阻抗巨大）作为模拟信号链前级
  
- 运放输入电阻影响前级测量结果：MSPM0G3507的片内运放的输入电阻不大，会影响前级的测量结果（在测量大阻抗时）
- 
  **解决方案**

  使用JFET或者CMOS运放，保证输入电阻巨大。

- 大电流驱动：测量阻抗的原理基于**RLC串联分压**，因此测量小阻抗元件时需要大电流驱动，因此提高精度则需要大电流驱动，但是大电流又会导致功率过大。

  目前最小档位的分压电阻100Ω太大，测量阻抗小于10Ω乃至小于2Ω的元件时，信号峰峰值很小，导致波形噪声很大。

  **解决方案**

  大电流驱动方案：使用大电流输出的运放或模拟缓冲器（如BUF634）或三极管扩流电路

  节能方案：使用LCR数字电桥测量

- 片内运放GBW不够：片内运放GBW为6MHz，放大100kHz信号可能略有吃力（最大32倍）

  **解决方案**

  片外PGA，1.集成PGA或VGA；2.数字电位器+运放（数字电位器可能带不了负电压）；3.模拟开关+运放

- 信号输出频率范围有限：受限于片内DAC速率（1MSPS）和片内ADC采样率（4M），暂时只能测一个频率：1kHz。对于100kHz正弦波，4M采样率（8bit）的精度太低（感觉，没有实测过）。

  **信号源解决方案**

  ​	DAC方案：1MSPS的DAC可以输出100kHz正弦波，后级接重建滤波器（建议5阶以上无源椭圆滤波器+运放跟随）

  ​	DDS方案：AD9833或AD9834足矣

  **ADC解决方案**

  ​	片外ADC方案：高速ADC，芯片选择很多，但是昂贵

  ​	片内ADC方案：交错采样

- 模拟信号链中用到电容，导致相位失真：由于只有单电源，因此使用了交流耦合同相放大器的电路，引入电容元件导致放大器产生额外且不易补偿的相移，在增益很大（32V/V）时极其明显。

  **解决方案**

  模拟信号链不引入容性和感性元件（至少不要在程控增益级引入非纯阻性元件），使用双电源供电。

## 其他改进方向

- 低功耗：
  - 加入自锁开关，长时间无操作则自动关机
  - 使用MSPM0的低功耗模式，进入sleep或stop模式
- 自动测量：目前的自动测量是每隔2s测一次，频率太慢，主要原因是每次测量都要控制一次继电器，这样不仅使得耗电量急剧增加，还会拖慢测量速度。建议把测量二极管功能去掉（用万用表更好），纯测RLC，这样即可不引入继电器。
- 其他单片机复现：（实在是不想再用TI的MCU了）以STM32为例，主要的区别：ADC采样速率、DAC建立时间不同，需要灵活调整；片内没有集成程控增益放大器的话需要外接PGA或VGA。


## 联系方式

- 项目主页: https://github.com/csqNULL/LCR_Instrument
- 邮件联系: 3143280769@qq.com
