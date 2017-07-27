
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

#define _XTAL_FREQ 20000000  
#define BT PORTBbits.RB1
#define T1FC 1310700    //Factor de conversió T1 a 1:1 es 1310700 ns per cada overflow

volatile unsigned int temps[3], of[3];
volatile char cnt;
volatile bit ol;

void main(void) {

    unsigned long durada[2]; //En ns


    CMCON = 0x07;

    VRCON = 0x00;

    TXSTA = 0x00;

    RCSTA = 0x00;

    TRISA = 0x00;

    TRISB = 0x1f;

    T1CON = 0x00;

    CCP1CON = 0x04;

    OPTION_REG = 0xc0;

    INTCON = 0x10;
    PIE1 = 0x05;

    // PEIE = 1;
    // GIE = 1;

    LCD_Ini();




    LCD_Cursor(0, 0);




    LCD_Write("Press Start");


    while (1) {


        if (!BT) {


            CCP1CONbits.CCP1M = 0b0101;
            CCP1IF = 0;
            CCP1IE = 1;

            TMR1IF = 0;
            TMR1IE = 1;

            INTF = 0;
            INTE = 1;

            for(int i=0;i<3;i++){
                temps[i]=0;
                of[i]=0;
            }
            cnt = 0;
            ol = 0;

            LCD_Clear();
            LCD_Cursor(0, 0);
            LCD_Write("Ready");

            PEIE = 1;
            GIE = 1;


            while (cnt < 4) {
            }

            PEIE = 0;
            GIE = 0;

            CCP1IF = 0;
            CCP1IE = 0;

            TMR1ON = 0;
            TMR1IF = 0;
            TMR1IE = 0;

            INTF = 0;
            INTE = 0;


            if (ol) {

                durada[0] = T1FC * of[1] + 200 * temps[1];
                durada[1] = T1FC * (of[2] - of[0]) + 200 * (temps[2] - temps[0]);

            } else {

                durada[0] = T1FC * of[0] + 200 * temps[0];
                durada[1] = T1FC * (of[2] - of[1]) + 200 * (temps[2] - temps[1]);

            }

            LCD_Clear();

            LCD_Cursor(0, 0);
            LCD_WriteNum(durada[0]);



            LCD_Cursor(0, 1);
            LCD_WriteNum(durada[1]);



        }


    }

    return;
}

void interrupt isr(void) {

    if (CCP1IE && CCP1IF) {

        CCP1IE = 0;

        if (cnt == 0) {
            TMR1 = 0;
            CCPR1 = 0;
            TMR1ON = 1;
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

    if (TMR1IE && TMR1IF) {

        if (cnt != 0) of[cnt - 1]++;

        TMR1IF = 0;
        return;

    }

    if (INTE && INTF) {

        ol = 1;

        INTF = 0;
        return;

    }

    return;
}


