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
    camera_vsync();
}
void DMA0_IRQHandler(void)
{
    camera_dma();
}


