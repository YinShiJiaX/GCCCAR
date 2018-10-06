#ifndef __PID_H__
#define __PID_H__
#include "include.h"
#include "common.h"

#define KP 0
#define KI 1
#define KD 2
#define KT 3

#define DIFFSPEED	if(Point >= 50)\
    				{\
        				ftm_pwm_init(FTM0, FTM_CH0, 10*1000, 25);\
    					Point += 10;\
    				}\
    				else\
    				{\
        				ftm_pwm_init(FTM0, FTM_CH0, 10*1000, 13);\
    				}

typedef struct PID
{
	/* 误差累计 */
	long SumError;		
	/*上次误差*/
	int32 LastError;	//Error[-1]
	/* 上上次误差 */
	int32 PrevError;	//Error[-2]	
	/* 上次速度 */
	int32 LastSpeed;	//Speed[-1]
} PID;
/* 定义舵机和电机的PID参数结构体 */
extern PID S_D5_PID, MOTOR_PID;

/* 位置式PID参数初始化 */
void PlacePID_Init(PID *sptr);

/* 增量式PID参数初始化 */
void IncPID_Init(PID *sptr);

/* 位置式PID舵机控制 */
int32 PlacePID_Control(PID *sprt, int32 NowPiont, int32 SetPoint);

/*增量式串级PID控制 */
int32 PID_Cascade(PID *sprt, int32 NowPiont, int32 SetPoint);

/* 增量式PID控制 */
int32 PID_Realize(PID *sptr, int32 ActualSpeed, int32 SetSpeed);

#endif /* __PID_H__ */
