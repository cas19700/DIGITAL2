/*
 *Archivo:          Lab02D2.c
 *Dispositivo:	    PIC16F887
 *Autor:            Brayan Castillo
 *Compilador:	    XC8
 *Programa:         LCD y UART
 *Hardware:         LCD y Potenciometros 
 *Creado:           21 de julio del 2021
 *Ultima modificacion:	28 de julio del 2021
*/
//******************************************************************************
//Importaciòn de librerias
//****************************************************************************** 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>
#include "ADCCONF.h"
#include "ADCCALL.h"
#include "LCD.h"
#include "UART.h"
//******************************************************************************
//Palabras de configuracion
//****************************************************************************** 
    
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO 
                                    // oscillator: I/O function on 
                                    // RA6/OSC2/CLKOUT pin, I/O function 
                                    // on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF           // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON           // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF          // RE3/MCLR pin function select bit 
                                    // (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF             // Code Protection bit (Program memory code 
                                    // protection is disabled)
#pragma config CPD = OFF            // Data Code Protection bit (Data memory 
                                    // code protection is disabled)
#pragma config BOREN = OFF          // Brown Out Reset Selection bits 
                                    // (BOR disabled)
#pragma config IESO = OFF           // Internal External Switchover bit 
                                    // (Internal/External Switchover mode 
                                    // is disabled)
#pragma config FCMEN = OFF          // Fail-Safe Clock Monitor Enabled bit 
                                    // (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = ON             // Low Voltage Programming Enable bit 
                                    // (RB3/PGM pin has PGM function, low 
                                    // voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V       // Brown-out Reset Selection bit (Brown-out 
                                    // Reset set to 4.0V)
#pragma config WRT = OFF            // Flash Program Memory Self Write Enable 
                                    // bits (Write protection off)

//******************************************************************************
//Directivas del compilador
//******************************************************************************
#define _XTAL_FREQ 8000000  //Frecuencia

#define RS RC4
#define EN RC5
#define D0 RD0
#define D1 RD1
#define D2 RD2
#define D3 RD3
#define D4 RD4
#define D5 RB5
#define D6 RD6
#define D7 RD7


//******************************************************************************
//Variables
//******************************************************************************
uint8_t   num = 0;     //Variable para el numero del adc
uint8_t   num1 = 0;     //Variable para el numero1 del adc
float   contador = 0;     //Variable para el contador
float     v1 = 0;         //Variables de voltages
float     v2 = 0;
char f[10];               //Variables para desplegar en consola y LCD
char vol1[10];
char vol2[10];
char voltage1[30];
char voltage2[30];
char vardato;

//******************************************************************************
//Prototipos de Funciones
//******************************************************************************

void setup(void);
float valLCD(uint8_t vart);
void __interrupt() isr(void)
    { 
    
	if(PIR1bits.ADIF == 1){
        if(ADCON0bits.CHS == 5){
            num = ADRESH;     //Si se esta usando AN5 mover el valor a num
            }
        else if (ADCON0bits.CHS == 6){
            num1 = ADRESH;       //Si se esta usando AN6 mover el valor a num1
        }
        PIR1bits.ADIF = 0;      //Limpiar la bandera de interrupciòn
        }
    if(PIR1bits.RCIF == 1){
        vardato = RCREG;        //Mover el dato de la consola a la variable
        
    }  
        
    }
//******************************************************************************
//Ciclo Principal
//******************************************************************************
    
void main(void){
    setup();                //Llamar las configutaciones
    initADC();
    __delay_us(50);
    ADCON0bits.GO = 1;
    Lcd_Init();             //Iniciar el LCD
    Lcd_Clear();
    while(1){                //Siempre realizar el ciclo
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("S1:   S2:    S3:");   //Titulos en la línea 1
        ADCCALL();
        v1 = valLCD(num);             //Llamar la funcion para la conversion
        v2 = valLCD(num1);            //Llamar la funcion para la conversion
        __delay_ms(100);
        Lcd_Set_Cursor(2,1);
        sprintf(vol1, "%3.2fV",v1);      //Escribir los valores del voltaje 1
        Lcd_Write_String(vol1);
        
        __delay_ms(100);
        Lcd_Set_Cursor(2,7);          //Escribir los valores del voltaje 2
        sprintf(vol2, "%3.2fV",v2);
        Lcd_Write_String(vol2);
        __delay_ms(100);
        Lcd_Set_Cursor(2,14);          //Escribir los valores del contador
        sprintf(f, "%3.0f",contador);
        Lcd_Write_String(f);
        __delay_ms(100);
        
        sprintf(voltage1,"Voltaje 1: %3.2fV",v1);  //Mostrar valor en la consola
        sprintf(voltage2,"Voltaje 2: %3.2fV",v2);
        
        TXREG = '\f';                              // //Impresión en la terminal         
        letras(voltage1);
        TXREG = '\r';
        letras(voltage2);
        TXREG = '\r';
        
         __delay_ms(100);
        
         if(vardato == '+'){        //Aumentar o decrementar el contador
            contador++;
            vardato = 0;
            
        }
        else if(vardato == '-'){
            contador--;
            vardato = 0;
        }
         
         
        }
    }
//******************************************************************************
//Configuracion
//******************************************************************************

    void setup(void){
    //  Configuracion de puertos
    ANSEL = 0b01100000;
    ANSELH = 0x00;
    //  Declaramos puertos como entradas o salidas
    TRISA = 0x00;
    TRISC = 0b11000000;
    TRISD = 0x00;
    TRISB = 0b00;
    TRISE = 0x03;
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
    //Configuracion de interrupciones
    PIR1bits.RCIF = 0;
    PIE1bits.RCIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    }

   
//******************************************************************************
//Funciones
//******************************************************************************   
float valLCD(uint8_t var){ //Funcion para pasar de 0 y 255 a 0 y 5V
       float numt;
       numt = var * 0.0196;
       return numt;
    }
