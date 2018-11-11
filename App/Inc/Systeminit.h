#ifndef _PORTINIT_H_
#define _PORTINIT_H_

#include "include.h"


/* LED端口初始化 */
void ledinit(void);

/* 电机和舵机端口初始化 */
void motorinit(void);

/* 设置中断优先级，中断函数映射，开中断*/
void setvector(void);


#endif /* _PORTINIT_H_ */