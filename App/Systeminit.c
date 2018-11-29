#include "include.h"



void EverythingInit(void)
{
    uint8 FiveButton = 0x00;                           /* 用于提取五向按键的状态 */
    /****************设置中断优先级分组以及配置中断优先级***************************/
    NVIC_SetPriorityGrouping(5);
  
	NVIC_SetPriority(PORTA_IRQn,0);
	NVIC_SetPriority(PORTE_IRQn,1);
	NVIC_SetPriority(DMA0_IRQn,2);
	NVIC_SetPriority(PIT0_IRQn,3);
	NVIC_SetPriority(PIT1_IRQn,4);

    /****************配置中断服务函数与中断向量表对应以及使能中断********************/
    set_vector_handler(PORTA_VECTORn, PORTA_IRQHandler);
    set_vector_handler(DMA0_VECTORn, DMA0_IRQHandler);
    set_vector_handler(PORTE_VECTORn, PORTE_IRQHandler);
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);
    enable_irq(PORTE_IRQn);
    enable_irq (PIT0_IRQn);
    port_init(PTE27, ALT1 | IRQ_FALLING | PULLUP );/* 控制舵机的中断端口打开 */ 

    /*************************电机端口初始化***************************************/
    gpio_init(PTC1,GPO,0);
    gpio_init(PTC2,GPO,0);
    gpio_init(PTC3,GPO,0);
    gpio_init(PTC4,GPO,0);

    /*************************舵机端口初始化***************************************/
    gpio_init(PTD4,GPO,0);

    /*************************舵机PWM初始化***************************************/
    ftm_pwm_init(S_D5_FTM,S_D5_CH,S_D5_HZ,S_D5_MID);

    /*************************LED初始化，全部点亮*********************************/
    gpio_init(PTC16,GPO,1);
    gpio_init(PTC17,GPO,1); 
    gpio_init(PTC18,GPO,1); 
    gpio_init(PTC19,GPO,1);

    /****************************摄像头初始化**************************************/
    camera_init(imgbuff); 

    /*******************初始化正交解码模块***************/
    //ftm_quad_init(FTM2);

    /*******************定时器初始化********************/
    //pit_init_ms(PIT0, 5); 

    /******************************档位设置****************************************/
    gpio_init(PTD12,GPI,1);
    gpio_init(PTD13,GPI,1); 
    gpio_init(PTD14,GPI,1); 
    gpio_init(PTD15,GPI,1);
    /*            
    FiveButton = FiveButton | (gpio_get(PTD12) << 3);
    FiveButton = FiveButton | (gpio_get(PTD13) << 2);
    FiveButton = FiveButton | (gpio_get(PTD14) << 1);
    FiveButton = FiveButton | (gpio_get(PTD15) << 0);
    */
    if(gpio_get(PTD12) == 1)
    {
        //MOTOR_Duty1 = 40;
        //MOTOR_Duty2 = 40;
        //ST_MOTOR[3] = 20;
    }
    else if(gpio_get(PTD13) == 1)
    {
        //MOTOR_Duty1 = 35;
        //MOTOR_Duty2 = 35;
        //ST_MOTOR[3] = 15;        
    }
    else
    {
        //MOTOR_Duty1 = 20;
        //MOTOR_Duty2 = 20;
        //ST_MOTOR[3] = 0;        

    }

    /*
    switch(FiveButton)
    {
        case 7:
        MOTOR_Duty1 = 30;
        MOTOR_Duty2 = 20;
        MOTOR[3]    = 10;
        break;
        case 11:
        MOTOR_Duty1 = 35;
        MOTOR_Duty2 = 25;
        MOTOR[3]    = 15;
        break;
        case 13:
        MOTOR_Duty1 = 37;
        MOTOR_Duty2 = 27;
        MOTOR[3]    = 17;
        break;
        case 14:
        MOTOR_Duty1 = 40;
        MOTOR_Duty2 = 30;
        MOTOR[3]    = 25;
        break;
        default:
        MOTOR_Duty1 = 36;
        MOTOR_Duty2 = 26;
        MOTOR[3]    = 10;
    }
    */
    /*************************电机PWM初始化***************************************/
    ftm_pwm_init(MOTOR_FTM, MOTOR1_PWM, MOTOR_HZ, MOTOR_Duty1);/* 左轮正转 */
    ftm_pwm_init(MOTOR_FTM, MOTOR2_PWM, MOTOR_HZ, 0);
    ftm_pwm_init(MOTOR_FTM, MOTOR3_PWM, MOTOR_HZ, 0);
    ftm_pwm_init(MOTOR_FTM, MOTOR4_PWM, MOTOR_HZ, MOTOR_Duty2);
    
}