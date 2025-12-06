/**
  ******************************************************************************
  * File Name          : menu.c
  * Description        : 菜单系统实现文件
  ******************************************************************************
  */

#include "menu.h"
#include <string.h>

/* Global variables ----------------------------------------------------------*/
uint8_t currentMenuLevel = 0;  // 当前菜单级别
uint8_t currentSelection = 0;  // 当前选中项
uint8_t menuNeedsInit[4] = {1, 1, 1, 0};  // 各级菜单初始化标志（1是需要初始化 0初始化完毕的意思）
uint8_t LED_mode = 0;          // LED工作模式
uint8_t menuStack[3] = {0, 0, 0};  // 菜单选择栈，用于记录用户的选择路径
uint8_t stackPointer = 0;         // 栈指针

// 菜单数据数组定义

MenuItem menuData[3] = 
{
  // 主菜单（第一级）
    {
      .title = "STM32f103c8t6",
      .content = {{"1.configurtion"}, {"2.kaomoji"}, {"3.LED showtime"}, {""}},
      .itemCount = 3,
      .nextLevel = 2
    },

  // 配置子菜单（第二级）
    {
      .title = "Configuration",
      .content = {{"1.frequency"}, {"2.peripheral"}, {""}, {""}},
      .itemCount = 2,
      .nextLevel = 3
    },  
  // 颜文字子菜单（第二级）
    {
      .title = "Kaomoji",
      .content = {{"1.cute"}, {"2.classic"}, {"3.crazy"}, {""}},
      .itemCount = 3,
      .nextLevel = 3
    }

};

MenuItem menuLevel3[9] = 
{ 
  // 配置-频率（第三级）
    {
        .title = "Frequency",
        .content = {{"72MHz"}, {""}, {""}, {""}},
        .itemCount = 1
    },

  // 配置-外设（第三级）
    {
        .title = "Peripheral",
        .content = {{"1OLED"}, {"3LED"}, {"3Button"}, {""}},
        .itemCount = 3
    },

  // 颜文字-可爱（第三级）
    {
        .title = "Cute",
        .content = {{""}, {""}, {"('-'*)"}, {""}},
        .itemCount = 1
    },

  // 颜文字-经典（第三级）
    {
        .title = "Classic",
        .content = {{""}, {""}, {"(^^*)"}, {""}},
        .itemCount = 1
    },

  // 颜文字-疯狂（第三级）
    {
        .title = "Crazy",
        .content = {{""}, {""}, {"( >_<)"}, {""}},
        .itemCount = 1
    },

  // LED-频闪（第三级）
    {
        .title = "Strobe",
        .content = {{""}, {""}, {"Watching..."}, {""}},
        .itemCount = 1,
        .ledMode = 1
    },

  // LED-警告（第三级）
    {
        .title = "Waring",
        .content = {{""}, {""}, {"Watching..."}, {""}},
        .itemCount = 1,
        .ledMode = 2
    },

  // LED-变速（第三级）
    {
        .title = "DiffSpeed",
        .content = {{""}, {""}, {"Watching..."}, {""}},
        .itemCount = 1,
        .ledMode = 3
    }

}; 


/**
  * @brief  通用菜单显示函数
 
  * @param  menu: 要显示的菜单指针
 
  * @param  selection: 当前选中的菜单项索引
 
  * @retval None
 
  */
void displayMenu(MenuItem *menu, uint8_t selection)
{
  // 显示标题
    OLED_ShowString(1, 1, menu->title);
    
  // 显示菜单项和选择标记
    for (uint8_t i = 0; i < menu->itemCount && i < 3; i++) 
    {
      // 在第2+i行，第2列显示选择标记或空格
        OLED_ShowString(2 + i, 2, (selection == i + 1) ? "*" : " ");

      // 在第2+i行，第3列显示菜单项内容
        OLED_ShowString(2 + i, 3, menu->content[i]);
    }
}

/**
 
  * @brief  更新选择位置的通用函数

  * @param  selection: 当前选择位置指针

  * @param  maxItems: 最大菜单项数量

  * @retval None

  */

void updateSelection(uint8_t *selection, uint8_t maxItems) 
{
  // 增加选择索引
    (*selection)++;

  // 如果超出范围，循环回到第一个选项
    if (*selection > maxItems) 
    {
        *selection = 1;
    }
    
  // 清除按键标志
    Key_Num = 0;
}

/**
  * @brief  菜单导航的通用函数
 
  * @param  key: 按键值
 
  * @retval None
 
  */
void navigateMenu(uint8_t key) 
{
    switch (key) 
    {
        case KEY_DOWN:
          // 根据当前菜单级别更新选择位置
            if (currentMenuLevel == 1) 
            {
                updateSelection(&currentSelection, 3);
            } 
            else if (currentMenuLevel == 2) 
            {
              // 第二级菜单根据前一级选择确定最大项目数
                updateSelection(&currentSelection, 
                              menuStack[stackPointer-1] == 1 ? 2 : 3);
            }
            break;
            
        case KEY_OK:
          // 保存当前选择并进入下一级菜单
            menuStack[stackPointer++] = currentSelection;
            currentMenuLevel++;
            currentSelection = 1;
            OLED_Clear();
            Key_Num = 0;
            break;
            
        case KEY_BACK:
          // 返回上一级菜单
            if (currentMenuLevel > 1) 
            {
                currentMenuLevel--;
                stackPointer--;
                currentSelection = 1;
                OLED_Clear();
                menuNeedsInit[currentMenuLevel] = 1;

              // 如果是从第三级菜单返回，重置LED模式
                if (currentMenuLevel == 2) 
                {
                    LED_mode = 0;
                }
            }

            Key_Num = 0;
            break;
    }
}

/**
  
  * @brief  获取当前活动菜单项
  
  * @retval 当前活动菜单项指针
 
  */
MenuItem* getCurrentMenu() 
{
    if (currentMenuLevel == 1) 
    {
        return &menuData[0];  // 返回主菜单
    } 
    else if (currentMenuLevel == 2) 
    {
        uint8_t prevSelection = menuStack[stackPointer-1];
        if (prevSelection == 1) 
        {
            return &menuData[1];  // 返回配置菜单
        } 
        else if (prevSelection == 2) 
        {
            return &menuData[2];  // 返回颜文字菜单
        } 
        else if (prevSelection == 3) 
        {
            // LED菜单需要特殊处理，返回修改后的颜文字菜单结构
            static MenuItem tempLEDMenu = 
            {
                .title = "LED showtime",
                .content = {{"1.Strobe"}, {"2.Waring"}, {"3.DiffSpeed"}, {""}},
                .itemCount = 3,
                .nextLevel = 3
            };

            return &tempLEDMenu;
        }
    } 
    else if (currentMenuLevel == 3) 
    {
        uint8_t level1Selection = menuStack[0];  // 一级菜单选择
        uint8_t level2Selection = menuStack[1];  // 二级菜单选择
        
        // 根据前两级的选择确定显示哪个三级菜单
        if (level1Selection == 1) 
        {
            if (level2Selection == 1) return &menuLevel3[0];  // 频率
            if (level2Selection == 2) return &menuLevel3[1];  // 外设
        } 
        else if (level1Selection == 2) 
        {
            if (level2Selection == 1) return &menuLevel3[2];  // 可爱
            if (level2Selection == 2) return &menuLevel3[3];  // 经典
            if (level2Selection == 3) return &menuLevel3[4];  // 疯狂
        } 
        else if (level1Selection == 3) 
        {
            if (level2Selection == 1) return &menuLevel3[5];  // 频闪
            if (level2Selection == 2) return &menuLevel3[6];  // 警告
            if (level2Selection == 3) return &menuLevel3[7];  // 变速
        }
    }
    return NULL;
}

/**
  
  * @brief  重置菜单系统
  
  * @retval None
  
  */
void resetMenuSystem() 
{
    currentMenuLevel = 1;
    currentSelection = 1;
    stackPointer = 0;
    LED_mode = 0;
    
    // 设置所有菜单需要重新初始化
    for (uint8_t i = 0; i < 4; i++) 
    {
        menuNeedsInit[i] = 1;
    }
    
    // 清除OLED显示
    OLED_Clear();
}


