
// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable bit (RB4/PGM pin has PGM function, low-voltage programming enabled)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>
#include "lcd.h"
#include "pic16f628a.h"



unsigned int temps[3], of[3];
char cnt;

void main(void) {

    CMCON = 0x07;

    VRCON = 0x00;

    TRISA = 0x00;

    TRISB = 0x1f;

    T1CON = 0x00;

    CCP1CON = 0x04;

    LCD_Ini();




    LCD_Cursor(0, 0);




    LCD_Write("Hello World!");

    while (1) {




    }

    return;
}

void interrupt isr(void) {

    if (CCP1IE && CCP1IF) {
        
        CCP1IE = 0;

        if (cnt == 0) {
            TMR1 = 0;
            CCPR1 = 0;
            CCP1CONbits.CCP1M = 0b0100;

        } else if (cnt == 1) {
            temps[0] = CCPR1;
            CCP1CONbits.CCP1M = 0b0101;

        } else if (cnt == 2) {
            temps[1] = CCPR1;
            CCP1CONbits.CCP1M = 0b0100;
            
        } else if (cnt == 3) {
            temps[2] = CCPR1;
            
        }

        cnt++;
        
        CCP1IF = 0;
        CCP1IE = 1;
        return;
    }

    if(TMR1IE && TMR1IF){
        
       if(cnt != 0) of[cnt-1]++; 
                
       TMR1IF=0;
       return;
        
    }



}

