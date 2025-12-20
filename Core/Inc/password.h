/**
  ******************************************************************************
  * File Name          : password.h
  * Description        : 密码系统头文件
  ******************************************************************************
  */

#ifndef __PASSWORD_H
#define __PASSWORD_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal_flash_ex.h"

/* Define --------------------------------------------------------------------*/
#define PASSWORD_LENGTH 4    // 密码长度
#define PASSWORD_MAX_TRIES 3 // 最大尝试次数

// 定义密码存储的FLASH扇区和地址
// STM32F401RE的FLASH扇区划分：扇区11是最后一个扇区(128KB)，地址从0x080E0000开始
#define PASSWORD_FLASH_SECTOR     FLASH_SECTOR_7
#define PASSWORD_FLASH_ADDRESS    0x080E0000

/* Exported constants --------------------------------------------------------*/
// 密码状态枚举
typedef enum {
    PASSWORD_IDLE,           // 空闲状态：密码系统初始化后或操作完成后的等待状态
    PASSWORD_INPUTTING,      // 输入状态：用户正在输入密码的过程中
    PASSWORD_VERIFYING,      // 验证状态：密码输入完成，正在与存储的密码进行比较验证
    PASSWORD_SUCCESS,        // 成功状态：密码验证通过
    PASSWORD_FAILED,         // 失败状态：密码验证失败，可能需要重新输入
    PASSWORD_CHANGING        // 更改状态：用户正在进行密码修改操作
} PasswordState;

/* Exported variables --------------------------------------------------------*/
extern PasswordState passwordState;
extern uint8_t inputPassword[PASSWORD_LENGTH];
extern uint8_t storedPassword[PASSWORD_LENGTH];
extern uint8_t inputCount;
extern uint8_t failedAttempts;

/* Exported functions prototypes ---------------------------------------------*/
void Password_Init(void);
void Password_Input(uint8_t key);
uint8_t Password_Verify(void);
void Password_Change(void);
void Password_Display(void);

void Password_SaveToEEPROM(void);
void Password_LoadFromEEPROM(void);

#endif /* __PASSWORD_H */