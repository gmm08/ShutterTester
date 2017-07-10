
#ifndef LCD_H
#define	LCD_H

#include <xc.h>   

void LCD_SendNibble(unsigned char);
void LCD_SendCmd(unsigned char);
void LCD_SendData(unsigned char);
void LCD_EN();
void LCD_Ini();
void LCD_WaitBF();
void LCD_Cursor(unsigned char , unsigned char );
void LCD_Write(unsigned char []);
void LCD_Clear();




#endif	/* LCD_H */

