/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_it.c
 * @brief      山外K60 平台中断服务函数
 * @author     山外科技
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
    uint8  n = 0;    //引脚号
    uint32 flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //清中断标志位

    n = 16;                                             //场中断
    if(flag & (1 << n))                                 //PTA29触发中断
    {
        camera_vsync();
    }
    #if 0             //鹰眼直接全速采集，不需要行中断
    n = 17;
    if(flag & (1 << n))                                 //PTA28触发中断
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
    
    uint8  n = 0;    //引脚号
    n = 27;
    if(PORTE_ISFR & (1 << n))           //PTD7 触发中断
    {
        PORTE_ISFR  = (1 << n);        //写1清中断标志位

        /*  以下为用户任务  */

        gpio_init(PTD4, GPO, 1);

        /*  以上为用户任务  */
    }
   
}


void PIT0_IRQHandler(void)
{
    int16 val;
    val = ftm_quad_get(FTM2);          //获取FTM 正交解码 的脉冲数(负数表示反方向)
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

    PIT_Flag_Clear(PIT0);       //清中断标志位
}


