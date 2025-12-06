#include "main.h"
#include "gpio.h"
#include "key.h"

volatile uint8_t Key_Num;

// 按键映射表（可根据需求自定义）
const uint8_t KeyMap[4][4] = 
{
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};



uint8_t Key_GetNum(void)
{
		uint8_t temp;
		if( Key_Num )  //濡傛灉璇碖ey_Num鏈変笢瑗匡紝閭ｄ箞閫佸嚭鍘?
		{
				temp = Key_Num;
				Key_Num = 0;
			  
			  return temp;
		}

			return 0 ;
		
}

uint8_t Key_GetState(void)
{
		// if(HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin) == 0)
		// {
		// 		return 1 ;
		// }	

		// if(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin) == 0)
		// {
		// 		return 2 ;
		// }	
		
		// if(HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin) == 0)
		// {
		// 		return 3 ;
		// }					
		
		return 0;
}	


void key_Tick(void) //鐢宠瀹氭椂涓柇璋冪敤 1ms杩涘叆1娆?
{
		static uint8_t count;
		static uint8_t CurrState,PrevState;
	
		count++;
		if(count >= 20)
		{
				 PrevState = CurrState;         // 瀛樹釜妗ｃ�傛寜涓嬬瓑浜庡悇涓寜閿鏈夌殑鍊笺�傛澗鎵嬫槸0
			CurrState = Key_GetState();		
			
				if( CurrState == 0 && PrevState != 0 )
				{
						Key_Num = PrevState ;
				
				}	
		}	
}	

