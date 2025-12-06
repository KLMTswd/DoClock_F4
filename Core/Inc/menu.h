/**
  ******************************************************************************
  * File Name          : menu.h
  * Description        : 菜单系统头文件
  ******************************************************************************
  */

#ifndef MENU_H
#define MENU_H

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "key.h"
#include "OLED.h"

/* Exported defines ----------------------------------------------------------*/
// 按键定义宏
#define KEY_DOWN 1    // 向下选择键
#define KEY_OK   2    // 确认键
#define KEY_BACK 3    // 返回键

/* Exported typedef ----------------------------------------------------------*/
// 菜单结构体定义
//先安排定义数组，在定义一些成员变量
typedef struct {           
    char title[20];         // 菜单项标题
    char content[4][20];    // 菜单项内容（最多3个，第4个用于预留）
    uint8_t itemCount;      // 实际菜单项数量
    uint8_t nextLevel;      // 下一级菜单ID，如果为0表示没有下一级
    uint8_t ledMode;        // LED模式，用于LED菜单选项
} MenuItem;

/* Exported variables --------------------------------------------------------*/
// 全局变量外部声明
extern uint8_t currentMenuLevel;  // 当前菜单级别
extern uint8_t currentSelection;  // 当前选中项
extern uint8_t menuNeedsInit[4];  // 各级菜单初始化标志
extern uint8_t LED_mode;          // LED工作模式
extern uint8_t menuStack[3];      // 菜单选择栈，用于记录用户的选择路径
extern uint8_t stackPointer;      // 栈指针

// 菜单数据数组外部声明
extern MenuItem menuData[3];      // 第一级和第二级菜单
extern MenuItem menuLevel3[9];    // 第三级菜单

/* Exported functions prototypes ---------------------------------------------*/
// 通用菜单显示函数
void displayMenu(MenuItem *menu, uint8_t selection);

// 更新选择位置的通用函数（必须有）
void updateSelection(uint8_t *selection, uint8_t maxItems);

// 菜单导航的通用函数
void navigateMenu(uint8_t key);

// 获取当前活动菜单项
MenuItem* getCurrentMenu(void);

// 重置菜单系统
void resetMenuSystem(void);

#endif /* MENU_H */
