#ifndef __INCLUDE_H__
#define __INCLUDE_H__



/*
 *
 * 通用库头文件
 * 
 */
#include  "stdio.h"
#include  "math.h"
#include  "common.h"
#include  "MK60_gpio.h"     
#include  "MK60_uart.h"     
#include  "MK60_SysTick.h"
#include  "MK60_lptmr.h"    
#include  "MK60_pit.h"      
#include  "MK60_FLASH.h" 
#include  "MK60_ftm.h"
#include  "MK60_it.h"

/*
 *山外通用外设库
 */
#include  "VCAN_SCCB.h"
#include  "VCAN_camera.h"
#include  "VCAN_OV7725_Eagle.h"
#include  "VCAN_OV7725_REG.h"
#include  "VCAN_LED.H"          
#include  "VCAN_KEY.H"
#include  "oled.h"

/* 
 *多功能调试助手 
 */
#include  "VCAN_computer.h" 

/*
 * Include 用户自定义的头文件
 */
#include  "PID.h"
#include  "Systeminit.h"
#include  "Control.h"
#include  "Handle.h"
#include  "stdio.h"

    


#endif  /* __INCLUDE_H__ */
