#include "include.h"

void 
main()
{
    /* 初始化所有模块*/
    EverythingInit();
    
    while(1)
    {
      /* 获取图像数组 */
      camera_get_img();
      /* 解压图像 */
      img_extract(img, imgbuff, CAMERA_SIZE);
      /* 图像分析，得到中点数量 */
      Mid_Count = Image_GetLine(img);
      /* 使用加权平均获取目标点横坐标 */	
      Point = Point_Weight();
      /* 使用位置式PID解算,获取所需舵机占空比*/
      S_D5_Duty = PlacePID_Control(&S_D5_PID, 40, Point);
      /* 限幅保护，防止舵机转角过大被打坏，坏板子(8350~8750)，好板子(8270~8570) */
      S_D5_Duty = range_protect(S_D5_Duty, 8260, 8580);
      /*
      char Dutystr2[10];
      sprintf(Dutystr2,"%d  \n",S_D5_Duty);
      uart_putstr(UART5,Dutystr2);
      */
      /* 检测停车 */
      if(Starting_Line_Flag == 1)
      {
        S_D5_Duty = 8420;
        if(Run_Flag == 1)
        {
          Stop_Flag = 1;
        }
        Starting_Line_Flag = 0;
      }
      else
      {
        Run_Flag = 1;
      }
      
      if(Run_Flag == 1 && Stop_Flag == 1)
      {
        ftm_pwm_init(MOTOR_FTM, MOTOR1_PWM, MOTOR_HZ, 0);
        ftm_pwm_init(MOTOR_FTM, MOTOR4_PWM, MOTOR_HZ, 0);
        ftm_pwm_init(MOTOR_FTM, MOTOR2_PWM, MOTOR_HZ, 4);
        ftm_pwm_init(MOTOR_FTM, MOTOR3_PWM, MOTOR_HZ, 4); 
        S_D5_Duty = 8420;
        disable_irq(PIT0_IRQn);
        Starting_Line_Flag = 0;
      }
      

      /* 修改舵机占空比 */
      ftm_pwm_duty(S_D5_FTM, S_D5_CH, S_D5_Duty);
      //vcan_sendimg(imgbuff,CAMERA_SIZE);
    }
}

