#include "delay.h"
#include "ti_msp_dl_config.h"

///**
// * @brief ����GROUP1�ж�����
// *
// * �ú������ڴ�������GROUP1���ж����󡣾�����˵�������ȼ�����ĸ��ж�Դ�������жϣ�
// * Ȼ����Բ�ͬ���ж�Դִ����Ӧ�Ĵ����߼���������ж�Դ��ҪָGPIO�����жϡ�
// */
//
// void GROUP1_IRQHandler(void)
//{
//    // ���GROUP1���ĸ��ж�Դ�������ж�
//    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1))
//    {
//    case GPIO_KEY_INT_IIDX:
//        // ����GPIO�����ж�
//        switch (DL_GPIO_getPendingInterrupt(GPIO_KEY_PORT))
//        {
//        case GPIO_KEY_key0_IIDX:
//            // �������̼�0�İ��º��ͷ��ж�
//            /* ��⵽��0�ͷ� */
//            delay_ms(10); // ��ʱȥ����
//            if (DL_GPIO_readPins(
//                    GPIO_KEY_PORT, GPIO_KEY_key0_PIN) &&
//                key0.KeyState)
//            {
//
//                // �ж��ǵ�������˫��
//                DL_TimerG_stopCounter(TIMER_1_INST); // ֹͣ��ʱ��
//                // ���ö�ʱ��1�ļ���ֵ
//                DL_TimerG_setLoadValue(TIMER_1_INST,
//                DL_TimerG_getLoadValue(TIMER_1_INST));
//
//                key0.KeyNumberContinuous++; // �������µļ���
//
//                key0.flagPeriod = 0;
//                DL_TimerG_startCounter(TIMER_1_INST); // ������ʱ��
//            }
//            /* ��⵽��0���� */
//            else
//            {
//
//                // ���°���״̬�ͼ���
//                // ��n�ΰ���
//                if (!key0.KeyState)
//                {
//                    key0.KeyState = 1;
//                    DL_TimerG_startCounter(TIMER_1_INST); // ������ʱ��
//                }
//                // ��n+1�ΰ���
//                else
//                {
//                    // ֹͣ��ʱ��1�ļ���
//                    DL_TimerG_stopCounter(TIMER_1_INST);
//                    // ���ö�ʱ��1�ļ���ֵ
//                    DL_TimerG_setLoadValue(TIMER_1_INST,
//                    DL_TimerG_getLoadValue(TIMER_1_INST)); key0.flagPeriod =
//                    0; DL_TimerG_startCounter(TIMER_1_INST); // ������ʱ��
//                    // if (key0.flagPeriod > )
//                }
//
//            }
//            return;
//        default:
//            // ���������ж�Դ�����ﲻ������
//            break;
//        }
//    }
//}
///**
// * @brief TIMER_1_INST_IRQHandler�����Ƕ�ʱ��1���жϷ�������
// * ����������ڴ�����ʱ��1�������жϣ�������Ҫ�߼��Ǽ�ⰴ��0��״̬��
// * �����ݰ���0��״ִ̬����Ӧ�Ĳ�����
// */
//
// void TIMER_1_INST_IRQHandler(void)
//{
//    // ���ݶ�ʱ��1���ж�Դ���д���
//    switch (DL_TimerG_getPendingInterrupt(TIMER_1_INST))
//    {
//    case DL_TIMER_IIDX_ZERO:
//        // ������ʱ������������ж�
//        // ����Ƿ��⵽key0��������
//        if (key0.KeyState)
//        {
//            // ����������£����Ӱ���0�����ڼ���
//            key0.flagPeriod++;
//
//            // ����Ƿ����㳤������
//            if (key0.flagPeriod >= LONG_CLICK)
//            {
//                // ֹͣ��ʱ��1�ļ���
//                DL_TimerG_stopCounter(TIMER_1_INST);
//                // ���ö�ʱ��1�ļ���ֵ
//                DL_TimerG_setLoadValue(TIMER_1_INST,
//                DL_TimerG_getLoadValue(TIMER_1_INST));
//                // �������ڼ��������ð����Ѱ��±�־
//                key0.flagPeriod = 0;
//                key0.KeyAlready = 1;
//                key0.Continuous = 1;
//            }
//            else if (key0.flagPeriod >= END_CLICK &&
//            !key0.KeyNumberContinuous)
//            {
//                // ֹͣ��ʱ��1�ļ���
//                DL_TimerG_stopCounter(TIMER_1_INST);
//                // ���ö�ʱ��1�ļ���ֵ
//                DL_TimerG_setLoadValue(TIMER_1_INST,
//                DL_TimerG_getLoadValue(TIMER_1_INST)); key0.flagPeriod = 0;
//                key0.KeyAlready = 1;
//
//            }
//        }
//        else
//        {
//            // ����δ����ʱ�Ŀղ���
//        }
//        break;
//    default:
//        // ��������δ������ж����
//        break;
//    }
//}
