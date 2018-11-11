/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_it.c
 * @brief      ɽ��K60 ƽ̨�жϷ�����
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-06-26
 */

#include "include.h"

void uart5_handler(void)
{
    char ch;
    if(uart_query(UART5) == 1)
    {
        uart_getchar(UART5,&ch);
        uart_putchar(UART5,ch);
    }
}
void PORTA_IRQHandler(void)
{
    uint8  n = 0;    //���ź�
    uint32 flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //���жϱ�־λ

    n = 16;                                             //���ж�
    if(flag & (1 << n))                                 //PTA29�����ж�
    {
        camera_vsync();
    }
    #if 0             //ӥ��ֱ��ȫ�ٲɼ�������Ҫ���ж�
    n = 17;
    if(flag & (1 << n))                                 //PTA28�����ж�
    {
        camera_href();
    }
    #endif
}
void DMA0_IRQHandler(void)
{
    camera_dma();
}
void PORTE_IRQHandler(void)
{
    
    uint8  n = 0;    //���ź�
    n = 27;
    if(PORTE_ISFR & (1 << n))           //PTD7 �����ж�
    {
        PORTE_ISFR  = (1 << n);        //д1���жϱ�־λ

        /*  ����Ϊ�û�����  */

        gpio_init(PTD4, GPO, 1);

        /*  ����Ϊ�û�����  */
    }
   
}


void PIT0_IRQHandler(void)
{
    int16 val;
    val = ftm_quad_get(FTM2);          //��ȡFTM �������� ��������(������ʾ������)
    ftm_quad_clean(FTM2);
    char *Pointstr;
    if(val >= 0)
    {
      sprintf(Pointstr,"%d  \n", val);
      uart_putstr(UART5,Pointstr);
    }
    else
    {
      sprintf(Pointstr,"%d  \n", -val);
      uart_putstr(UART5,Pointstr);
    }

    PIT_Flag_Clear(PIT0);       //���жϱ�־λ
}


