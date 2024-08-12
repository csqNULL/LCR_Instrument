#ifndef __KEY_H
#define __KEY_H

#include "ti_msp_dl_config.h"

// 最小计时单位100ms
#define DOUBLE_CLICK 3
#define LONG_CLICK 20
#define END_CLICK 3
/**
 * @struct Key
 *
 * 定义了一个与键相关的信息结构体。
 * 该结构体包含了一个键的各种状态和属性信息。
 */
struct Key
{
    int flagPeriod;          /**< 标记键的周期状态，用于表示键是否处于周期性动作中。 */
    int KeyState;            /**< 键的状态，用于表示键当前是按下还是释放。 */
    int Continuous;          /**< 标记键是否为连续触发模式。 */
    int KeyNumberContinuous; /**< 在连续触发模式下，记录连续触发的次数。 */
    int KeyAlready;          /**< 标记键是否已经被触发过。 */
};
#endif
