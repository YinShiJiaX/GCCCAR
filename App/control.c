#include "include.h"

uchar   BangBang_Flag = 0;
uchar 	Run_Flag      = 0;
int32   RC_Get        = 1500;			/* 遥控器的通道值 */
uchar   Game_End      = 1;
uchar   Stop_Flag     = 1;
int16   Max_Speed     = 300;
int32   MOTOR_Duty1   = 41;
int32   MOTOR_Duty2   = 37;
int32   MOTOR_Duty    = 0;
int32   MOTOR_Speed   = 0;
int32   Pulses_Count  = 0;		/* 正交解码脉冲计数，必须为 int32 */
int32   S_D5_Duty     = 8420;
char    Set           = 7;

/******** 电机控制 *********/
void MOTOR_Control(void)
{
	int32 Set_Speed = 30;
	uchar i;          //计数变量
	uchar Send_Count; //串口需要发送的数据个数
	
	if(Point >= 39 && Point <= 41 )
	{
		Set_Speed = 35;
	}
	else if((Point >= 36 && Point < 39) || (Point > 41 && Point <= 44))
	{
		Set_Speed = 32;
	}
	else if((Point >= 30 && Point < 36) || (Point > 44 && Point <= 50))
	{
		Set_Speed = 30;
	}
	else if((Point >= 28 && Point < 30) || (Point > 50 && Point <= 52))
	{
		Set_Speed = 29;
	}
	else 
	{
		Set_Speed = 25;
	}
	


	
	/* 使用串级增量式PID进行调节 */
	MOTOR_Duty2 += PID_Realize(&MOTOR_PID, MOTOR_Speed, Set_Speed);
	MOTOR_Duty1 = MOTOR_Duty2;
	if (MOTOR_Duty1 >= 0)
	{
		MOTOR_Duty1 = range_protect(MOTOR_Duty1, 0, 30);	//限幅保护
		MOTOR_Duty2 = range_protect(MOTOR_Duty2, 0, 30);	//限幅保护
		/******************电机正转***********************************/
		ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, MOTOR_Duty1);//左电机正转
		ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, MOTOR_Duty2);//右电机正转
		ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM,0);
		ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM,0);
	}
	else
	{
		MOTOR_Duty1 = range_protect(MOTOR_Duty,-30, 0);	//限幅保护
		MOTOR_Duty2 = range_protect(MOTOR_Duty,-30,0);	//限幅保护
		/*******************电机反转***********************************/
		ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
		ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
		ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM,-MOTOR_Duty1);//左电机反转	   			
		ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM,-MOTOR_Duty2);//右电机反转
	}
}

/******* 电机速度测量 ********/
void speed_measure()
{
	int16 Pulses;
	
	Pulses = ftm_quad_get(FTM1);//获取FTM 正交解码 的脉冲数(负数表示反方向)	 
	ftm_quad_clean(FTM1);   	//正交解码寄存器清零 

	MOTOR_Speed = Pulses;	//车子当前的实际速度	
	MOTOR_Control();
}

void Start_Control(void)
{
	static uchar Start_OK = 0;
	static uchar Ready_Stop = 0;
	static uint16 No_Start_Line_Count = 0;
	
	if (Run_Flag)
	{									/** 起跑阶段检测 **/
		if (!Starting_Line_Flag && !Start_OK)	//没有检测到起跑线且未成功出发
		{
			No_Start_Line_Count++;			//相当于延时约5s
			if (No_Start_Line_Count >= 200)	//连续200次没有检测到起跑线，避免起跑时的光线异常干扰
			{
				No_Start_Line_Count = 0;
				Start_OK = 1;	//出发成功
			}
		}
										/** 结束阶段检测 **/
		if (Start_OK && Starting_Line_Flag)	//成功起跑后检测到终点线
		{
			Ready_Stop = 1;	//准备停车
		}
		
		if (Ready_Stop)
		{
			if (!Starting_Line_Flag)	/** 检测到停车线后延时一段时间停车 **/
			{
				No_Start_Line_Count++;
				if (No_Start_Line_Count >= 10 && Game_End)	//连续30次没有检测到起跑线，约延时1s
				{
					No_Start_Line_Count = 0;
					Run_Flag = 0;			//停车
					Start_OK = 0;			//清除出发成功标志位
					Ready_Stop = 0;			//清除准备停车标志位
				}
			}
		}
	}
}


/******** 限幅保护 *********/
int32 
range_protect(int32 duty, int32 min, int32 max)
{
	if (duty >= max)
	{
		return max;
	}
	if (duty <= min)
	{
		return min;
	}
	else
	{
		return duty;
	}
}
void motor_control(void)
{
	if(Point > 45 )
	{
		MOTOR_Duty1 -= 10;
		MOTOR_Duty2 -= 10;
	}
	if(Point < 35)
	{
		MOTOR_Duty1 -= 10;
		MOTOR_Duty2 -= 10;
	}
}