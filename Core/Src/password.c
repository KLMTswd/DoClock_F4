/**
  ******************************************************************************
  * File Name          : password.c
  * Description        : 密码系统实现文件
  ******************************************************************************
  */

#include <string.h>
#include "password.h"
#include "oled.h"

#ifndef PASSWORD_FLASH_SECTOR
#define PASSWORD_FLASH_SECTOR 11U
#endif

#ifndef PASSWORD_FLASH_ADDRESS
#define PASSWORD_FLASH_ADDRESS 0x080E0000U
#endif


/* Global variables ----------------------------------------------------------*/
PasswordState passwordState = PASSWORD_IDLE;
uint8_t inputPassword[PASSWORD_LENGTH] = {0};
uint8_t storedPassword[PASSWORD_LENGTH] = {1, 2, 3, 4}; // 默认密码1234
uint8_t inputCount = 0;
uint8_t failedAttempts = 0;

/**
  * @brief  密码系统初始化
  * @retval None
  */
void Password_Init(void)
{
    passwordState = PASSWORD_IDLE;
    inputCount = 0;
    failedAttempts = 0;
    memset(inputPassword, 0, PASSWORD_LENGTH);
    
    // 可以在这里从EEPROM加载密码
}

/**
  * @brief  处理密码输入
  * @param  key: 输入的按键值 (0-9)
  * @retval None
  */
void Password_Input(uint8_t key)
{
    if (passwordState == PASSWORD_IDLE || passwordState == PASSWORD_FAILED)
    {
        passwordState = PASSWORD_INPUTTING;
        inputCount = 0;
        memset(inputPassword, 0, PASSWORD_LENGTH);
        OLED_Clear();
        OLED_ShowString(1, 1, "Password:");
    }
    
    if (passwordState == PASSWORD_INPUTTING && inputCount < PASSWORD_LENGTH)
    {
        // 只接受0-9的数字输入
        if (key >= 0 && key <= 9)
        {
            inputPassword[inputCount] = key;
            inputCount++;
            
            // 在OLED上显示输入的密码（用*表示）
            OLED_ShowChar(2, 1 + inputCount, '*');
            
            // 如果输入了4位密码，进行验证
            if (inputCount == PASSWORD_LENGTH)
            {
                passwordState = PASSWORD_VERIFYING;
                if (Password_Verify())
                {
                    passwordState = PASSWORD_SUCCESS;
                    failedAttempts = 0;
                    OLED_Clear();
                    OLED_ShowString(1, 1, "Success!");
                    // 可以在这里添加验证成功后的处理
                }
                else
                {
                    passwordState = PASSWORD_FAILED;
                    failedAttempts++;
                    OLED_Clear();
                    OLED_ShowString(1, 1, "Failed!");
                    OLED_ShowString(2, 1, "Tries:");
                    OLED_ShowNum(2, 7, PASSWORD_MAX_TRIES - failedAttempts, 1);
                    
                    // 如果尝试次数用完，可以添加锁定功能
                    if (failedAttempts >= PASSWORD_MAX_TRIES)
                    {
                        OLED_ShowString(3, 1, "Locked!");
                        // 可以在这里添加锁定逻辑
                    }
                }
            }
        }
    }
}

/**
  * @brief  验证密码
  * @retval 1: 密码正确, 0: 密码错误
  */
uint8_t Password_Verify(void)
{
    for (uint8_t i = 0; i < PASSWORD_LENGTH; i++)
    {
        if (inputPassword[i] != storedPassword[i])
        {
            return 0; // 密码错误
        }
    }
    return 1; // 密码正确
}

/**
  * @brief  修改密码
  * @retval None
  */
void Password_Change(void)
{
    // 这里需要实现密码修改逻辑
    // 通常流程：输入旧密码验证 -> 输入新密码 -> 确认新密码
    
    // 示例实现（简化版）
    passwordState = PASSWORD_CHANGING;
    uint8_t step = 0;
    uint8_t newPassword[PASSWORD_LENGTH] = {0};
    uint8_t confirmPassword[PASSWORD_LENGTH] = {0};
    
    OLED_Clear();
    OLED_ShowString(1, 1, "Change PW:");
    OLED_ShowString(2, 1, "1.Enter new:");
    
    // 这里需要根据实际的键盘输入来实现完整的修改流程
    // 可以在FreeRTOS任务中处理
}

/**
  * @brief  在OLED上显示密码相关信息
  * @retval None
  */
void Password_Display(void)
{
    switch (passwordState)
    {
        case PASSWORD_IDLE:
            OLED_ShowString(1, 1, "Enter Password:");
            break;
        case PASSWORD_INPUTTING:
            OLED_ShowString(1, 1, "Password:");
            // 显示已输入的*号
            for (uint8_t i = 0; i < inputCount; i++)
            {
                OLED_ShowChar(2, 1 + i, '*');
            }
            break;
        // 其他状态的显示可以根据需要添加
        default:
            break;
    }
}


/**
  * @brief  将密码保存到FLASH
  * @param  storedPassword: 要存储的密码数组指针
  * @retval 无
  */
void Password_SaveToEEPROM(void)
{
  // 定义FLASH操作状态变量，用于检查操作是否成功
    HAL_StatusTypeDef status;
    
  // 定义FLASH擦除初始化结构体，用于配置擦除参数
    FLASH_EraseInitTypeDef eraseInit;
    
  // 定义扇区错误变量，用于存储擦除失败时的扇区地址
    uint32_t sectorError = 0;
    
    
  // 1. 解锁FLASH - FLASH默认是锁定状态，必须先解锁才能进行读写操作
    HAL_FLASH_Unlock();
    
  // 2. 擦除扇区 - FLASH写入前必须先擦除对应扇区
    eraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;   // 设置擦除类型为扇区擦除
    eraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;  // 设置供电电压范围为3.3V (FLASH操作需要正确的电压范围)
    eraseInit.Sector = PASSWORD_FLASH_SECTOR;        // 设置要擦除的扇区编号 (由宏定义指定)
    eraseInit.NbSectors = 1;                         // 设置要擦除的扇区数量 (这里只擦除1个扇区)
    
    // 执行扇区擦除操作
    status = HAL_FLASHEx_Erase(&eraseInit, &sectorError);
    // 检查擦除操作是否成功
    if (status != HAL_OK)
    {
        // 擦除失败处理 - 锁定FLASH并返回
        HAL_FLASH_Lock();
        return;
    }
    
    // 3. 写入密码到FLASH - 擦除成功后，将密码逐个字节写入FLASH
    for (uint8_t i = 0; i < PASSWORD_LENGTH; i++)
    {
        // 执行字节写入操作，参数依次为：写入类型(字节)、写入地址、写入数据
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, PASSWORD_FLASH_ADDRESS + i, storedPassword[i]);
        // 检查写入操作是否成功
        if (status != HAL_OK)
        {
            // 写入失败处理 - 锁定FLASH并返回
            HAL_FLASH_Lock();
            return;
        }
    }
    
    // 4. 锁定FLASH - 操作完成后锁定FLASH，防止误操作
    HAL_FLASH_Lock();
}


/**
  * @brief  从FLASH加载密码
  * @retval None
  */
void Password_LoadFromEEPROM(void)
{
    // 定义密码存储的FLASH地址
    // PASSWORD_FLASH_ADDRESS is defined above
    
    // 读取密码
    for (uint8_t i = 0; i < PASSWORD_LENGTH; i++)
    {
        storedPassword[i] = *(__IO uint8_t *)(PASSWORD_FLASH_ADDRESS + i);
    }
    
    // 检查是否是第一次使用（FLASH内容为0xFF）
    uint8_t isFirstUse = 1;
    for (uint8_t i = 0; i < PASSWORD_LENGTH; i++)
    {
        if (storedPassword[i] != 0xFF)
        {
            isFirstUse = 0;
            break;
        }
    }
    
    // 如果是第一次使用，设置默认密码1234
    if (isFirstUse)
    {
        storedPassword[0] = 1;
        storedPassword[1] = 2;
        storedPassword[2] = 3;
        storedPassword[3] = 4;
        
        // 保存默认密码到FLASH
        Password_SaveToEEPROM();
    }
}