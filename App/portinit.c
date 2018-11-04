#include "include.h"




void
ledinit(void)
{
    /* 初始化LED0，亮 */
    gpio_init(PTC16,GPO,0);
    gpio_init(PTC17,GPO,0); 
    gpio_init(PTC18,GPO,0); 
    gpio_init(PTC19,GPO,0); 
}
void
motorinit(void)
{
    /* 控制舵机的中断端口打开 */
    port_init(PTE27, ALT1 | IRQ_FALLING | PULLUP ); 
    //gpio_init(PTE27,GPI,0);
    /* 电机端口初始化 */
    gpio_init(PTC1,GPO,0);
    gpio_init(PTC2,GPO,0);
    gpio_init(PTC3,GPO,0);
    gpio_init(PTC4,GPO,0);
    /* 舵机端口初始化 */
    //gpio_init(PTB0,GPO,0);
    gpio_init(PTD4,GPO,0);


    /* 电机PWM初始化 */
    ftm_pwm_init(MOTOR_FTM, MOTOR1_PWM, MOTOR_HZ, MOTOR_Duty1);/* 左轮正转 */
    ftm_pwm_init(MOTOR_FTM, MOTOR2_PWM, MOTOR_HZ, 0);
    ftm_pwm_init(MOTOR_FTM, MOTOR3_PWM, MOTOR_HZ, 0);/* 右轮正转 */
    ftm_pwm_init(MOTOR_FTM, MOTOR4_PWM, MOTOR_HZ, MOTOR_Duty2);
    /* 舵机PWM初始化 */
    ftm_pwm_init(S_D5_FTM,S_D5_CH,S_D5_HZ,S_D5_MID);
}