#ifndef _LCD_H
#define _LCD_H

/*------------------------------*
*  include head files           *
*-------------------------------*/
#include "sys.h"

/*------------------------------*
*  macros for others            *
*-------------------------------*/
#define   LCD_BL_EN   PCout(8)
#define   DS_CD       PCout(7)
#define   DS_RST      PCout(6)
#define   DS_CS       PBout(12)

/*------------------------------*
*  public variables             *
*-------------------------------*/
extern u8 lcd_buff[128][8];

/*------------------------------*
*  public functions prototypes  *
*-------------------------------*/
extern void LCD_init(void);
extern void LCD_Display_16x16(u8 row, u8 col, const u8 *buf);
extern void LCD_DisplayString(u8 row, u8 col, u8 *text);
extern void LCD_Display_Num(u8 row, u8 col, u8 n);
extern void LCD_Clear_16x8(u8 row, u8 col);
extern void LCD_Display_Pointer(u8 row, u8 col);
extern void LCD_Clear_Pointer(u8 row, u8 col);
extern void LCD_Clear(void);

/*------------------------------*
*  others for task *
*-------------------------------*/
extern OS_STK LCD_TASK_STK[LCD_STK_SIZE];
extern void lcd_task(void *pdata);
//extern OS_EVENT *os_sem_lcd;

#endif

