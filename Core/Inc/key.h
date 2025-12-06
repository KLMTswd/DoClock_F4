#ifndef __KEY_H__
#define __KEY_H__

#include "main.h"

void key_Tick(void);
uint8_t Key_GetNum(void);
uint8_t Key_GetState(void);

extern volatile uint8_t Key_Num;
extern const uint8_t KeyMap[4][4];


// 硬件配置定义
#define  Rows  4			      //行数
#define  Cols  4                  //列数
#define  KEY_DEBOUNCE_MS   20     //去抖动时间设定

#endif

