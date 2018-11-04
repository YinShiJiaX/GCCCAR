#include "include.h"

uint8 colour[2] = {255, 0},black=1,white=0; //0 �� 1 �ֱ��Ӧ����ɫ
//ע��ɽ�������ͷ 0 ��ʾ ��ɫ��1��ʾ ��ɫ

uchar Weight[60] = /*{1,  1,  1,  1,  1,    1,  1,  1,  1,  2,
					3,  4,  5,  7,  9,   10, 12, 14, 16, 19,
				   21, 20, 19, 19, 18,   15, 13, 11,  8,  6,
				    4,  3,  3,  2,  1,    1,  1,  1,  1,  1,
					1,  1,  1,  1,  1,    1,  1,  1,  1,  1,
					1,  1,  1,  1,  1,    1,  1,  1,  1,  1};	//��Ȩƽ������*/
				   {1,  1,  1,  2,  4,    7,  11,  15,  16,  18,
					20,  24,  25,  27,  29,   30, 30, 30, 16, 19,
				   15, 15, 13, 13, 12,   12, 11, 11,  8,  6,
				   1,  1,  1,  1,  1,    1,  1,  1,  1,  1,
				   4,  3,  3,  2,  1,    1,  1,  1,  1,  1,
				   1, 1,  1,  1,  1,    1,  1,  1,  1,  2,};	//��Ȩƽ������

uchar Left_Line[63], Mid_Line[63], Right_Line[63];	//�����ұ߽���
uchar Left_Line_New[63], Right_Line_New[63];		//�߽粹�ߵ�����
uchar Width[61], Width_Min;							//ÿ�������Ŀ��
uchar Left_Add[61], Right_Add[61]; 					//���ұ߽粹�߱�־λ
uchar Left_Add_Flag, Right_Add_Flag;				//���ұ߽粹���ܱ�־λ
uchar Left_Add_Start, Right_Add_Start;				//��¼���ߵĿ�ʼλ��
uchar Left_Add_End, Right_Add_End;					//��¼���ߵĽ���λ��
uchar Left_Max, Right_Min;							//���ұ߽缫ֵ����������S��
uchar Left_y, Right_y;								//���ұ߽缫ֵ��������
uchar Point_x, Point_y;								//Ŀ���ĺ�������
uchar Mid_Count = 0;		   						//�ܹ�ʶ�����Զ���е�����
uchar Foresight_Left, Foresight_Right;				//ǰհ���ұ߽缫ֵ
uchar Turn_Left = 0, Turn_Right = 0;
uchar Repair_Flag;
uchar Hazard_Left_Flag,Hazard_Right_Flag;			//�ϰ����־λ
uchar Left_Jam, Right_Jam;							//���Ҹ����ź�
uchar Starting_Line_Flag;							//�����߱�־λ
int32 Last_Angle = 0;
float Last_Slope = 0;
float Left_Last_Slope = 0;
float Right_Last_Slope = 0;
uint8 Point = 40;
uint8 imgbuff[CAMERA_SIZE];                           
uint8 img[CAMERA_W*CAMERA_H]; 

uchar Point_Mid = 0;
uchar Foresight = 0;	/* ǰհ�����ٶ� */
uchar Out_Side = 0;		/* ������� */

uchar Image_GetLine(uchar *data)	/* ��ȡ�����ұ߽��� */
{
	PointWeightAdjust(Weight, 6, 0);
	char i = 59;	/* i����ͼ������� */
	uchar Line_Count, Left_Temp, Right_Temp;
	char temp;
	float Add_Slope;
	
	Hazard_Left_Flag = 0;	/* �����ϰ����־λ���� */
	Hazard_Right_Flag = 0;
	Left_Jam = 0;			/* ���Ҹ����ź����� */
	Right_Jam = 0;
	//Starting_Line_Flag = 0;	/* �����߱�־λ���� */
	
	Mid_Count = 0;
	Left_Max = 2;
	Right_Min = 78;
	Left_y = 59;
	Right_y = 59;
	Width_Min = 80;
	Repair_Flag = 0;
	Left_Add_Start = 0;
	Right_Add_Start = 0;
	Left_Add_End = 0;
	Right_Add_End = 0;
	Foresight_Left = 2;
	Foresight_Right = 78;
	Width[60]     = 80;
	Left_Add[60]  = 0;
	Right_Add[60] = 0;
	Left_Line[60] = 2;
	Left_Line[61] = 2;
	Left_Line[62] = 1;
	Mid_Line[60]  = 40;
	Mid_Line[61]  = 40;
	Mid_Line[62]  = 40;
	Right_Line[60]= 78;
	Right_Line[61]= 78;
	Right_Line[62]= 79;	/* ��Ϊȷ�����·�����ͼ��ı߽� */
	uchar r = 0;/* ������������� */
	
	/* ֻ�ɼ�ǰ40�е�ͼ����Ϊ̫Զ��ͼ���ȶ����ο���ֵ���� */
	for (i = 59; i >= 20; i--)	
	{
		for(uchar c = 0; c <= 80; c++)
		{
			if(data[80*i + c] && data[80*i + c + 1] && !data[80*i + c + 2] && !data[80*i + c + 3])
			{
				r++;
			}
		}
		if(r >= 6)
		{
			Starting_Line_Flag = 1;/* ��⵽������ͣ�� */
		}
		r = 0;
		/* ɨ��ÿ��֮ǰ����Ϊ���ұ߽綼��Ҫ���� */
		Left_Add[i] = 1;
		Right_Add[i] = 1;
		/* ǰһ���е��ڱ���Ϊ�ڵ� */
		if (!data[80 * i + Mid_Line[i + 1]])
		{
			/* ��һ�� */
			if (i == 59)
			{
				/* ʹ�õ�һ��ר�ô����� */
				First_Line_Add(data);
				/* ��һ�������쳣 */
				if (Right_Line[59] - Left_Line[59] < 40)
				{
					/* ����ʹ�ñ߽�ɨ�� */
					Traversal_Left_Side(59, data);
					Traversal_Right_Side(59, data);
				}				
				continue;
			}
			else
			{
				break;
			}
		}
		else
		{			
			/* ��ǰһ���е�������Ѱ�ұ߽� */
			Traversal_Mid_Line(i, data);
			if (i >= 55)
			{
				/* �����쳣 */
				if (Right_Line[i] - Left_Line[i] < 40)
				{
					/* ����ʹ�ñ߽�ɨ�� */
					Traversal_Left_Side(i, data);
					Traversal_Right_Side(i, data);
				}
			}
		}		
		
		/* ��һ���ֶ�ͼ����д�������Ҫ���ߵ�ʱ������б�����ϲ���
		   ��������ֻ��Ϊ�������ұ߽�ʱ��׼ȷ��������б��ʮ��ʱ����
		   ���ߺ�ı߽粻��ֱ��ʹ�ã���Ҫ�޸���ſ���ʹ�� */
		
		/* ���㱾���������	*/
		Width[i] = Right_Line[i] - Left_Line[i];
		/* ����������ȴ��ڵ�����һ��������ȣ���Ϊ����ͻ�� */    
		if ((Width[i] >= Width[i+1]) || (Width[i] >= Width_Min))
		{
			/* ��Ҫ���� */
			Repair_Flag = 1;
			/* ǰһ�в����� */
			if (Left_Add[i+1])
			{
				/* ��ǰһ�е���߽�ʵ�߱Ƚ� */
				if (Left_Line[i] < Left_Line_New[i+1]-1)	
				{
					Left_Add[i] = 1;
				}
			}
			/* ǰһ��û�в��� */
			else
			{
				/* ��ǰһ�е���߽�ʵ�߱Ƚ�	*/
				if (Left_Line[i] < Left_Line[i+1]-1)
				{
					Left_Add[i] = 1;
				}
			}
			/* ǰһ���ұ߽粹���� */
			if (Right_Add[i+1])
			{
				/* ��ǰһ�е��ұ߽�ʵ�߽��бȽ� */
				if (Right_Line[i] > Right_Line_New[i+1]+1)
				{
					Right_Add[i] = 1;
				}
			}
			/* ǰһ���ұ߽�û�в��� */
			else
			{
				/* ��ǰһ�е��ұ߽�ʵ�߽��бȽ� */
				if (Right_Line[i] > Right_Line[i+1]+1)
				{
					Right_Add[i] = 1;
				}
			}
		}
		/**************************   �����ϰ����   ******************/
		else
		{
			/* ��һ�в���,̫Զ��Ҳ���� */
			if (i != 59 && i > 30)
			{
				/* ������ǰһ���ұ߽��нϴ�ͻ�� */
				if ((Right_Line[i] + 5 < Right_Line[i+1]) && !Right_Add[i+1])
				{
					/* �������ж��Ǻ�ɫ */
					if (!data[80*(i-1)+Right_Line[i]] && !data[80*(i-2)+Right_Line[i]])
					{
						/* �Ҳ��ϰ����־λ��û����λ */
						if (!Hazard_Right_Flag)
						{
							/* ��Ϊ�Ұ��������ͻ�䣬��¼�ϰ�����λ�� */
							Hazard_Right_Flag = i;
						}
					}
					else
					{
						/* ����Ҳ�����źŻ�δ��λ */
						if (!Right_Jam)
						{
							/* ��Ϊ�Ҳ���ָ��ţ���¼�����ź�λ�� */
							Right_Jam = i;
						}
						/* ����ʹ�ñ߽�Ѱ�߷�Ѱ�Ҹ����б߽� */
						Traversal_Right_Side(i, data);
					}
				}
				/* ������ǰһ����߽��нϴ�ͻ����ǰһ��û�в��� */
				if (Left_Line[i] > Left_Line[i+1] + 5 && !Left_Add[i+1])
				{
					/* �������ж��Ǻ�ɫ */
					if (!data[80*(i-1)+Left_Line[i]] && !data[80*(i-2)+Left_Line[i]])
					{
						/* ����ϰ����־λ��û����λ */
						if (!Hazard_Left_Flag)
						{
							/* ��Ϊ�����������ϰ����¼�ϰ�����λ�� */
							Hazard_Left_Flag = i;
						}
					}
					else
					{
						/* ����������źŻ�δ��λ */
						if (!Left_Jam)
						{
							/* ��Ϊ�����ָ��ţ���¼�����ź�λ�� */
							Left_Jam = i;
						}
						/* ����ʹ�ñ߽�Ѱ�߷�Ѱ�Ҹ����б߽� */
						Traversal_Left_Side(i, data);
					}
				}			
			}
		}
		/*******************************   �ϰ�������   **********************/
		/* �����Ҫ���� */
		if (Left_Add[i])
		{	
			/* �����û�м�¼��ʼ����λ�� */
			if (!Left_Add_Start)
			{
				/* ��¼��߽粹�߿�ʼλ�� */
				Left_Add_Start = i;
			}
			/* �ܹ�ʶ��������ǳ��٣����߱��ο���ֵ */
			if (i >= 50)
			{
				/* ʹ����һ֡ͼ�����߽�б�� */
				Add_Slope = Left_Last_Slope;
				if (Add_Slope > 0)
				{
					Add_Slope = 0;
				}
				/* ͨ��б�����㲹�ߵ�λ�� */
				temp = (char)((i - 59) * Add_Slope + Left_Line[59]);
			}
			else
			{
				if (Left_Add_Start >= 50)
				{
					Add_Slope = Left_Last_Slope;
				}
				/* ʹ��ǰ���е�б�ʽ��в��� */
				else
				{
					/* ������ʶ���ǰ����ͼ��б�� */
					Add_Slope = 1.0*(Left_Line[Left_Add_Start+6] - Left_Line[Left_Add_Start+1]) / 5;
				}
				/* �޷� */
				if (Add_Slope > 0)
				{
					Add_Slope = 0;
				}
				/* ͨ��б�����㲹�ߵ�λ�� */
				temp = (char)((i - (Left_Add_Start+1)) * Add_Slope + Left_Line[Left_Add_Start+1]);
				/* �����ϴ���߽�б�� */
				Left_Last_Slope = Add_Slope;
			}
			/* ��ֱ���޸ı߽磬ֻ�����ڲ��������� */
			Left_Line_New[i] = range_protect(temp, 2,78);

													/* ��һ�β��ߣ�ֻ��¼������ͼ������ʾ */
//			data[80 * i + Left_Line_New[i] + 2] = 0;	//����2λ��ʾ��߽磬������Թ۲�	  
		}
		/* �ұ���Ҫ���� */
		if (Right_Add[i])
		{	
			/* �����û�м�¼��ʼ����λ�� */
			if (!Right_Add_Start)
			{
				/* ��¼��߽粹�߿�ʼλ�� */
				Right_Add_Start = i;
			}
			/* �ܹ�ʶ��������ǳ��٣����߱��ο���ֵ */
			if (i >= 50)
			{				
				/* ʹ����һ֡ͼ����ұ߽�б�� */
				Add_Slope = Right_Last_Slope;
				if (Add_Slope < 0)
				{
					Add_Slope = 0;
				}
				/* ͨ��б�����㲹�ߵ�λ�� */
				temp = (char)((i - 59) * Add_Slope + Right_Line[59]);
			}
			else
			{
				if (Right_Add_Start >= 50)
				{					
					Add_Slope = Right_Last_Slope;	
				}
				else
				{
					/* ������ʶ���ǰ����ͼ��б�� */
					Add_Slope = 1.0*(Right_Line[Right_Add_Start+6] - Right_Line[Right_Add_Start+1]) / 5;
				}
				/* �޷� */
				if (Add_Slope < 0)
				{
					Add_Slope = 0;
				}
				/* ͨ��б�����㲹�ߵ�λ�� */
				temp = (char)((i - (Right_Add_Start+1)) * Add_Slope + Right_Line[Right_Add_Start+1]);
				/* ��ϸ�ϴ��ұ߽�б�� */
				Right_Last_Slope = Add_Slope;
			}
			/* ��ֱ���޸ı߽磬ֻ�����ڲ��������� */
			Right_Line_New[i] = range_protect(temp, 2, 78);
			
													/* ��һ�β��ߣ�ֻ��¼������ͼ������ʾ */			
//			data[80 * i + Right_Line_New[i] - 2]  = 0;	//����2λ��ʾ�ұ߽磬������Թ۲�	  
		}
		/* ���߶���Ҫ���� */
		if (Left_Add[i] && Right_Add[i])
		{
			Width[i] = Right_Line_New[i] - Left_Line_New[i];
		}
		/* ����Ҫ���߻�ֻ��һ����Ҫ���� */
		else
		{
			/* �˴����ֻ��һ�߻���Ҫ���� */
			if (Left_Add[i])
			{
				/* ���¼��㱾��������� */
				Width[i] = Right_Line[i] - Left_Line_New[i];
			}
			else if (Right_Add[i])
			{
				/* ���¼��㱾��������� */
				Width[i] = Right_Line_New[i] - Left_Line[i];
			}
			else
			{
				/* ���ܻ��и��ţ�Ҳ���¼���һ��������� */
				Width[i] = Right_Line[i] - Left_Line[i];
			}
			if (Width[i] < Width_Min)
			{
			/* ������С������� */
				Width_Min = Width[i];
			}
			/* ������߽����ֵ */
			if (Left_Line[i] > Foresight_Left)
			{
				Foresight_Left = Left_Line[i];
			}
			/* �����ұ߽���Сֵ */
			if (Right_Line[i] < Foresight_Right)
			{
				Foresight_Right = Right_Line[i];
			}
			/* ��߲���Ҫ������ǰһ����Ҫ����,������ߵ�һ�β��� */
			if (!Left_Add[i] && Left_Add[i+1] && !Left_Add_End)
			{
				/* ֻ��¼��һ�β��ߵ�Ŀ������꣬���Ժ���� */
				Left_Add_End = 1;
				/* ��¼��߽粹�߽�����λ�� */
				Left_y = i;
				if (Left_Line[i] > Left_Max)
				{
					/* ������߽����ֵ */
					Left_Max = Left_Line[i];
				}
			}
			/* �ұ߲���Ҫ������ǰһ����Ҫ����,�����ұߵ�һ�β��� */
			if (!Right_Add[i] && Right_Add[i+1] && !Right_Add_End)
			{
				/* ֻ��¼��һ�β��ߵ�Ŀ������꣬���Ժ���� */
				Right_Add_End = 1;
				/* ��¼�ұ߽粹�߽�����λ�� */
				Right_y = i;
				if (Right_Line[i] < Right_Min)
				{
					/* �����ұ߽���Сֵ */
					Right_Min = Right_Line[i];
				}
			}			
		}
		
		if (Left_Add[i])
		{
			Left_Temp = Left_Line_New[i];
		}
		else
		{
			Left_Temp = Left_Line[i];
		}
		if (Right_Add[i])
		{
			Right_Temp = Right_Line_New[i];
		}
		else
		{
			Right_Temp = Right_Line[i];
		}
		/* ������������е㣬ֻ�����ο�������ͼ������ʾ */
		Mid_Line[i] = (Left_Temp + Right_Temp) / 2;
	}
	/* ���Ҷ����ڸ��� */
	if (Left_Jam && Right_Jam)
	{
		/* ��Ϊ��⵽������ */
		Starting_Line_Flag = 1;
	}
	/* ����ɹ������ı߽������ */
	Line_Count = i;		
	/* �ڶ���ͼ����ʹ�����ұ߽����ֵ�����ұ߽��޸��ĸ��Ӻ��� */
	if (Right_Add_Start && Left_Add_Start && !Hazard_Left_Flag && !Hazard_Right_Flag)	//���Ҷ���Ҫ������û���ϰ���
	{
		/* ��߽粹������ */
		Left_Line_Repair(Left_y, Left_Max, data);
		/* �ұ߽粹������ */
		Right_Line_Repair(Right_y, Right_Min, data);
	}

	
	/* ������ͼ���������޸���ʹ���޸��������ұ߽����¼����е㲢��ͼ������ʾ���� */		
	Mid_Line_Repair(Line_Count, data);
	/* �ɹ�������ͼ�񲻳���������Ϊ���磬ͣ�� */
	if (Mid_Count <= 2)
	{
		Out_Side++;
		led(LED0,LED_OFF);
		if (Out_Side >= 50)
		{
			Run_Flag = 0;			
		}
	}
	else
	{
		Out_Side = 0;
	}
		
	return Mid_Count;
}

void First_Line_Add(uchar *data)
{
	uchar i = 59;
	
	if (Turn_Left)
	{
		/* ʹ���������ȡ�߽� */
		Traversal_Left_Line(i, &data[80 * i], &Left_Line[i], &Right_Line[i]);
	}
	else if (Turn_Right)
	{
		/* ʹ���ұ�����ȡ�߽� */
		Traversal_Right_Line(i, &data[80 * i], &Left_Line[i], &Right_Line[i]);
	}
	else
	{
		/* ��߼������ұ߼�� */
		if (Traversal_Left(&data[80 * i]) >= Traversal_Right(&data[80 * i]))
		{
			/* ʹ���������ȡ�߽� */
			Traversal_Left_Line(i, &data[80 * i], &Left_Line[i], &Right_Line[i]);
		}
		else
		{
			/* ʹ���ұ�����ȡ�߽� */
			Traversal_Right_Line(i, &data[80 * i], &Left_Line[i], &Right_Line[i]);
		}
	}
	/* ��������Ҫ���� */
	if (Left_Add[i])
	{
		/* ��ǰһ�е���߽���Ϊ������߽���ʱ����λ�� */
		Left_Line[i] = 2;
	}
	/* ����ұ���Ҫ���� */
	if (Right_Add[i])	
	{
		/* ��ǰһ�е��ұ߽���Ϊ�����ұ߽���ʱ����λ�� */
		Right_Line[i] = 78;
	}
}

uchar Point_Handle(void)
{
	uchar i;
	uchar Point = 40;
	int32 Sum = 0;
	
	if (Foresight_Left + 15 < Foresight_Right)		//λ��ֱ�߻�СS��û���ϰ���
	{
		Point = (Foresight_Left + Foresight_Right) / 2;	//ȡ���Ҽ�ֵ�е���ΪĿ���
	}
	else		/****** �򵥼�Ȩ��Ŀ��� ******/
	{
		if (Mid_Count > 10)	
		{
			if (Mid_Count > 30)
			{
				Mid_Count = 30;
			}
			for (i = 0; i <= Mid_Count-10; i++)
			{
				Sum += Mid_Line[49-i];
			}
			Point = Sum / (Mid_Count - 10);
		}
		else
		{
			Point = Mid_Line[60 - Mid_Count];
		}
	}
	
				/****** �ϰ��ﴦ�� ******/
	if (Hazard_Left_Flag)					//������ϰ���
	{
		Point = Mid_Line[Hazard_Left_Flag] + 0;	//ʹ���ϰ�����ֵ���һ���е���ΪĿ���
	}
	else if (Hazard_Right_Flag)			//�Ҳ����ϰ���
	{
		Point = Mid_Line[Hazard_Right_Flag] - 0;//ʹ���ϰ�����ֵ���һ���е���ΪĿ���
	}
	
	
	if (Mid_Count >= 35)
	{
		Point_Mid = Mid_Line[60-35];
	}
	else
	{
		Point_Mid = Mid_Line[60-Mid_Count];
	}
	Foresight = 0.8 * Error_Transform(Point_Mid, 40)	//ʹ����Զ��ƫ��ͼ�Ȩƫ��ȷ��ǰհ
			  + 0.2 * Error_Transform(Point, 	 40);
	
	return Point;
}

uchar Point_Weight(void)
{
	char i ,Point;
	static char Last_Point = 40;
	int32 Sum = 0, Weight_Count = 0;
	
	if (Out_Side)	//�����������ͷͼ���쳣
	{
		Point  = Last_Point;
		Point_Mid = 2;
	}
	else
	{
		if (Foresight_Left + 33 < Foresight_Right)		//λ��ֱ�߻�СS��û���ϰ���
		{
			Point = (Foresight_Left + Foresight_Right) / 2;	//ȡ���Ҽ�ֵ�е���ΪĿ���
		}
		else	//λ����ͨ������S
		{
			for (i = 59; i >= 60-Mid_Count; i--)		//ʹ�ü�Ȩƽ��
			{
				Sum += Mid_Line[i] * Weight[59-i];
				Weight_Count += Weight[59-i];
			}
			Point = range_protect(Sum / Weight_Count, 2, 78);
		}
							/*** �ϰ�������������� ***/
		if (Hazard_Left_Flag && Left_Add_Start)			//������ϰ�������Ҫ���ߣ���ʹ����Ҳ�������Ӱ��
		{
			Point = Mid_Line[Hazard_Left_Flag];	//ʹ���ϰ�����ֵ���һ���е���ΪĿ���
			if (Hazard_Left_Flag < 40)
			{
				Point += 3;
			}
		}
		else if (Hazard_Right_Flag && Right_Add_Start)	//�Ҳ����ϰ�������Ҫ���ߣ���ʹ����Ҳ�������Ӱ��
		{
			Point = Mid_Line[Hazard_Right_Flag];//ʹ���ϰ�����ֵ���һ���е���ΪĿ���
			if (Hazard_Right_Flag < 40)
			{
				Point -= 5;
			}
		}
		Point = range_protect(Point, 2, 78);
		Last_Point = Point;
		
		if (Point < 40)		/***** �����⼸��ò�Ʋ�û��ʲô���ã�ֻ�Ե�һ�����������΢��Ӱ�� ****/
		{
			Turn_Left = 1;
			Turn_Right = 0;
		}
		else if (Point > 40)
		{
			Turn_Right = 1;
			Turn_Left = 0;
		}
		else
		{
			Turn_Left = 0;
			Turn_Right = 0;
		}
		/***** ʹ����Զ�����ݺ�Ŀ�����Ϊǰհ *****/
		if (Mid_Count >= 25)
		{
			Point_Mid = Mid_Line[60-30];
		}
		else
		{
			Point_Mid = Mid_Line[60-Mid_Count];
		}
	}
	Foresight = 0.8 * Error_Transform(Point_Mid, 40)	//ʹ����Զ��ƫ��ͼ�Ȩƫ��ȷ��ǰհ
			  + 0.2 * Error_Transform(Point, 	 40);
	
	return Point;
}

char Error_Transform(uchar Data, uchar Set)
{
	char Error;
	
	Error = Set - Data;
	if (Error < 0)
	{
		Error = -Error;
	}
	
	return Error;
}

int32 Curvature_Transform(uchar Now, uchar Point)	//����ת��������Ŵ�1000��
{
	char Positive = 1; 	//��¼��������
	int32 Curvature;	//����
	int32 L_AB, L_BC;	//������ɵ�ֱ����������ֱ�Ǳ�
	int32 Error_AA;		//��������λ���������е�ľ���
	
	L_AB = 59 - Point;
	if (Mid_Line[Point] >= Now)	//Ӧ����ת
	{
		L_BC = Mid_Line[Point] - Now;
		Positive = -1;	//���ʼ�Ϊ����
	}
	else				//Ӧ����ת
	{
		L_BC = Now - Mid_Line[Point];
		Positive = 1;	//���ʼ�Ϊ����
	}
	if (Now >= 35 && Now <= 45)	//�����������м�
	{
		Curvature = 1000 * 2 * L_BC / (L_AB*L_AB + L_BC*L_BC);	//���ʷŴ�100��
	}
	else if (Now < 35)			//����������ƫ���λ��
	{
		Error_AA = 40 - Now;	//���㳵��ƫ�������е�ľ���
		Curvature = 1000 * 2 * (L_BC - Error_AA) / (L_AB*L_AB + (L_BC-Error_AA) * (L_BC-Error_AA));	//���ʷŴ�1000��
	}
	else if (Now > 45)			//����λ������ƫ�ҵ�λ��
	{
		Error_AA = Now - 40;	//���㳵��ƫ�������е�ľ���
		Curvature = 1000 * 2 * (L_BC + Error_AA) / (L_AB*L_AB + (L_BC+Error_AA) * (L_BC+Error_AA));	//���ʷŴ�1000��
	}
	
	Curvature *= Positive;
	
	return Curvature;
}

/*int32 Angle_Transform(uchar *data)	//Ŀ��Ƕ�ת��
{
	double Angle_Temp = 0;
	double Slope = 0;	//б��		
	int32 Angle = 0;	//�Ƕ�
	
	Point_Scan();
	Slope = Slope_Display(data);			//����Ŀ���б�ʲ���ʾ��ͼ����
	Angle_Temp = atan(Slope) * 180 / PI;	//ʹ�÷����Ǻ������б�ʶ�Ӧ�Ƕ�
	Angle = Wave_Filtering(Angle_Temp, 5);	//������ĽǶȽ������Ϊ5�Ļ����˲�
	Last_Slope = Slope;	//�����ϴ�б��
	Last_Angle = Angle;	//�����ϴ�Ŀ��Ƕ�
	
	if (Angle >= 0)
	{
		save_var(VAR2,Angle);					//���Ƕȷ�����λ��
	}
	else
	{
		save_var(VAR2,-Angle);
	}
		
	return Angle;
}*/


/*
 * Function:  Mid_Filtering
 * 
 * Description:  
 * �����˲�
 * 
 * Requires:
 * data
 */	
void Mid_Filtering(uchar *data)
{
	uchar i, Count;
	/* �ж�һ���ж��ٶ��ٿ����е� */
	Count = 60 - Mid_Count;
	/* ���������˲� */
	for (i = 58; i > Count; i--)
	{
		Mid_Line[i] = Mid_Line[i+1]*0.3 + Mid_Line[i]*0.7;
	}
	/* �������»����˲� */
	for (i = Count; i < 59; i++)
	{
		Mid_Line[i] = Mid_Line[i-1]*0.3 + Mid_Line[i]*0.7;
	}
	/* ��������ͼ������ʾ���� */
	for (i = 59; i >= Count; i--)
	{
		data[80*i + Mid_Line[i]] = 0;
	}
}

uint32 Wave_Filtering(uint32 data, uchar Deep)
{
	static uint32 Filter_Buff[10] = {0};
	static uchar Filter_Count = 0;
	uint32 Sum = 0;
	uint32 Filter;
	uchar i;
	
	Filter_Buff[Filter_Count++] = data;
	if (Filter_Count == Deep)
	{
		Filter_Count = 0;
	}
	for (i = 0; i < Deep; i++)
	{
		Sum += Filter_Buff[i];
	}
	Filter = Sum / Deep;
	
	return Filter;
}


/*
 * Function:  Point_Scan
 * 
 * Description:  
 * ȷ��Ŀ�������
 * 
 * Requires:
 * 
 */	
void Point_Scan(void)
{	
	/* ʹ���յ�������ΪĿ��� */
	if (Mid_Count > 30)
	{
		Point_x = Mid_Line[30];
		Point_y = 30;
	}
	/* ʹ����ʶ�����Զ�е�ΪĿ��� */
	else
	{
		Point_x = Mid_Line[60 - Mid_Count];
		Point_y = 60 - Mid_Count;
	}
	/* Ŀ����޷� */
	Point_x = range_protect(Point_x, 2, 78);
	Point_y = range_protect(Point_y, 20, 59);
}


/*
 * Function:  Slope_Display
 * 
 * Description:  
 * ����Ŀ���б��
 * 
 * Requires:
 * data    :ָ���i�е�ָ��
 */	
double Slope_Display(uchar *data)
{
	char i;
	/* б�� */
	double Slope;
	/* ����Ŀ���б�� */
	Slope = 1.0 * (40 - Point_x) / (60 - Point_y);
	
	for (i = 59; i >= Point_y; i--)
	{
		data[80 * i + (uchar)(40 - (60 - i)*Slope)] = 0;	
	}
	
	return Slope;
}

void Mid_Line_Repair(uchar count, uchar *data)
{
	uchar i, temp;
	uchar Left, Right;
	
	for (i = 59; i > count; i--)
	{
		if (Left_Add[i] && (Right_Add_Start || Hazard_Left_Flag))
		{
			Left = Left_Line_New[i];
		}
		else
		{
			Left = Left_Line[i];
		}
		if (Right_Add[i] && (Left_Add_Start || Hazard_Right_Flag))
		{
			Right = Right_Line_New[i];
		}
		else
		{
			Right = Right_Line[i];
		}
		temp = (Left + Right) / 2;	//���ұ߽綼���޸����ģ�ֱ��ȡ��ֵ
		Mid_Line[i] = range_protect(temp, 2, 78);	//�޷�����
		data[80*i + Left+2] = 0;	//����2λ��ʾ��߽磬������Թ۲�
		data[80*i + Right-2] = 0;	//����2λ��ʾ�ұ߽磬������Թ۲�
		data[80*i + Mid_Line[i]] = 0;	//��������ͼ������ʾ����
		if (Mid_Line[i] <= 2 || Mid_Line[i] >= 78)
		{
			i--;
			break;
		}
	}
	Mid_Count = 60 - (i+1);	//��¼�ɹ��������е�ĸ���
//	Mid_Filtering(data);	//������ִ�д��µ����ٴ��µ��������˲�
}


/*
 * Function:  Left_Line_Repair
 * 
 * Description:  
 * ��߽粹������
 * 
 * Requires:
 * x,y     :yΪ���߽�����������xΪ�����������
 * data    :ָ���i�е�ָ��
 */						
void Left_Line_Repair(uchar y, uchar x, uchar *data)
{
	int32 temp;
	uchar i, j, Row;
	/* б�� */
	float Slope;
	
	for (i = 59; i > y; i--)
	{
		/* ��Ҫ���� */
		if (Left_Add[i])
		{
			/* ��59����Ҫ���ߣ��������²� */
			if (i >= 55)
			{
				Row = y;
				Slope = 1.0 * (Left_Line[y] - Left_Line[y-5]) / 5;
				/* ��߽粹��б�ʲ��ܴ���0 */
				if (Slope > 0)
				{
					Slope = 0;
				}				
				for (j = Row+1; j <= 59; j++)
				{
					temp = (int32)((j-Row) * Slope + Left_Line[Row]);
					Left_Line_New[j] = range_protect(temp, 2,78);
					/* ������λ��ͼ������ʾ */
					data[80*j + Left_Line_New[j] + 2] = 0;
				}
			}
			/* �������ϲ� */
			else
			{
				/* ����Ҫ���ߵ�ǰ1�п�ʼ���� */
				Row = i + 1;
				/* ����Ŀ���б�� */
				Slope = 1.0 * (Left_Line[Row] - x) / (Row - y);	
				/* ��߽粹��б�ʲ��ܴ���0 */
				if (Slope > 0)
				{
					Slope = 0;
				}
				for (j = Row; j > y; j--)
				{
					temp = (int32)((j-Row)*Slope + Left_Line[Row]);				
					Left_Line_New[j] = range_protect(temp, 2, 78);
					/* ������λ��ͼ������ʾ */
					data[80*j + Left_Line_New[j] + 2] = 0;							
				}
			}
			/* ֻ���ҵ����ߵ���㼴�ɣ�һ�β��� */
			break;				
		}
	}
}


/*
 * Function:  Right_Line_Repair
 * 
 * Description:  
 * �ұ߽粹������
 * 
 * Requires:
 * x,y     :yΪ���߽�����������xΪ�����������
 * data    :ָ���i�е�ָ��
 */						
void Right_Line_Repair(uchar y, uchar x, uchar *data)
{
	int32 temp;
	uchar i, j, Row;
	/* б�� */
	float Slope;
	
	for (i = 59; i > y; i--)
	{
		/* ��Ҫ���� */
		if (Right_Add[i])	
		{
			/* ��59����Ҫ���ߣ��������²� */
			if (i >= 55)
			{
				Row = y;
				Slope = 1.0 * (Right_Line[y] - Right_Line[y-5]) / 5;
				/* �ұ߽粹��б�ʲ���С��0 */
				if (Slope < 0)	
				{
					Slope = 0;
				}
				for (j = Row+1; j <= 59; j++)
				{
					temp = (int32)((j-Row) * Slope + Right_Line[Row]);
					Right_Line_New[j] = range_protect(temp, 2,78);
					/* ������λ��ͼ������ʾ */
					data[80*j + Right_Line_New[j] - 2] = 0;	
				}
			}
			/* �������ϲ� */
			else
			{
				/* ����Ҫ���ߵ�ǰ1�п�ʼ���� */
				Row = i + 1;
				/* ����Ŀ���б�� */
				Slope = 1.0 * (Right_Line[Row] - x) / (Row - y);
				/* �ұ߽粹��б�ʲ���С��0 */
				if (Slope < 0)	
				{
					Slope = 0;
				}
				for (j = Row; j > y; j--)
				{
					temp = (int32)((j-Row)*Slope + Right_Line[Row]);
					Right_Line_New[j] = range_protect(temp, 2, 78);
					/* ������λ��ͼ������ʾ */
					data[80*j + Right_Line_New[j] - 2] = 0;	
				}
			}
			/* ֻ���ҵ����ߵ���㼴�ɣ�һ�β��� */
			break;	
		}
	}
}


/*
 * Function:  Traversal_Mid_Line
 * 
 * Description:  
 * ���м�������ɨ��߽�
 * 
 * Requires:
 * i       :��i��
 * data    :ָ���i�е�ָ��
 */
void Traversal_Mid_Line(uchar i, uchar *data)
{
	uchar j;
	/* ��ǰһ���е�Ϊ���������ұ߽� */
	for (j = Mid_Line[i + 1]; j >= 2; j--)	
	{
		/* ��⵽���������ڵ㣬���˲���Ч�� */
		if (!data[80*i+j] && !data[80*i+j-1])
		{
			/* ��߽粻��Ҫ���ߣ������־λ */
			Left_Add[i] = 0;		
			/* ��¼��ǰjֵΪ��i����߽� */
			Left_Line[i] = j;		 
//			data[80 * i + j + 2] = 0;//����2λ��ʾ��߽磬������Թ۲�
			
			break;
		}
	}
	/* ��ǰһ���е�Ϊ������Ҳ����ұ߽� */
	for (j = Mid_Line[i + 1]; j < 79; j++)
	{
		/* ��⵽���������ڵ㣬���˲���Ч�� */
		if (!data[80*i+j] && !data[80*i+j+1])
		{
			/* �ұ߽粻��Ҫ���ߣ������־λ */
			Right_Add[i] = 0;	
			/* ��¼��ǰjֵΪ��i���ұ߽� */
			Right_Line[i] = j;		 
//			data[80 * i + j - 2] = 0;//����2λ��ʾ�ұ߽磬������Թ۲�
			
			break;
		}
	}
	/* ��һ�β���ֱ�Ӳ������ұ�Ե�� */
	if (Left_Add[i])	
	{
		Left_Line[i] = 2;
		Left_Line_New[i] = 2;
	}
	if (Right_Add[i])
	{
		Right_Line[i] = 78;	
		Right_Line_New[i] = 78;
	}
}


/*
 * Function:  Traversal_Side_Line
 * 
 * Description:  
 * ���������м�Ѱ�ұ߽磬���ڳ��ָ��ŵ������ʹ��
 * 
 * Requires:
 * i       :��i��
 * data    :ָ���i�е�ָ��
 */
void Traversal_Side_Line(uchar i, uchar *data)
{
	uchar j;
	
	for (j = Left_Line[i+1]; j < 78; j++)
	{
//		if (data[80*i+j] && data[80*i+j-1])//��⵽���������׵㣬���˲���Ч��
		/* ��⵽�׵� */
		if (data[80*i+j])
		{
			/* ��߽粻��Ҫ���ߣ������־λ */
			Left_Add[i] = 0;
			/* ��¼��ǰjֵΪ��i����߽� */
			Left_Line[i] = j;		 
//			data[80 * i + j + 2] = 0;//����2λ��ʾ��߽磬������Թ۲�
			
			break;
		}
	}
	for (j = Right_Line[i+1]; j > 2; j--)
	{
//		if (data[80*i+j] && data[80*i+j+1])//��⵽���������׵㣬���˲���Ч��
		if (data[80*i+j])//��⵽�׵�
		{
			Right_Add[i] = 0;		 //�ұ߽粻��Ҫ���ߣ������־λ
			Right_Line[i] = j;		 //��¼��ǰjֵΪ��i���ұ߽�
//			data[80 * i + j - 2] = 0;//����2λ��ʾ�ұ߽磬������Թ۲�
			
			break;
		}
	}
}


/*
 * Function:  Traversal_Left_Side
 * 
 * Description:  
 * Ѱ�ҵ�i����߽�
 * 
 * Requires:
 * i       :��i��
 * data    :ָ���i�е�ָ��
 */
void Traversal_Left_Side(uchar i, uchar *data)
{
	uchar j;
	
	for (j = Left_Line[i+1]; j < 78; j++)
	{
//		if (data[80*i+j] && data[80*i+j-1])//��⵽���������׵㣬���˲���Ч��
		/* ��⵽�׵� */
		if (data[80*i+j])
		{
			/* ��߽粻��Ҫ���ߣ������־λ */
			Left_Add[i] = 0;		 
			/* ��¼��ǰjֵΪ��i����߽� */
			Left_Line[i] = j;		 
			
			break;
		}
	}
}  /* end Traversal_Left_Side */


/*
 * Function:  Traversal_Right_Side
 * 
 * Description:  
 * Ѱ�ҵ�i���ұ߽�
 * 
 * Requires:
 * i       :��i��
 * data    :ָ���i�е�ָ��
 * 
 * 
 * 
 */
void Traversal_Right_Side(uchar i, uchar *data)
{
	uchar j;
	
	for (j = Right_Line[i+1]; j > 2; j--)
	{
//		if (data[80*i+j] && data[80*i+j-1])//��⵽���������׵㣬���˲���Ч��
		/* ��⵽�׵� */
		if (data[80*i+j])
		{
			/* �ұ߽粻��Ҫ���ߣ������־λ */
			Right_Add[i] = 0;
			/* ��¼��ǰjֵΪ��i���ұ߽� */
			Right_Line[i] = j;		 
			
			break;
		}
	}
}  /* end Traversal_Right_Side */


/*
 * Function:  Traversal_Left
 * 
 * Description:  
 * ������߽�, �������ұ߽�ľ���
 * 
 * Requires:
 * data:һ��ָ��ͼ����ĳ��Ԫ�ص�ָ��
 * 
 * 
 * 
 */
uchar Traversal_Left(uchar *data)
{
	char j, White_Flag = 1;
	uchar Left_Line = 0, Right_Line = 79;
	
	for (j = 0; j < 78; j++)
	{
		/* �Ȳ�����߽� */
		if (White_Flag)	
		{
			/* ��⵽���������׵㣬���˲���Ч�� */
			if (data[j] && data[j+1] && data[j+2])
			{
				/* ��¼��ǰjֵΪ������߽� */
				Left_Line = j;
			}
		}
		else
		{
			/* ��⵽���������ڵ㣬���˲���Ч�� */
			if (!data[j] && !data[j+1] && !data[j+2])
			{
				/* ��¼��ǰjֵΪ�����ұ߽� */
				Right_Line = j;
				
				break;
			}
		}
	}
	
	return (Right_Line - Left_Line);
}  /* end Traversal_Left */


/*
 * Function:  Traversal_Right
 * 
 * Description:  
 * �ұ����߽�, �������ұ߽�ľ���
 * 
 * Requires:
 * data:һ��ָ��ͼ����ĳ��Ԫ�ص�ָ��
 * 
 * 
 * 
 */
uchar Traversal_Right(uchar *data)
{
	char j, White_Flag = 1;
	uchar Left_Line = 0, Right_Line = 79;
	
	for (j = 79; j >= 2; j--)
	{
		/* �Ȳ����ұ߽� */
		if (White_Flag)	
		{
			/* ��⵽���������׵㣬���˲���Ч�� */
			if (data[j] && data[j-1] && data[j-2])
			{
				/* ��¼��ǰjֵΪ�����ұ߽� */
				Right_Line = j;
			}
		}
		else
		{
			/* ��⵽���������ڵ㣬���˲���Ч�� */
			if (!data[j] && !data[j-1] && !data[j-2])
			{
				/* ��¼��ǰjֵΪ������߽� */
				Left_Line = j;	
				
				break;
			}
		}
	}
	
	return (Right_Line - Left_Line);
}  /* end Traversal_Right */


/*
 * Function:  Traversal_Left_Line
 * 
 * Description:  
 * ���ҵ��������i���ҳ����еı߽�ֵ��Ȼ�����Ǳ�����Left_Line��Right_Line����������
 * 
 * Requires:
 * i             :����ָʾ�ǵڼ���
 * data          :ָ��80��60��ͼ�������е�ĳ��Ԫ�ص�ָ��
 * Left_Line     :ָ��Left_Line[]��ĳ��Ԫ�ص�ָ�룬���������i�е���߽����ڵ�����
 * Right_Line    :ָ��Right_Line[]��ĳ��Ԫ�ص�ָ�룬���������i�е��ұ߽����ڵ�����
 * 
 * 
 * 
 */
void Traversal_Left_Line(uchar i, uchar *data, uchar *Left_Line, uchar *Right_Line)
{
	char j, White_Flag = 1;
	
	for (j = 0; j < 78; j++)
	{
		/* ������߽�,����߽�ֵ������������ */
		if (White_Flag)	
		{
			/* ��⵽���������׵㣬���˲���Ч�� */
			if (data[j] && data[j+1] && data[j+2])
			{
				/* ��߽����ҵ��������־λ */
				White_Flag = 0;
				/* ��߽粻��Ҫ���ߣ������־λ */
				Left_Add[59] = 0;
				/* ��¼��ǰjֵΪ������߽� */
				*Left_Line = j;	
			}
		}
		/* �ҵ���߽��Ժ�����ұ߽� */
		else	
		{
			/* ��⵽���������ڵ㣬���˲���Ч�� */
			if (!data[j] && !data[j+1] && !data[j+2])
			{
				/* �ұ߽粻��Ҫ���ߣ������־λ */
				Right_Add[59] = 0;
				/* ��¼��ǰjֵΪ�����ұ߽� */
				*Right_Line = j;

				break;
			}
		}
	}
}  /* end Traversal_Left_Line */


/*
 * Function:  Traversal_Right_Line
 * 
 * Description:  
 * �����ұ�����i���ҳ����еı߽�ֵ��Ȼ�����Ǳ�����Left_Line��Right_Line����������
 * 
 * Requires:
 * i             :����ָʾ�ǵڼ���
 * data          :ָ��80��60��ͼ�������е�ĳ��Ԫ�ص�ָ��
 * Left_Line     :ָ��Left_Line[]��ĳ��Ԫ�ص�ָ�룬���������i�е���߽����ڵ�����
 * Right_Line    :ָ��Right_Line[]��ĳ��Ԫ�ص�ָ�룬���������i�е��ұ߽����ڵ�����
 * 
 * 
 * 
 */
void Traversal_Right_Line(uchar i, uchar *data, uchar *Left_Line, uchar *Right_Line)
{
	char j, White_Flag = 1;
	
	for (j = 79; j >= 2; j--)
	{
		/* �Ȳ����ұ߽� */
		if (White_Flag)
		{
			/* ��⵽���������׵㣬���˲���Ч�� */
			if (data[j] && data[j-1] && data[j-2])
			{
				/* �ұ߽����ҵ��������־λ */
				White_Flag = 0; 
				/* �ұ߽粻��Ҫ���ߣ������־λ */
				Right_Add[59] = 0; 	
				/* ��¼��ǰjֵΪ�����ұ߽� */
				*Right_Line = j;
			}
		}
		/* �ҵ��ұ߽��Ժ������߽� */
		else	
		{
			/* ��⵽���������ڵ㣬���˲���Ч�� */
			if (!data[j] && !data[j-1] && !data[j-2])
			{
				/* ��߽粻��Ҫ���ߣ������־λ */
				Left_Add[59] = 0;	
				/* ��¼��ǰjֵΪ������߽� */
				*Left_Line = j;	

				break;
			}
		}
	}
}  /* end Traversal_Right_Line */


/*!
*  @brief      ��ֵ��ͼ���ѹ���ռ� �� ʱ�� ��ѹ��
*  @param      dst             ͼ���ѹĿ�ĵ�ַ
*  @param      src             ͼ���ѹԴ��ַ
*  @param      srclen          ��ֵ��ͼ���ռ�ÿռ��С
*  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
*  Sample usage:
*/
void img_extract(uint8 *dst, uint8 *src, uint32 srclen)
{
  uint8 tmpsrc;
  while(srclen --)
  {
    tmpsrc = *src++;
    *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
  }
}  /* end img_extract */

/*!
*  @brief      ��ֵ��ͼ��ѹ�����ռ� �� ʱ�� ѹ����
*  @param      dst             ͼ��ѹ��Ŀ�ĵ�ַ
*  @param      src             ͼ��ѹ��Դ��ַ
*  @param      srclen          ��ֵ��ͼ���ռ�ÿռ��С
*  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
*  Sample usage:
*/
void img_recontract(uint8 *dst, uint8 *src, uint32 srclen)
{
  uint8 tmpsrc;
  
  while(srclen --)
  {
    tmpsrc=0;
    if(*dst++ == colour[black])
    {
      tmpsrc = tmpsrc + 0x80;
    }
    
    if(*dst++ == colour[black])
    {
      tmpsrc = tmpsrc + 0x40;
    }
    
    if(*dst++ == colour[black])
    {
      tmpsrc = tmpsrc + 0x20;
    }
    
    if(*dst++ == colour[black])
    {
      tmpsrc = tmpsrc + 0x10;
    }
    
    if(*dst++ == colour[black])
    {
      tmpsrc = tmpsrc + 0x08;
    }
    
    if(*dst++ == colour[black])
    {
      tmpsrc = tmpsrc + 0x04;
    }
    
    if(*dst++ == colour[black])
    {
      tmpsrc = tmpsrc + 0x02;
    }
    
    if(*dst++ == colour[black])
    {
      tmpsrc = tmpsrc + 0x01;
    }
    
    *src++ = tmpsrc;
  }
}  /* end img_recontract */

/*!
*  @brief      ȡ�߽���
*  @param      dst             ͼ��ѹ��Ŀ�ĵ�ַ
*  @param      src             ͼ��ѹ��Դ��ַ
*  @param      srclen          ��ֵ��ͼ���ռ�ÿռ��С
*  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
*  Sample usage:
*/
void img_getline(uint8 *dst, uint8 *src, uint32 srclen)
{
  uint8 tmpsrc,buff_y,buff_x;
  
  while(srclen --)
  {
    tmpsrc=0;
    buff_x = *(dst + 80);
    buff_y = *dst++;
    *(dst-1)=colour[white];
    if(*dst != buff_y || (buff_y != buff_x && srclen > 9))
    {
      tmpsrc = tmpsrc + 0x80;
      *(dst-1)=colour[black];
    }
    
    buff_x = *(dst + 80);
    buff_y = *dst++;
    *(dst-1)=colour[white];
    if(*dst != buff_y || (buff_y != buff_x && srclen > 9))
    {
      tmpsrc = tmpsrc + 0x40;
      *(dst-1)=colour[black];
    }
    
    buff_x = *(dst + 80);
    buff_y = *dst++;
    *(dst-1)=colour[white];
    if(*dst != buff_y || (buff_y != buff_x && srclen > 9))
    {
      tmpsrc = tmpsrc + 0x20;
      *(dst-1)=colour[black];
    }
    
    buff_x = *(dst + 80);
    buff_y = *dst++;
    *(dst-1)=colour[white];
    if(*dst != buff_y || (buff_y != buff_x && srclen > 9))
    {
      tmpsrc = tmpsrc + 0x10;
      *(dst-1)=colour[black];
    }
    
    buff_x = *(dst + 80);
    buff_y = *dst++;
    *(dst-1)=colour[white];
    if(*dst != buff_y || (buff_y != buff_x && srclen > 9))
    {
      tmpsrc = tmpsrc + 0x08;
      *(dst-1)=colour[black];
    }
    
    buff_x = *(dst + 80);
    buff_y = *dst++;
    *(dst-1)=colour[white];
    if(*dst != buff_y || (buff_y != buff_x && srclen > 9))
    {
      tmpsrc = tmpsrc + 0x04;
      *(dst-1)=colour[black];
    }
    
    buff_x = *(dst + 80);
    buff_y = *dst++;
    *(dst-1)=colour[white];
    if(*dst != buff_y || (buff_y != buff_x && srclen > 9))
    {
      tmpsrc = tmpsrc + 0x02;
      *(dst-1)=colour[black];
    }
    
    buff_x = *(dst + 80);
    buff_y = *dst++;
    *(dst-1)=colour[white];
    if((*dst != buff_y && srclen % 10 !=0 ) || (buff_y != buff_x && srclen > 9))
    {
      tmpsrc = tmpsrc + 0x01;
      *(dst-1)=colour[black];
    }
    
    *src++ = tmpsrc;
  }
}  /* end img_getline */
/*************************************************************
 *Function:PointWeightAdjust
 *Description:Adjust the weight array
 * - PointWeight: Original PointWeight array.
 * - AdjustCount: The count you want to adjust.
 * - Direction  : Adjust to Right(0) or Left(1)
 ************************************************************/

void PointWeightAdjust(uchar *PointWeight, uchar AdjustCount, uchar Direction)
{
    char i;
	/* Ȩ������������� */
	if(Direction == 1)
	{
		for(i = AdjustCount; i < 60; i++)
		{
			PointWeight[i] = PointWeight[i - AdjustCount];
			
		}
		for(i = 0; i < AdjustCount; i++)
		{
			PointWeight[i] = 1;
		}

	}
	/* Ȩ���������ҵ��� */
	if(Direction == 0)
	{
		for(i = 0; i < 60 - AdjustCount; i++)
		{
			PointWeight[i] = PointWeight[i + AdjustCount];
			
		}
		for(i = 60 - AdjustCount; i < 60; i++)
		{
			PointWeight[i] = 1;
		}

	}
	
}