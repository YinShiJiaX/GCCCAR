
#ifndef _OLED_H
#define _OLED_H
#include "common.h"
#include "include.h"
#include "MK60_gpio.h"
//#include "delay.h"
#define delayms(ms)    systick_delay_ms(ms)


#define OLED_SCL  PTB21
#define OLED_SDA  PTB22
#define OLED_RST  PTB6
#define OLED_DC   PTB5



#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xCF

#define X_WIDTH 128
#define Y_WIDTH 64

#define LED_IMAGE_WHITE       0x01
#define LED_IMAGE_BLACK       0

#define LED_MAX_ROW_NUM      64
#define LED_MAX_COLUMN_NUM  128


void OLED_Init(void);
void OLED_CLS(void);
void OLED_Fill(u8 bmp_data);

void OLED_PutPixel(u8 x,u8 y);							//����һ���㣨x,y��
void OLED_Rectangle(u8 x1,u8 y1,u8 x2,u8 y2,u8 gif);            //����һ��ʵ�ľ���
void OLED_P6x8Str(u8 x,u8 y,u8 ch[]);				    //�ַ���
void OLED_P8x16Str(u8 x,u8 y,u8 ch[]);				    //�ַ���
void OLED_P14x16Str(u8 x,u8 y,u8 ch[]);			        //���ִ�
void OLED_Print(u8 x, u8 y, u8 ch[]);		//��ʾ�����ַ���� OLED_Print(6,6,"ֱ��Saaaa");


void Draw_BMP(u8 x0,u8 y0,u8 x1,u8 y1,u8 bmp[]);	    //��ʾͼƬ

void OLED_Drawtu(void);       //��ʾͼƬ-С��
void OLED_Picture(void);      //��ʾͼƬ-����

void OLED_PrintImage(u8 *pucTable, u16 usRowNum, u16 usColumnNum);     //ͼ��ɼ���ʾ

void OLED_Picture_sign(void);   //��ʾ��ǣ������У�



void OLED_nurbem(u8 x,u8 y,u8 a,u8 b);                      //����
void OLED_nurbem_big(u8 x,u8 y,u8 a,u8 b);
void OLED_nurbem_int(u8 x,u8 y,u16 a);
void OLED_float(u8 x,u8 y,float a);     //������




//OLED_Init();             //----OLED��ʾ��ʼ��
//OLED_Print(0,0,"OLED-CHECK-OK!");     //----����OLED�ܷ���������

#endif
