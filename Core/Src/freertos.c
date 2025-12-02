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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

	uint8_t SetLoop = 1;
	uint8_t LED_mode = 0;
	uint8_t StartMenu1 = 1;	
	uint8_t StartMenu2 = 0;
	uint8_t StartMenu3 = 0;	
  uint8_t SelectMenu2 = 0;
  uint8_t SelectMenu3 = 0;	
	uint8_t Menu1_InitHandle = 1;
	uint8_t Menu2_InitHandle = 1;

  
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

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void vTaskMenu1(void *argument);
void vTaskMenu2(void *argument);
void vTaskMenu3(void *argument);

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
      // 菜单初始化标志检查
      // 当Menu1_InitHandle为1时，执行菜单初始化显示
      if(Menu1_InitHandle == 1)      
      {
        // 在OLED第1行第1列显示STM32型号
          OLED_ShowString(1,1,"STM32f103c8t6");  

        // 在第2行第2列显示选择标记
          OLED_ShowString(2,2,"*");  

        // 在第2行第3列显示第一个菜单项
          OLED_ShowString(2,3,"1.configurtion");

        // 在第3行第3列显示第二个菜单项
          OLED_ShowString(3,3,"2.kaomoji");

        // 在第4行第3列显示第三个菜单项
          OLED_ShowString(4,3,"3.LED showtime");  

        // 初始化完成，设置标志为0以避免重复初始化
          Menu1_InitHandle = 0;      
      }
      
      
      // 检查是否启用第一级菜单显示
      if(StartMenu1 == 1)
      {
          // 检查是否按下选择键（Key_Num为1表示向下选择键）
          if(Key_Num == 1)
          {
              // 处理选择标记向下移动的逻辑
              
              // 当选择索引为1且按下向下键时
              if(SetLoop == 1 && Key_Num == 1)
              {
                  // 清除当前行的选择标记
                  OLED_ShowString(2,2," ");  
                  // 在第二行显示选择标记
                  OLED_ShowString(3,2,"*");  
                  // 清除第三行的选择标记
                  OLED_ShowString(4,2," ");  
                  // 清除按键标志
                  Key_Num = 0;
                  // 选择索引增加，移动到下一项
                  SetLoop ++;
              }
  
              // 当选择索引为2且按下向下键时
              if(SetLoop == 2 && Key_Num == 1)
              {
                  // 清除第一行的选择标记
                  OLED_ShowString(2,2," ");  
                  // 清除第二行的选择标记
                  OLED_ShowString(3,2," ");  
                  // 在第三行显示选择标记
                  OLED_ShowString(4,2,"*");  
                  // 清除按键标志
                  Key_Num = 0;
                  // 选择索引增加，准备循环回到第一项
                  SetLoop ++;
              }
  
              // 当选择索引为3且按下向下键时，循环回到第一项
              if(SetLoop == 3 && Key_Num == 1)
              {
                  // 在第一行显示选择标记
                  OLED_ShowString(2,2,"*");  
                  // 清除第二行的选择标记
                  OLED_ShowString(3,2," ");  
                  // 清除第三行的选择标记
                  OLED_ShowString(4,2," ");  
                  // 清除按键标志
                  Key_Num = 0;
                  // 重置选择索引为1，实现循环选择
                  SetLoop = 1;
              }
          }
          
          // 检查是否按下确认键（Key_Num为2表示确认选择键）
          if(Key_Num == 2)
          {
              // 清除OLED屏幕内容，准备显示下一级菜单
              OLED_Clear();
              // 关闭第一级菜单显示标志
              StartMenu1 = 0;
              // 开启第二级菜单显示标志
              StartMenu2 = 1;
              // 保存当前选择的菜单项索引到SelectMenu2变量
              SelectMenu2 = SetLoop;
              // 重置选择索引，为下次显示第一级菜单做准备
              SetLoop = 1;
              // 清除按键标志
              Key_Num = 0;
          }
      }
      
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
    // 检查是否启用第二级菜单显示
    if(StartMenu2 == 1)
    {
      // 根据从一级菜单选择的选项(SelectMenu2)显示不同的二级菜单内容
      
      // 当选择配置菜单(configuration)且需要初始化时
      if(SelectMenu2 == 1 && Menu2_InitHandle == 1)
      {
      // 显示标题"Configuration"
        OLED_ShowString(1,1,"Configuration");
      // 显示第一个子选项
        OLED_ShowString(2,3,"1.frequency");
      // 显示第二个子选项
        OLED_ShowString(3,3,"2.peripheral");
      // 初始化完成，设置标志为0以避免重复初始化
        Menu2_InitHandle = 0;
      }
      
    // 当选择颜文字菜单(Kaomoji)且需要初始化时
      if(SelectMenu2 == 2 && Menu2_InitHandle == 1)
      {
     // 显示标题"Kaomoji"
        OLED_ShowString(1,1,"Kaomoji");
     // 显示子选项1
        OLED_ShowString(2,3,"1.cute");
     // 显示子选项2
        OLED_ShowString(3,3,"2.classic");
     // 显示子选项3
        OLED_ShowString(4,3,"3.crazy");
     // 初始化完成，设置标志为0以避免重复初始化
        Menu2_InitHandle = 0;
      }
      
      // 当选择LED显示菜单(LED showtime)且需要初始化时
      if(SelectMenu2 == 3 && Menu2_InitHandle == 1)
      {
      // 显示标题"LED showtime"
        OLED_ShowString(1,1,"LED showtime");
      // 显示子选项1
        OLED_ShowString(2,3,"1.Strobe");
      // 显示子选项2
        OLED_ShowString(3,3,"2.Waring");
      // 显示子选项3
        OLED_ShowString(4,3,"3.DiffSpeed");
      // 初始化完成，设置标志为0以避免重复初始化
        Menu2_InitHandle = 0;
      }
      
      // 处理按键操作
      if(Key_Num == 1)
      {
        // 检查是否按下向下选择键
        if(Key_Num == 1)
        {
          // 处理选择标记向下移动的逻辑
          
          // 当选择索引为1且按下向下键时
          if(SetLoop == 1 && Key_Num == 1)
          {
            // 清除当前行的选择标记
            OLED_ShowString(2,2," ");
            // 在第二行显示选择标记
            OLED_ShowString(3,2,"*");
            // 清除第三行的选择标记
            OLED_ShowString(4,2," ");
            // 清除按键标志
            Key_Num = 0;
            // 选择索引增加，移动到下一项
            SetLoop ++;
          }
          
          // 当选择索引为2且按下向下键时
          if(SetLoop == 2 && Key_Num == 1)
          {
            // 清除第一行的选择标记
            OLED_ShowString(2,2," ");
            // 清除第二行的选择标记
            OLED_ShowString(3,2," ");
            // 在第三行显示选择标记
            OLED_ShowString(4,2,"*");
            // 清除按键标志
            Key_Num = 0;
            // 选择索引增加，准备循环回到第一项
            SetLoop ++;
          }
          
          // 当选择索引为3且按下向下键时，循环回到第一项
          if(SetLoop == 3 && Key_Num == 1)
          {
            // 在第一行显示选择标记
            OLED_ShowString(2,2,"*");
            // 清除第二行的选择标记
            OLED_ShowString(3,2," ");
            // 清除第三行的选择标记
            OLED_ShowString(4,2," ");
            // 清除按键标志
            Key_Num = 0;
            // 重置选择索引为1，实现循环选择
            SetLoop = 1;
          }
        }
      }
    }
    
    // 检查是否按下确认键(Key_Num为2)
    if(Key_Num == 2)
    {
      // 清除OLED屏幕内容，准备显示下一级菜单
      OLED_Clear();
      // 关闭第二级菜单显示标志
      StartMenu2 = 0;
      // 开启第三级菜单显示标志
      StartMenu3 = 1;
      // 保存当前选择的菜单项索引到SelectMenu3变量
      SelectMenu3 = SetLoop;
      // 重置选择索引，为下次显示菜单做准备
      SetLoop = 1;
      // 清除按键标志
      Key_Num = 0;
    }
    
    // 检查是否按下返回键(Key_Num为3)
    if(Key_Num == 3)
    {
      // 清除OLED屏幕内容
      OLED_Clear();
      // 开启第一级菜单显示标志，返回到上一级菜单
      StartMenu1 = 1;
      // 关闭第二级菜单显示标志
      StartMenu2 = 0;
      // 重置选择索引
      SetLoop = 1;
      // 设置一级菜单需要重新初始化
      Menu1_InitHandle = 1;
      // 设置二级菜单需要重新初始化
      Menu2_InitHandle = 1;
      // 清除按键标志
      Key_Num = 0;
    }
    
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
    // 检查是否启用第三级菜单显示
    if(StartMenu3 == 1)
    {
      // 根据第二级菜单的选择显示相应的第三级菜单内容
      // 当第二级菜单选择为1时
      if(SelectMenu3 == 1 && SelectMenu2 == 1)      
      {
        // 显示频率相关信息
        OLED_ShowString(1,1,"Frequency");
        OLED_ShowString(2,3,"72MHz");
      }
      
      if(SelectMenu3 == 2 && SelectMenu2 == 1)      
      {
        // 显示外设相关信息
        OLED_ShowString(1,1,"Peripheral");
        OLED_ShowString(2,3,"1OLED");
        OLED_ShowString(3,3,"3LED");
        OLED_ShowString(4,3,"3Button");  
      }
  
      if(SelectMenu3 == 3 && SelectMenu2 == 1)      
      {
        // 保留位，暂无功能
      }
                    
      // 当第二级菜单选择为2时
      if(SelectMenu3 == 1 && SelectMenu2 == 2)      
      {
        // 显示表情风格1
        OLED_ShowString(1,1,"Cute");
        OLED_ShowString(3,4,"('-'-*)");
      }
      
      if(SelectMenu3 == 2 && SelectMenu2 == 2)      
      {
        // 显示表情风格2
        OLED_ShowString(1,1,"Classic");
        OLED_ShowString(3,4,"(^^*)");
      }
  
      if(SelectMenu3 == 3 && SelectMenu2 == 2)      
      {
        // 显示表情风格3
        OLED_ShowString(1,1,"Crazy");
        OLED_ShowString(3,4,">(>_<)");
      }
                  
      // 当第二级菜单选择为3时
      if(SelectMenu3 == 1 && SelectMenu2 == 3)      
      {
        // 设置LED闪烁模式1
        LED_mode = 1;
        OLED_ShowString(1,1,"Storbe");
        OLED_ShowString(3,3,"Watching...");
      }
      
      if(SelectMenu3 == 2 && SelectMenu2 == 3)      
      {
        // 设置LED警告模式
        LED_mode = 2;
        OLED_ShowString(1,1,"Waring");
        OLED_ShowString(3,3,"Watching...");  
      }
  
      if(SelectMenu3 == 3 && SelectMenu2 == 3)      
      {
        // 设置LED不同速度模式
        LED_mode = 3;
        OLED_ShowString(1,1,"DiffSpeed");
        OLED_ShowString(3,3,"Watching...");  
      }
                  
      // 处理选择键（下一个）
      if(Key_Num == 1)
      {
        // 根据循环标志设置不同的选项高亮显示
        if(SetLoop == 1 && Key_Num == 1)
        {
          // 清除上一选项的高亮，高亮显示第二个选项
          OLED_ShowString(2,2," ");  
          OLED_ShowString(3,2,"*");  
          OLED_ShowString(4,2," ");  
          Key_Num = 0;
          SetLoop ++;
        }

        if(SetLoop == 2 && Key_Num == 1)
        {
          // 清除上一选项的高亮，高亮显示第三个选项
          OLED_ShowString(2,2," ");  
          OLED_ShowString(3,2," ");  
          OLED_ShowString(4,2,"*");  
          Key_Num = 0;
          SetLoop ++;
        }

        if(SetLoop == 3 && Key_Num == 1)
        {
          // 清除上一选项的高亮，高亮显示第一个选项（循环回顶部）
          OLED_ShowString(2,2,"*");  
          OLED_ShowString(3,2," ");  
          OLED_ShowString(4,2," ");  
          Key_Num = 0;
          SetLoop = 1;
        }
      }
    }
                
    // 处理确认键操作（此处保留位，暂无功能）
    if(Key_Num == 2)
    {
    }
    
    // 处理返回键操作
    if(Key_Num == 3)
    {
      // 清除OLED显示
      OLED_Clear();
      // 重置LED模式
      LED_mode = 0;
      // 启用第二级菜单显示
      StartMenu2 = 1;                
      // 关闭第三级菜单显示
      StartMenu3 = 0;               
      // 重置循环标志
      SetLoop = 1;
      // 标记第二级菜单需要初始化
      Menu2_InitHandle = 1;           
      // 清除按键状态
      Key_Num = 0;
    }
                
    // 任务延时50ms
    osDelay(50);
  }
  /* USER CODE END vTaskMenu3 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

