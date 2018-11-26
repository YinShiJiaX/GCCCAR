#include "include.h"

uchar   BangBang_Flag = 0;
uchar 	Run_Flag      = 0;
int32   RC_Get        = 1500;			/* ң������ͨ��ֵ */
uchar   Game_End      = 0;
uchar   Stop_Flag     = 0;
uchar   run_Flag      = 0;
uchar   stop_Flag     = 0;
int16   Max_Speed     = 300;
int32   MOTOR_Duty1   = 25;
int32   MOTOR_Duty2   = 25;
int32   MOTOR_Duty    = 0;
int32   MOTOR_Speed   = 0;
int32   Pulses_Count  = 0;		/* ���������������������Ϊ int32 */
int32   S_D5_Duty     = 8420;
char    Set           = 7;

/******** ������� *********/
void MOTOR_Control(void)
{
	int32 Set_Speed = 30;
	/* �ж���ת��ת���������ж� */
	Turn_Left = 0;
	Turn_Right = 0;
	if(Point >= 39 && Point <= 41 )
	{
		Set_Speed = 60;
	}
	else if((Point >= 36 && Point < 39) || (Point > 41 && Point <= 44))
	{
		if(Point > 40 )
		{
			Turn_Right = 1;
		}
		else
		{
			Turn_Left = 1;
		}
		Set_Speed = 30;
	}
	else if((Point >= 30 && Point < 36) || (Point > 44 && Point <= 50))
	{
		if(Point > 40 )
		{
			Turn_Right = 1;
		}
		else
		{
			Turn_Left = 1;
		}
		Set_Speed = 28;
	}
	else if((Point >= 28 && Point < 30) || (Point > 50 && Point <= 52))
	{
		if(Point > 40 )
		{
			Turn_Right = 1;
		}
		else
		{
			Turn_Left = 1;
		}
		Set_Speed = 26;
	}
	else 
	{
		if(Point > 40 )
		{
			Turn_Right = 1;
		}
		else
		{
			Turn_Left = 1;
		}
		Set_Speed = 24;
	}
	
	/* ʹ�ô�������ʽPID���е��� */
	MOTOR_Duty2 += PID_Realize(&MOTOR_PID, MOTOR_Speed, Set_Speed);
	MOTOR_Duty1 = MOTOR_Duty2;
	if (MOTOR_Duty1 >= 0)
	{
		MOTOR_Duty1 = range_protect(MOTOR_Duty1, 0, 35);	//�޷�����
		MOTOR_Duty2 = range_protect(MOTOR_Duty2, 0, 35);	//�޷�����
		/******************�����ת***********************************/
		if(Turn_Left == 1)
		{
			//MOTOR_Duty2 = (MOTOR_Duty2 + 10);
			//MOTOR_Duty1 = MOTOR_Duty1;
		}
		if(Turn_Right == 1)
		{
			//MOTOR_Duty1 = (MOTOR_Duty1 + 10);
			//MOTOR_Duty2 = MOTOR_Duty2;
		}

		ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, MOTOR_Duty1);//������ת
		ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, MOTOR_Duty2);//�ҵ����ת
		ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM,0);
		ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM,0);
		
	}
	else
	{
		
		MOTOR_Duty1 = range_protect(MOTOR_Duty,-35, 0);	//�޷�����
		MOTOR_Duty2 = range_protect(MOTOR_Duty,-35, 0);	//�޷�����
		/*******************�����ת***********************************/

		ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
		ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
		ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, MOTOR_Duty1);//������ת	   			
		ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, MOTOR_Duty2);//�ҵ����ת	
	}
	
}

/******* ����ٶȲ��� ********/
void speed_measure()
{
	int16 Pulses;
	
	Pulses = ftm_quad_get(FTM1);//��ȡFTM �������� ��������(������ʾ������)	 
	ftm_quad_clean(FTM1);   	//��������Ĵ������� 

	MOTOR_Speed = Pulses;	//���ӵ�ǰ��ʵ���ٶ�	
	MOTOR_Control();
}

void Start_Control(void)
{
	static uchar Start_OK = 0;
	static uchar Ready_Stop = 0;
	static uint16 No_Start_Line_Count = 0;
	
	if (Run_Flag)
	{									/** ���ܽ׶μ�� **/
		if (!Starting_Line_Flag && !Start_OK)	//û�м�⵽��������δ�ɹ�����
		{
			No_Start_Line_Count++;			//�൱����ʱԼ5s
			if (No_Start_Line_Count >= 200)	//����200��û�м�⵽�����ߣ���������ʱ�Ĺ����쳣����
			{
				No_Start_Line_Count = 0;
				Start_OK = 1;	//�����ɹ�
			}
		}
										/** �����׶μ�� **/
		if (Start_OK && Starting_Line_Flag)	//�ɹ����ܺ��⵽�յ���
		{
			Ready_Stop = 1;	//׼��ͣ��
		}
		
		if (Ready_Stop)
		{
			if (!Starting_Line_Flag)	/** ��⵽ͣ���ߺ���ʱһ��ʱ��ͣ�� **/
			{
				No_Start_Line_Count++;
				if (No_Start_Line_Count >= 10 && Game_End)	//����30��û�м�⵽�����ߣ�Լ��ʱ1s
				{
					No_Start_Line_Count = 0;
					Run_Flag = 0;			//ͣ��
					Start_OK = 0;			//��������ɹ���־λ
					Ready_Stop = 0;			//���׼��ͣ����־λ
				}
			}
		}
	}
}


/******** �޷����� *********/
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