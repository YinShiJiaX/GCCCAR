#include "include.h"

uchar   BangBang_Flag = 0;
uchar 	Run_Flag      = 0;
int32   RC_Get        = 1500;			/* 遥控器的通道值 */
uchar   Game_End      = 1;
uchar   Stop_Flag     = 1;
int16   Max_Speed     = 300;
int32   MOTOR_Duty1   = 28;
int32   MOTOR_Duty2   = 28;
int32   MOTOR_Duty    = 0;
int32   MOTOR_Speed   = 0;
int32   Pulses_Count  = 0;		/* 正交解码脉冲计数，必须为 int32 */
int32   S_D5_Duty     = 8420;
char    Set           = 7;

/******** 电机控制 *********/
void MOTOR_Control(void)
{
	int32 Set_Speed;
	uchar i;          //计数变量
	uchar Send_Count; //串口需要发送的数据个数

	if (Run_Flag)
	{
		if (Set == 1)
		{
			Set_Speed = 140;
		}
		else if (Set == 2)
		{
			Set_Speed = 150;
		}
		else if (Set == 3)
		{
			Set_Speed = 160;
		}
		else if (Set == 4)
		{
			Set_Speed = 170;
		}
		else if (Set == 5)
		{
			if (Foresight >= 18)
			{
				Set_Speed = 170;
			}
			else if (Foresight >= 16)
			{
				Set_Speed = 180;
			}
			else if (Foresight >= 14)
			{
				Set_Speed = 190;
			}
			else if (Foresight >= 12)
			{
				Set_Speed = 200;
			}
			else if (Foresight >= 10)
			{
				Set_Speed = 205;
			}
			else if (Foresight >= 8)
			{
				Set_Speed = 210;
			}
			else if (Foresight >= 7)
			{
				Set_Speed = 220;
			}
			else if (Foresight >= 6)
			{
				Set_Speed = 230;
			}
			else if (Foresight >= 5)
			{
				Set_Speed = 240;
			}
			else
			{
				Set_Speed = 255;
			}
		}
		else if (Set == 6)
		{
			if (Foresight >= 18)
			{
				Set_Speed = 175;
			}
			else if (Foresight >= 16)
			{
				Set_Speed = 185;
			}
			else if (Foresight >= 14)
			{
				Set_Speed = 195;
			}
			else if (Foresight >= 12)
			{
				Set_Speed = 205;
			}
			else if (Foresight >= 10)
			{
				Set_Speed = 215;
			}
			else if (Foresight >= 8)
			{
				Set_Speed = 220;
			}
			else if (Foresight >= 7)
			{
				Set_Speed = 230;
			}
			else if (Foresight >= 6)
			{
				Set_Speed = 240;
			}
			else if (Foresight >= 5)
			{
				Set_Speed = 250;
			}
			else
			{
				Set_Speed = 265;
			}
		}
		else if (Set == 7)
		{
			if (Foresight >= 18)
			{
				Set_Speed = 180;
			}
			else if (Foresight >= 16)
			{
				Set_Speed = 210;
			}
			else if (Foresight >= 14)
			{
				Set_Speed = 220;
			}
			else if (Foresight >= 12)
			{
				Set_Speed = 225;
			}
			else if (Foresight >= 10)
			{
				Set_Speed = 235;
			}
			else if (Foresight >= 8)
			{
				Set_Speed = 245;
			}
			else if (Foresight >= 6)
			{
				Set_Speed = 250;
			}
			else if (Foresight >= 5)
			{
				Set_Speed = 255;
			}
			else if (Foresight >= 4)
			{
				Set_Speed = 265;
			}
			else
			{
				Set_Speed = 280;
			}
		}
		else if (Set == 8)
		{
			if (Foresight >= 18)
			{
				Set_Speed = 195;
			}
			else if (Foresight >= 16)
			{
				Set_Speed = 205;
			}
			else if (Foresight >= 14)
			{
				Set_Speed = 220;
			}
			else if (Foresight >= 12)
			{
				Set_Speed = 230;
			}
			else if (Foresight >= 10)
			{
				Set_Speed = 240;
			}
			else if (Foresight >= 8)
			{
				Set_Speed = 250;
			}
			else if (Foresight >= 6)
			{
				Set_Speed = 260;
			}
			else if (Foresight >= 5)
			{
				Set_Speed = 270;
			}
			else if (Foresight >= 4)
			{
				Set_Speed = 280;
			}
			else
			{
				Set_Speed = 290;
			}
		}
		else if (Set == 0)
		{
			if (RC_Get > 1300)
			{
				Set_Speed = 70 + 80 * (RC_Get - 1000) / 1000;
			}
			else
			{
				Set_Speed = 0;
			}
			Set_Speed = range_protect(Set_Speed, 0, 180);
		}
	}
	else
	{
		Set_Speed = 0;
	}
	
		
	MOTOR_Duty += PID_Cascade(&MOTOR_PID, MOTOR_Speed, Set_Speed);//使用串级增量式PID进行调节
//	MOTOR_Duty = 200;
	
	if ((!Run_Flag || !Set_Speed) && MOTOR_Speed > -10 && MOTOR_Speed < 10)
	{
		MOTOR_Duty = 0;
	}
	if (MOTOR_Duty >= 0)
	{
		MOTOR_Duty = range_protect(MOTOR_Duty, 0, 900);	//限幅保护
					//电机正转
		ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM,MOTOR_Duty);	//占空比最大990！！！
		ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM,0);	   		//占空比最大990！！！
		ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM,MOTOR_Duty);
		ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM,0);
//		DataScope_Get_Channel_Data(MOTOR_Duty, 2);		//将速度写入通道 2
	}
	else
	{
		MOTOR_Duty = range_protect(MOTOR_Duty, -900, 0);	//限幅保护
					//电机反转
		ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM,0);	   			//占空比最大990！！！
		ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM,-MOTOR_Duty);	//占空比最大990！！！
//		DataScope_Get_Channel_Data(MOTOR_Duty, 2);  		//将速度写入通道 2
	}
//	DataScope_Get_Channel_Data( MOTOR_Speed, 1);//将速度写入通道 1
}

/******* 电机速度测量 ********/
void speed_measure()
{
	int16 Pulses;
	
	Pulses = ftm_quad_get(FTM1);//获取FTM 正交解码 的脉冲数(负数表示反方向)	 
	ftm_quad_clean(FTM1);   	//正交解码寄存器清零 

	MOTOR_Speed = Pulses;	//车子当前的实际速度	
	//save_var(VAR6, MOTOR_Speed);
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