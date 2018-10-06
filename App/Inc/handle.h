#ifndef __HANDLE_H__
#define __HANDLE_H__
#include "include.h"
#include "VCAN_OV7725_Eagle.h"

extern uint8 screen_mid;
extern uchar Foresight;

extern uchar Mid_Line[];
extern uchar Mid_Count;
extern uchar Point_Mid;
extern uchar Turn_Left, Turn_Right;
extern uchar Foresight_Left, Foresight_Right;										 //前瞻左右边界极值
extern uchar Starting_Line_Flag;													 //起跑线标志位
extern uint8 Point;                                                                  //图像中点值
extern uint8 imgbuff[CAMERA_SIZE];                                                   //采集到的图像
extern uint8 img[CAMERA_W*CAMERA_H];                                                 //用来存放解压后的图像


uchar Image_GetLine(uchar *data);													 //获取左中右边界线
void First_Line_Add(uchar *data);													 //用于处理第一行找不到点的特殊情况
uchar Point_Handle(void);															 //加上路肩后只使用简单加权算法保证稳定过弯
uchar Point_Weight(void);															 //使用加权平均获取目标横坐标
void Point_Scan(void);																 //选取目标点
char Error_Transform(uchar Data, uchar Set);
int32 Curvature_Transform(uchar Now, uchar Point);									 //曲率转化，结果放大1000倍
int32 Angle_Transform(uchar *data);													 //解算目标角度
void Mid_Filtering(uchar *data);													 //中线滤波
uint32 Wave_Filtering(uint32 data, uchar Deep);										 //滤波函数
double Slope_Display(uchar *data);													 //斜率线显示
void Mid_Line_Repair(uchar count, uchar *data);		 								 //中线修复
void Left_Line_Repair(uchar y, uchar x, uchar *data);          						 //左边界补线修正
void Right_Line_Repair(uchar y, uchar x, uchar *data);								 //右边界补线修正
uchar Traversal_Left(uchar *data);													 //左遍历边界, 返回左右边界的距离
uchar Traversal_Right(uchar *data); 												 //右遍历边界, 返回左右边界的距离
void Traversal_Mid_Line(uchar i, uchar *data);										 //从中间向两边扫描边界，记录左右边界
void Traversal_Left_Side(uchar i, uchar *data);										 //从前一行左边界开始扫描左边界
void Traversal_Right_Side(uchar i, uchar *data);									 //从前一行右边界开始扫描右边界
void Traversal_Side_Line(uchar i, uchar *data);										 //从两边向中间寻找边界，仅在出现干扰的情况下使用
void Traversal_Left_Line(uchar i, uchar *data, uchar *Left_Line, uchar *Right_Line); //左遍历边界, 记录左右边界
void Traversal_Right_Line(uchar i, uchar *data, uchar *Left_Line, uchar *Right_Line);//右遍历边界, 记录左右边界


void img_extract(uint8 *dst, uint8 *src, uint32 srclen);                          	 //解压
void img_recontract(uint8 *dst, uint8 *src, uint32 srclen);     					 //压缩
void img_getline(uint8 *dst, uint8 *src, uint32 srclen);        					 //提取边界线

#endif /* __HANDLE_H__ */