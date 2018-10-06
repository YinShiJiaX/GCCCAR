#ifndef __CONTROL_H__
#define __CONTROL_H__
#include "include.h"
#include "common.h"

/* 
 *定义电机端口(PTC1~PTC4)
 *定义电机工作频率
 */
#define MOTOR_FTM   FTM0
#define MOTOR1_PWM  FTM_CH0
#define MOTOR2_PWM  FTM_CH1
#define MOTOR3_PWM  FTM_CH2
#define MOTOR4_PWM  FTM_CH3
#define MOTOR_HZ    (10*1000)	/* 滑行模式下，频率应该是 30~100。 常规模式下，频率应该是 20k 左右 */

/* 
 *定义舵机端口(PTB0) 
 *舵机频率 100(HZ)
 *8420(新板子),8550(坏板子)
 */
#define S_D5_FTM FTM1 
#define S_D5_CH  FTM_CH0
#define S_D5_HZ  (100)
#define S_D5_MID 8420
								
extern int32 MOTOR_Duty;
extern int32 MOTOR_Speed;
extern int32 RC_Get;
extern uchar Run_Flag;
extern uchar Game_End;
extern uchar Stop_Flag;
extern uchar BangBang_Flag;
extern char Set;
extern int32 S_D5_Duty;
/* 目标点横坐标 */
extern uint8   Point_Mid;
/* 正交解码脉冲计数，必须为 int32 ！！！ */
extern int32 Pulses_Count;


/* 电机速度测量 */
void speed_measure();
/* 起跑线检测与停车控制 */
void Start_Control(void);
/* 增量式PID调控电机速度 */
void MOTOR_Control(void);
/* 限幅保护 */
int32 range_protect(int32 duty, int32 min, int32 max); 

#endif /* __CONTROL_H__ */