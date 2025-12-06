/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "key.h"
#include "OLED.h"
#include "menu.h"
#include "string.h"
#include "queue.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
  
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Menu1 */
osThreadId_t Menu1Handle;
const osThreadAttr_t Menu1_attributes = {
  .name = "Menu1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Menu2 */
osThreadId_t Menu2Handle;
const osThreadAttr_t Menu2_attributes = {
  .name = "Menu2",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Menu3 */
osThreadId_t Menu3Handle;
const osThreadAttr_t Menu3_attributes = {
  .name = "Menu3",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for KeyScan */
osThreadId_t KeyScanHandle;
const osThreadAttr_t KeyScan_attributes = {
  .name = "KeyScan",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for KeyProcess */
osThreadId_t KeyProcessHandle;
const osThreadAttr_t KeyProcess_attributes = {
  .name = "KeyProcess",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for xKeyIntQueue */
osMessageQueueId_t xKeyIntQueueHandle;
const osMessageQueueAttr_t xKeyIntQueue_attributes = {
  .name = "xKeyIntQueue"
};
/* Definitions for xKeyValueQueue */
osMessageQueueId_t xKeyValueQueueHandle;
const osMessageQueueAttr_t xKeyValueQueue_attributes = {
  .name = "xKeyValueQueue"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void vTaskMenu1(void *argument);
void vTaskMenu2(void *argument);
void vTaskMenu3(void *argument);
void vTaskKeyScan(void *argument);
void vTaskKeyProcess(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of xKeyIntQueue */
  xKeyIntQueueHandle = osMessageQueueNew (10, sizeof(uint16_t), &xKeyIntQueue_attributes);

  /* creation of xKeyValueQueue */
  xKeyValueQueueHandle = osMessageQueueNew (16, sizeof(uint16_t), &xKeyValueQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of Menu1 */
  Menu1Handle = osThreadNew(vTaskMenu1, NULL, &Menu1_attributes);

  /* creation of Menu2 */
  Menu2Handle = osThreadNew(vTaskMenu2, NULL, &Menu2_attributes);

  /* creation of Menu3 */
  Menu3Handle = osThreadNew(vTaskMenu3, NULL, &Menu3_attributes);

  /* creation of KeyScan */
  KeyScanHandle = osThreadNew(vTaskKeyScan, NULL, &KeyScan_attributes);

  /* creation of KeyProcess */
  KeyProcessHandle = osThreadNew(vTaskKeyProcess, NULL, &KeyProcess_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_vTaskMenu1 */
/**
* @brief Function implementing the Menu1 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_vTaskMenu1 */
void vTaskMenu1(void *argument)
{
  /* USER CODE BEGIN vTaskMenu1 */

	osDelay(100);    
	OLED_Init();				
	OLED_Clear();
  

  /* Infinite loop */
  for(;;)
  {
    OLED_ShowString(1, 1, "Hello MyFriend!");
    // // 只在当前是第一级菜单时执行
    //   if (currentMenuLevel == 1) 
    //   {
    //   // 菜单初始化显示
    //     if (menuNeedsInit[1] == 1) 
    //     {      
    //       // 使用通用函数显示菜单
    //       displayMenu(&menuData[0], currentSelection);

    //       // 标记第一级菜单已初始化
    //       menuNeedsInit[1] = 0;      

    //     }
        
    //   // 处理按键
    //     if (Key_Num != 0) 
    //     {
    //       // 使用通用导航函数处理菜单导航
    //         navigateMenu(Key_Num);

    //     }
    //   }
            
      // 任务延时100ms，让出CPU给其他任务
      osDelay(100);
  }
  /* USER CODE END vTaskMenu1 */
}

/* USER CODE BEGIN Header_vTaskMenu2 */
/**
* @brief Function implementing the Menu2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_vTaskMenu2 */
void vTaskMenu2(void *argument)
{
  /* USER CODE BEGIN vTaskMenu2 */
  /* Infinite loop */
  for(;;)
  {
    // // 只在当前是第二级菜单时执行
    // if (currentMenuLevel == 2) 
    // {

    // // 使用getCurrentMenu获取当前活动菜单
    //   MenuItem* currentMenu = getCurrentMenu();
      
    // // 菜单初始化显示
    //   if (menuNeedsInit[2] == 1 && currentMenu != NULL) 
    //   {
    //     // 使用通用函数显示菜单
    //       displayMenu(currentMenu, currentSelection);
    //     // 标记第二级菜单已初始化
    //       menuNeedsInit[2] = 0;
    //   }
      
    // // 处理按键
    //   if (Key_Num != 0) 
    //   {
    //       navigateMenu(Key_Num);
    //   }
    // }
    
    // 任务延时50ms，让出CPU给其他任务
    osDelay(50);
  }
  /* USER CODE END vTaskMenu2 */
}

/* USER CODE BEGIN Header_vTaskMenu3 */
/**
* @brief Function implementing the Menu3 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_vTaskMenu3 */
void vTaskMenu3(void *argument)
{
  /* USER CODE BEGIN vTaskMenu3 */
  /* Infinite loop */
  for(;;)
  {
    // // 只在当前是第三级菜单时执行
    // if (currentMenuLevel == 3) 
    // {

    // // 使用getCurrentMenu获取当前活动菜单
    //   MenuItem* currentMenu = getCurrentMenu();
      
    // // 显示菜单内容
    //   if (currentMenu != NULL && menuNeedsInit[3] == 1) 
    //   {
    //     // 显示标题
    //       OLED_ShowString(1, 1, currentMenu->title);
          
    //     // 显示内容
    //       for (uint8_t i = 0; i < 4; i++) 
    //       {

    //         if (strlen((char*)currentMenu->content[i]) > 0) 
    //         {

    //           OLED_ShowString(i + 1, 3, currentMenu->content[i]);
    //         }
    //       }
          
    //       // 如果是LED相关菜单，设置LED模式
    //       if (currentMenu->ledMode != 0) 
    //       {
    //         LED_mode = currentMenu->ledMode;

    //       }
          
    //       menuNeedsInit[3] = 0;
    //   }
      
    //   // 处理返回键
    //   if (Key_Num == KEY_BACK) 
    //   {
    //     navigateMenu(KEY_BACK);
    //   }

    // }
    
    // 任务延时50ms
    osDelay(50);
  }
  /* USER CODE END vTaskMenu3 */
}

/* USER CODE BEGIN Header_vTaskKeyScan */
/**
* @brief Function implementing the KeyScan thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_vTaskKeyScan */
void vTaskKeyScan(void *argument)
{
  /* USER CODE BEGIN vTaskKeyScan */
  uint8_t row;
	uint8_t received_val;  // 从队列接收的值（仅用于通知，实际值未使用）
	uint8_t key_value = 0;   // 按键值（高4位表示行，低4位表示列）

  /* Infinite loop */
  for(;;)
  {
		// 等待从队列接收信号（由外部中断回调函数触发）
		// portMAX_DELAY表示无限期等待
      if(xQueueReceive(xKeyIntQueueHandle, &received_val, portMAX_DELAY) == pdPASS)
      {
        // 执行实际的矩阵键盘扫描
        for( row =0; row < Rows; row++ )  // 遍历所有行
        {
          // 先将所有行置为低电平
            HAL_GPIO_WritePin(row_1_GPIO_Port, row_1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(row_2_GPIO_Port, row_2_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(row_3_GPIO_Port, row_3_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(row_4_GPIO_Port, row_4_Pin, GPIO_PIN_RESET);

          // 将当前行置为高电平
            switch (row)
            {
                case 0: HAL_GPIO_WritePin(row_1_GPIO_Port, row_1_Pin, GPIO_PIN_SET); break;
                case 1: HAL_GPIO_WritePin(row_2_GPIO_Port, row_2_Pin, GPIO_PIN_SET); break;
                case 2: HAL_GPIO_WritePin(row_3_GPIO_Port, row_3_Pin, GPIO_PIN_SET); break;
                case 3: HAL_GPIO_WritePin(row_4_GPIO_Port, row_4_Pin, GPIO_PIN_SET); break;
            }

              osDelay(2);  // 短暂延时，确保电平稳定
          
          // 检测列引脚状态
            if (HAL_GPIO_ReadPin(col_1_GPIO_Port, col_1_Pin) == GPIO_PIN_SET)
            {
                key_value = KeyMap[row][0];
              // 将按键值发送到队列，供其他任务处理
						  // 第三个参数0表示不等待，立即返回
                xQueueSend(xKeyValueQueueHandle, &key_value, 0);
                break;
            }
            else if (HAL_GPIO_ReadPin(col_2_GPIO_Port, col_2_Pin) == GPIO_PIN_SET)
            {
                key_value = KeyMap[row][1];
                xQueueSend(xKeyValueQueueHandle, &key_value, 0);
                break;
            }
            else if (HAL_GPIO_ReadPin(col_3_GPIO_Port, col_3_Pin) == GPIO_PIN_SET)
            {
                key_value = KeyMap[row][2];
                xQueueSend(xKeyValueQueueHandle, &key_value, 0);
                break;
            }
            else if (HAL_GPIO_ReadPin(col_4_GPIO_Port, col_4_Pin) == GPIO_PIN_SET)
            {
                key_value = KeyMap[row][3];
                xQueueSend(xKeyValueQueueHandle, &key_value, 0);
                break;
            }


        }

      }
    osDelay(1);
  }
  /* USER CODE END vTaskKeyScan */
}

/* USER CODE BEGIN Header_vTaskKeyProcess */
/**
* @brief Function implementing the KeyProcess thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_vTaskKeyProcess */
void vTaskKeyProcess(void *argument)
{
  /* USER CODE BEGIN vTaskKeyProcess */

  uint8_t received_key = 0;

  /* Infinite loop */
  for(;;)
  {
    // 等待从队列接收按键值
    if(xQueueReceive(xKeyValueQueueHandle, &received_key, portMAX_DELAY) == pdPASS)
    {
      // 处理按键值
      switch(received_key)
      {
        case '1':
          // 处理按键1的逻辑
          OLED_Clear();
          osDelay(5);
          OLED_ShowString(2, 1, "Hello Seviffer");
					received_key = 0;
          break;
        case '2':
          // 处理按键2的逻辑
          OLED_Clear();
					osDelay(5);
          OLED_ShowString(2, 1, "Hello Sev2");
					received_key = 0;
          break;
        case '3':
          // 处理按键3的逻辑
          OLED_Clear();
          osDelay(5);
          OLED_ShowString(2, 1, "Hello Sev3");
					received_key = 0;
          break;
        case '4':
          // 处理按键4的逻辑
          OLED_Clear();
          osDelay(5);
          OLED_ShowString(2, 1, "Hello Sev4");
					received_key = 0;
          break;
        case '5':
          // 处理按键5的逻辑
          OLED_Clear();
          osDelay(5);
          OLED_ShowString(2, 1, "Hello Sev5");
					received_key = 0;
          break;
        case '6':
          // 处理按键6的逻辑
          OLED_Clear();
          osDelay(5);
          OLED_ShowString(2, 1, "Hello Sev6");
					received_key = 0;
          break;
        case '7':
          // 处理按键7的逻辑
          OLED_Clear();
          osDelay(5);
          OLED_ShowString(2, 1, "Hello Sev7");
					received_key = 0;
          break;        
        case '8':
          // 处理按键8的逻辑
          OLED_Clear();
          osDelay(5);
          OLED_ShowString(2, 1, "Hello Sev8");
					received_key = 0;
          break;  
        case 'A':
          // 处理按键9的逻辑
          OLED_Clear();
          osDelay(5);
          OLED_ShowString(2, 1, "Hello Sev9");
					received_key = 0;
          break;

        // 其他按键情况...
      }
    }
    osDelay(10);
  }
  /* USER CODE END vTaskKeyProcess */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

