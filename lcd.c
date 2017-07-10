

#include <xc.h>
#include "lcd.h"
#include <stdlib.h>

#define _XTAL_FREQ 20000000  

#define RS PORTBbits.RB7
#define EN PORTBbits.RB6
#define RW PORTBbits.RB5

void LCD_SendNibble(unsigned char nib) {

    nib = 0x0f & nib;
    PORTA = nib;
    LCD_EN();


}

void LCD_SendCmd(unsigned char num) {

    LCD_SendNibble(num >> 4);
    LCD_WaitBF();
    LCD_SendNibble(num);
    LCD_WaitBF();

}

void LCD_SendData(unsigned char num) {

    RS = 1;
    LCD_SendNibble(num >> 4);
    LCD_WaitBF();
    RS = 1;
    LCD_SendNibble(num);
    LCD_WaitBF();

}

void LCD_EN() {

    EN = 1;
    __delay_ms(5);
    EN = 0;


}

void LCD_Ini() {

    RW = 0;
    RS = 0;

    __delay_ms(500);
    LCD_SendNibble(0x03);
    __delay_ms(10);
    LCD_SendNibble(0x03);
    __delay_ms(1);
    LCD_SendNibble(0x03);

    LCD_WaitBF();
    LCD_SendNibble(0x02);

    LCD_WaitBF();
    LCD_SendCmd(0x28);

    LCD_SendCmd(0x06);

    LCD_SendCmd(0x01);

    LCD_SendCmd(0x0c);



}

void LCD_WaitBF() {

    RW = 1;

    RS = 0;
    //   EN = 1;
    TRISA = 0x0f;

    while (!PORTAbits.RA3) {

    }

    RW = 0;
    EN = 0;
    TRISA = 0x00;

}

void LCD_Cursor(unsigned char x, unsigned char y) {

    unsigned char pos = 0x80 + x + 0x40 * y;
    LCD_SendCmd(pos);


}

void LCD_Write(unsigned char str[]){
    
    char i = 0;
    while(str[i]!=NULL){
        LCD_SendData(str[i]);
       i++; 
    }
    
}

void LCD_WriteNum(unsigned long num){
    
    unsigned char buffer[10];
    ultoa(num,buffer,10);
    LCD_Write(buffer);
    
}

void LCD_Clear(){
    
    LCD_SendCmd(0x01);
    
}