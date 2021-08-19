//*****************************************************************************
/*
 * File:   LAB05D2.c
 * Author: Brayan Castillo
 * Comunicación UART con Adafruit
 * Código Maestro
 * Created on 17 de agosto del 2021
 */
//*****************************************************************************
//*****************************************************************************
// Palabra de configuración
//*****************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <xc.h>
#include <stdint.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "UART.h"


//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000

double   num;           //Variable para el numero 1
uint8_t   cont = 0;     //Variable para el numero1 del adc
char v1[10];            //Variable para desplegar el valor
char v2[80];            //Variable para desplegar el valor

//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
void __interrupt() isr(void)
    {    
	if(PIR1bits.RCIF == 1){
                            // Si se presiona mandar a contador
           cont = RCREG;    
        }
    if(RBIF == 1){              //Interrupcion del Puerto B         
            if(RB0 == 1){
                PORTD++;        //Aumentar PORTD
                }
            if(RB1 == 1){       
                PORTD--;        //Decrementar PORTD
            }    
            INTCONbits.RBIF = 0;    //Limpiar la bandera 
                }
    return;
}
    
//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    setup();
    //*************************************************************************
    // Loop infinito
    //*************************************************************************

       while(1){
       __delay_ms(1000);
       num = PORTD;
       sprintf(v1, "%0.0f,%0.0f,%0.0f,",num,num,num);
       letras(v1);          //Valores correspondientes del v1
     
       
       PORTA = cont;        //Puerto D igual a contador

       
       }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    //  Configuracion de puertos
    ANSEL = 0b01100000;
    ANSELH = 0x00;
    //  Declaramos puertos como entradas o salidas
    TRISA = 0x00;
    TRISC = 0b11000000;
    TRISD = 0x00;
    TRISB = 0b00000011;
    TRISE = 0x00;
    //Limpiamos valores en los puertos
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    //Configuracion del oscilador
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;

     //Configuracion de TX y RX
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    BAUDCTLbits.BRG16 = 1;
    SPBRG = 207;
    SPBRGH = 0;
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    TXSTAbits.TXEN = 1;
    //Pullups
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB0 = 1;
    WPUBbits.WPUB1 = 1;
    //Acvitar interrupciones
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;
    PIR1bits.RCIF = 0;
    PIE1bits.RCIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
}

