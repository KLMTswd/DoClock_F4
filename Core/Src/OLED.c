#include "stm32f4xx_hal.h"
#include "OLED_Font.h"
#include "main.h"

/*å¼•è„šé…ç½®*/

#define OLED_W_SCL(x)		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, (GPIO_PinState)(x));
#define OLED_W_SDA(x)	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, (GPIO_PinState)(x))





/*å¼•è„šåˆå§‹åŒ?*/
void OLED_I2C_Init(void)
{

	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/**
  * @brief  I2Cå¼€å§?
  * @param  æ—?
  * @retval æ—?
  */
void OLED_I2C_Start(void)
{
	OLED_W_SDA(1);
	OLED_W_SCL(1);
	OLED_W_SDA(0);
	OLED_W_SCL(0);
}

/**
  * @brief  I2Cåœæ­¢
  * @param  æ—?
  * @retval æ—?
  */
void OLED_I2C_Stop(void)
{
	OLED_W_SDA(0);
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/**
  * @brief  I2Cå‘é€ä¸€ä¸ªå­—èŠ?
  * @param  Byte è¦å‘é€çš„ä¸€ä¸ªå­—èŠ?
  * @retval æ—?
  */
void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		OLED_W_SDA(Byte & (0x80 >> i));
		OLED_W_SCL(1);
		OLED_W_SCL(0);
	}
	OLED_W_SCL(1);	//é¢å¤–çš„ä¸€ä¸ªæ—¶é’Ÿï¼Œä¸å¤„ç†åº”ç­”ä¿¡å?
	OLED_W_SCL(0);
}

/**
  * @brief  OLEDå†™å‘½ä»?
  * @param  Command è¦å†™å…¥çš„å‘½ä»¤
  * @retval æ—?
  */
void OLED_WriteCommand(uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//ä»Žæœºåœ°å€
	OLED_I2C_SendByte(0x00);		//å†™å‘½ä»?
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}

/**
  * @brief  OLEDå†™æ•°æ?
  * @param  Data è¦å†™å…¥çš„æ•°æ®
  * @retval æ—?
  */
void OLED_WriteData(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//ä»Žæœºåœ°å€
	OLED_I2C_SendByte(0x40);		//å†™æ•°æ?
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}

/**
  * @brief  OLEDè®¾ç½®å…‰æ ‡ä½ç½®
  * @param  Y ä»¥å·¦ä¸Šè§’ä¸ºåŽŸç‚¹ï¼Œå‘ä¸‹æ–¹å‘çš„åæ ‡ï¼ŒèŒƒå›´ï¼?0~7
  * @param  X ä»¥å·¦ä¸Šè§’ä¸ºåŽŸç‚¹ï¼Œå‘å³æ–¹å‘çš„åæ ‡ï¼ŒèŒƒå›´ï¼?0~127
  * @retval æ—?
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//è®¾ç½®Yä½ç½®
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//è®¾ç½®Xä½ç½®ä½?4ä½?
	OLED_WriteCommand(0x00 | (X & 0x0F));			//è®¾ç½®Xä½ç½®é«?4ä½?
}

/**
  * @brief  OLEDæ¸…å±
  * @param  æ—?
  * @retval æ—?
  */
void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}

/**
  * @brief  OLEDæ˜¾ç¤ºä¸€ä¸ªå­—ç¬?
  * @param  Line è¡Œä½ç½®ï¼ŒèŒƒå›´ï¼?1~4
  * @param  Column åˆ—ä½ç½®ï¼ŒèŒƒå›´ï¼?1~16
  * @param  Char è¦æ˜¾ç¤ºçš„ä¸€ä¸ªå­—ç¬¦ï¼ŒèŒƒå›´ï¼šASCIIå¯è§å­—ç¬¦
  * @retval æ—?
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//è®¾ç½®å…‰æ ‡ä½ç½®åœ?
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//æ˜¾ç¤ºä¸ŠåŠéƒ¨åˆ†å†…å®¹
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//è®¾ç½®å…‰æ ‡ä½ç½®åœ?
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//æ˜¾ç¤ºä¸‹åŠéƒ¨åˆ†å†…å®¹
	}
}

/**
  * @brief  OLEDæ˜¾ç¤ºå­—ç¬¦ä¸?
  * @param  Line èµ·å§‹è¡Œä½ç½®ï¼ŒèŒƒå›´ï¼?1~4
  * @param  Column èµ·å§‹åˆ—ä½ç½®ï¼ŒèŒƒå›´ï¼?1~16
  * @param  String è¦æ˜¾ç¤ºçš„å­—ç¬¦ä¸²ï¼ŒèŒƒå›´ï¼šASCIIå¯è§å­—ç¬¦
  * @retval æ—?
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}

/**
  * @brief  OLEDæ¬¡æ–¹å‡½æ•°
  * @retval è¿”å›žå€¼ç­‰äºŽXçš„Yæ¬¡æ–¹
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}


/**
  * @brief  OLEDÏÔÊ¾Ê®½øÖÆÕýÕûÊý
  * @param  Line     ÆðÊ¼ÐÐÎ»ÖÃ£¬·¶Î§£º1~4
  * @param  Column   ÆðÊ¼ÁÐÎ»ÖÃ£¬·¶Î§£º1~16
  * @param  Number   ÒªÏÔÊ¾µÄÊý×Ö£¬·¶Î§£º0~4294967295
  * @param  Length   ÒªÏÔÊ¾Êý×ÖµÄ³¤¶È£¬·¶Î§£º1~10
  * @retval ÎÞ
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;  // Ñ­»·¼ÆÊýÆ÷£¬ÓÃÓÚ¿ØÖÆÏÔÊ¾µÄÎ»Êý
    
    // Ñ­»·ÏÔÊ¾Ã¿Ò»Î»Êý×Ö£¬¹²ÏÔÊ¾LengthÎ»
    for (i = 0; i < Length; i++)                            
    {
        // ¼ÆËãµ±Ç°Î»µÄÊý×Ö£º
        // 1. Number / OLED_Pow(10, Length - i - 1) - µÃµ½µ±Ç°Î»¼°¸ü¸ßÎ»µÄÊý×Ö
        // 2. % 10 - È¡Ä£10µÃµ½µ±Ç°Î»µÄÊý×Ö
        // 3. + '0' - ×ª»»ÎªASCII×Ö·û
        OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/**
  * @brief  OLEDæ˜¾ç¤ºæ•°å­—ï¼ˆåè¿›åˆ¶ï¼Œå¸¦ç¬¦å·æ•°ï¼‰
  * @param  Line èµ·å§‹è¡Œä½ç½®ï¼ŒèŒƒå›´ï¼?1~4
  * @param  Column èµ·å§‹åˆ—ä½ç½®ï¼ŒèŒƒå›´ï¼?1~16
  * @param  Number è¦æ˜¾ç¤ºçš„æ•°å­—ï¼ŒèŒƒå›´ï¼š-2147483648~2147483647
  * @param  Length è¦æ˜¾ç¤ºæ•°å­—çš„é•¿åº¦ï¼ŒèŒƒå›´ï¼š1~10
  * @retval æ—?
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLEDæ˜¾ç¤ºæ•°å­—ï¼ˆåå…­è¿›åˆ¶ï¼Œæ­£æ•°ï¼?
  * @param  Line èµ·å§‹è¡Œä½ç½®ï¼ŒèŒƒå›´ï¼?1~4
  * @param  Column èµ·å§‹åˆ—ä½ç½®ï¼ŒèŒƒå›´ï¼?1~16
  * @param  Number è¦æ˜¾ç¤ºçš„æ•°å­—ï¼ŒèŒƒå›´ï¼š0~0xFFFFFFFF
  * @param  Length è¦æ˜¾ç¤ºæ•°å­—çš„é•¿åº¦ï¼ŒèŒƒå›´ï¼š1~8
  * @retval æ—?
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

/**
  * @brief  OLEDæ˜¾ç¤ºæ•°å­—ï¼ˆäºŒè¿›åˆ¶ï¼Œæ­£æ•°ï¼‰
  * @param  Line èµ·å§‹è¡Œä½ç½®ï¼ŒèŒƒå›´ï¼?1~4
  * @param  Column èµ·å§‹åˆ—ä½ç½®ï¼ŒèŒƒå›´ï¼?1~16
  * @param  Number è¦æ˜¾ç¤ºçš„æ•°å­—ï¼ŒèŒƒå›´ï¼š0~1111 1111 1111 1111
  * @param  Length è¦æ˜¾ç¤ºæ•°å­—çš„é•¿åº¦ï¼ŒèŒƒå›´ï¼š1~16
  * @retval æ—?
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}


/**
 * @brief OLED³õÊ¼»¯º¯Êý
 * @retval None
 */
void OLED_Init(void)
{
   
    HAL_Delay(200);             // ÑÓÊ±200ms£¬È·±£OLEDµçÔ´ÎÈ¶¨

    OLED_I2C_Init();            // ³õÊ¼»¯I2C½Ó¿Ú£¬½¨Á¢ÓëOLEDµÄÍ¨ÐÅÍ¨µÀ
    
    OLED_WriteCommand(0xAE);    // ¹Ø±ÕOLEDÏÔÊ¾£¨³õÊ¼×´Ì¬ÏÈ¹Ø±Õ£¬ÅäÖÃÍê³ÉºóÔÙ¿ªÆô£©
    
    OLED_WriteCommand(0xD5);    // ÉèÖÃÏÔÊ¾Ê±ÖÓ·ÖÆµ/Õñµ´Æ÷ÆµÂÊÃüÁî
    OLED_WriteCommand(0x80);    // Ê±ÖÓ·ÖÆµÖµÉèÖÃÎªÄ¬ÈÏÖµ£¨0x80£©£¬È·±£ÎÈ¶¨ÏÔÊ¾
    
    OLED_WriteCommand(0xA8);    // ÉèÖÃ¶àÂ·¸´ÓÃÂÊÃüÁî
    OLED_WriteCommand(0x3F);    // 1/64 duty£¨0x3F¶ÔÓ¦64ÐÐ£©£¬¿ØÖÆOLEDµÄÉ¨Ãè·½Ê½
    
    OLED_WriteCommand(0xD3);    // ÉèÖÃÏÔÊ¾Æ«ÒÆÃüÁî
    OLED_WriteCommand(0x00);    // Æ«ÒÆÖµÎª0£¬²»Æ«ÒÆÏÔÊ¾ÄÚÈÝ
    
    OLED_WriteCommand(0x40);    // ÉèÖÃÏÔÊ¾ÆðÊ¼ÐÐÃüÁî£¨Ä¬ÈÏ´ÓµÚ0ÐÐ¿ªÊ¼ÏÔÊ¾£©
    
    OLED_WriteCommand(0xA1);    // ÉèÖÃ×óÓÒ·½ÏòÃüÁî£¬0xA1ÎªÕý³£·½Ïò£¨×óµ½ÓÒ£©
    // 0xA0Îª×óÓÒ·´×ª·½Ïò
    
    OLED_WriteCommand(0xC8);    // ÉèÖÃÉÏÏÂ·½ÏòÃüÁî£¬0xC8ÎªÕý³£·½Ïò£¨ÉÏµ½ÏÂ£©
    // 0xC0ÎªÉÏÏÂ·´×ª·½Ïò

    OLED_WriteCommand(0xDA);    // ÉèÖÃCOMÒý½ÅÓ²¼þÅäÖÃÃüÁî
    OLED_WriteCommand(0x12);    // ²ÉÓÃ½»ÌæCOMÒý½ÅÅäÖÃ£¬ÌáÉýÏÔÊ¾ÖÊÁ¿
    
    OLED_WriteCommand(0x81);    // ÉèÖÃ¶Ô±È¶È¿ØÖÆÃüÁî
    OLED_WriteCommand(0xCF);    // ¶Ô±È¶ÈÖµ£¨·¶Î§0x00-0xFF£©£¬0xCFÎª¸ß¶Ô±È¶È
    
    OLED_WriteCommand(0xD9);    // ÉèÖÃÔ¤³äµçÖÜÆÚÃüÁî
    OLED_WriteCommand(0xF1);    // Ô¤³äµçÖÜÆÚÉèÖÃ£ºÏàÎ»1=15 DCLK£¬ÏàÎ»2=1 DCLK
    
    OLED_WriteCommand(0xDB);    // ÉèÖÃVCOMHÈ¡ÏûÑ¡Ôñ¼¶±ðÃüÁî
    OLED_WriteCommand(0x30);    // VCOMHÉèÖÃÎª0.83V£¬ÓÅ»¯ÏÔÊ¾Ð§¹û
    
    OLED_WriteCommand(0xA4);    // ÉèÖÃÕûÌåÏÔÊ¾¿ª¹ØÃüÁî£¬0xA4Îª½öÏÔÊ¾RAMÖÐµÄÄÚÈÝ
    
    OLED_WriteCommand(0xA6);    // ÉèÖÃÕý³£/·´×ªÏÔÊ¾ÃüÁî£¬0xA6ÎªÕý³£ÏÔÊ¾£¨ºÚµ×°××Ö£©
    // 0xA7Îª·´×ªÏÔÊ¾£¨°×µ×ºÚ×Ö£©
    
    OLED_WriteCommand(0x8D);    // ÉèÖÃ³äµç±ÃÃüÁî
    OLED_WriteCommand(0x14);    // ¿ªÆô³äµç±Ã£¨0x10Îª¹Ø±Õ£©£¬ÓÃÓÚÇý¶¯OLED
    
    OLED_WriteCommand(0xAF);    // ¿ªÆôOLEDÏÔÊ¾£¨ËùÓÐÅäÖÃÍê³Éºó¿ªÆôÏÔÊ¾£©
        
    OLED_Clear();               // ÇåÆÁ²Ù×÷£¬Çå³ýOLEDÉÏµÄËùÓÐÏÔÊ¾ÄÚÈÝ
}

/**
  * @brief  OLEDÇå³ýÖ¸¶¨ÐÐ
 
  * @param  Line ÒªÇå³ýµÄÐÐºÅ£¬·¶Î§£º1~4

  * @retval None
 
  */
void OLED_ClearLine(uint8_t Line)
{
    uint8_t Y = (Line - 1) * 2;  // ¼ÆËãÆðÊ¼Ò³µØÖ·£¨Ã¿Ò³8ÐÐ£¬Ã¿ÐÐ¶ÔÓ¦2Ò³£©
    uint8_t i;
    
    // ÉèÖÃ¹â±êµ½Ö¸¶¨ÐÐµÄÆðÊ¼Î»ÖÃ
    OLED_SetCursor(Y, 0);
    for(i = 0; i < 128; i++)
    {
        OLED_WriteData(0x00);  // Çå³ýÉÏ°ëÒ³
    }
    
    OLED_SetCursor(Y + 1, 0);
    for(i = 0; i < 128; i++)
    {
        OLED_WriteData(0x00);  // Çå³ýÏÂ°ëÒ³
    }
}

/**
  * @brief  OLEDÇå³ýÖ¸¶¨ÇøÓò

  * @param  StartLine ÆðÊ¼ÐÐºÅ£¬·¶Î§£º1~4
 
  * @param  StartColumn ÆðÊ¼ÁÐºÅ£¬·¶Î§£º1~16
 
  * @param  EndLine ½áÊøÐÐºÅ£¬·¶Î§£º1~4
 
  * @param  EndColumn ½áÊøÁÐºÅ£¬·¶Î§£º1~16
 
  * @retval None
 
  */
void OLED_ClearArea(uint8_t StartLine, uint8_t StartColumn, uint8_t EndLine, uint8_t EndColumn)
{
    uint8_t startY, endY, startX, endX;
    uint8_t y, x;
    
    // ²ÎÊýÑéÖ¤ºÍ±ß½çµ÷Õû
    if(StartLine < 1) StartLine = 1;
    if(StartColumn < 1) StartColumn = 1;
    if(EndLine > 4) EndLine = 4;
    if(EndColumn > 16) EndColumn = 16;
    
    // È·±£ÆðÊ¼×ø±êÐ¡ÓÚµÈÓÚ½áÊø×ø±ê
    if(StartLine > EndLine) return;
    if(StartColumn > EndColumn) return;
    
    // ×ª»»ÎªOLEDÄÚ²¿×ø±ê
    startY = (StartLine - 1) * 2;
    endY = (EndLine - 1) * 2 + 1;
    startX = (StartColumn - 1) * 8;
    endX = (EndColumn) * 8 - 1;
    
    // ±éÀúÖ¸¶¨ÇøÓò²¢Çå³ý
    for(y = startY; y <= endY; y++)
    {
        OLED_SetCursor(y, startX);
        for(x = startX; x <= endX; x++)
        {
            OLED_WriteData(0x00);
        }
    }
}