/*
 *Archivo:          Lab01D2.c
 *Dispositivo:	    PIC16F887
 *Autor:            Brayan Castillo
 *Compilador:	    XC8
 *Programa:         Interrupciones y librerias
 *Hardware:         Displays, botones y LEDS
 *Creado:           15 de julio del 2021
 *Ultima modificacion:	22 de julio del 2021
*/
//******************************************************************************
//Importaciòn de librerias
//****************************************************************************** 

#include <xc.h>
#include <stdint.h>
#include "ADCCONF.h"
#include "DISPLAY.h"
#include "TimerCONF.h"
#include "ADCCALL.h"
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
#define _tmr0_value 247 //Prescaler debe estar en 128 para 1 ms 
#define _XTAL_FREQ 4000000  //Frecuencia


//******************************************************************************
//Variables
//******************************************************************************
uint8_t    num = 0;     //Variable para el numero
uint8_t    num1 = 0;     //Variable para el numero
uint8_t    num2 = 0;     //Variable para el numero
uint8_t    band = 0;    //Variable banderas para el display
uint8_t    d1 = 0;    //Variable centenas
uint8_t    d2 = 0;    //Variable decenas


//******************************************************************************
//Prototipos de Funciones
//******************************************************************************

void setup(void);
void rst_tmr0(void);
void unidades(void);
void __interrupt() isr(void)
    { 
    if(T0IF == 1){              //Interupcion del timer0
        rst_tmr0();
        PORTD = 0x00;           //Limpiar el PORTD
        if(band==0){            //Si band esta encendida mostrar el display 1
           band = 1;
           PORTC = Displayhex(d2);   //Mostrar las centenas
           PORTD = 0x01;
           
        }
        else if(band==1){       //Si band esta encendida mostrar el display 2
           band = 0;
           PORTC = Displayhex(d1);   //Mostrar las decenas
           PORTD = 0x02; 
        }
    }
	if(PIR1bits.ADIF == 1){
        if(ADCON0bits.CHS == 5){
            num1 = ADRESH;     //Si se esta usando AN5 mover el valor a PORTA
            }
        else{
            num = ADRESH;       //Si se esta usando AN6 mover el valor a num
        }
        PIR1bits.ADIF = 0;      //Limpiar la bandera de interrupciòn
        }
    
	if(RBIF == 1){              //Interrupcion del Puerto B         
            if(RB0 == 1){
                PORTA++;
                }
            if(RB1 == 1){       //Boton 1
                PORTA--;
            }    
            INTCONbits.RBIF = 0;    //Limpiar la bandera 
                }
           
        
    }
//******************************************************************************
//Ciclo Principal
//******************************************************************************
    
void main(void){
    setup();                //Llamar las configutaciones
    initADC();
    initTimer();
    __delay_us(50);
    ADCON0bits.GO = 1;
    while(1){                //Siempre realizar el ciclo
        ADCCALL();
        unidades();             //Llamar la funcion para separar unidades
        num2 = PORTA;
        if(num > PORTA){        //Si pasa el numero encender la alarma
            PORTEbits.RE2 = 1;
        }
        else{
            PORTEbits.RE2 = 0;
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
    TRISC = 0x00;
    TRISD = 0x00;
    TRISB = 0b00000011;
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
    OSCCONbits.IRCF0 = 0;
    OSCCONbits.SCS = 1;
    //Pullups
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB0 = 1;
    WPUBbits.WPUB1 = 1;
    //Configuracion de interrupciones
    INTCONbits.T0IE = 1;
    INTCONbits.T0IF = 0;
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    }
    //Reset del timer0
    void rst_tmr0(void){
    TMR0 = _tmr0_value;
    INTCONbits.T0IF = 0;
    }
   
//******************************************************************************
//Funciones
//******************************************************************************   
    void unidades(void){
        if(num >= 16){                     //Si numero es mayor a 100
            d2=num/16;                   //Centena igual a num dividido 100
            d1=num-(d2*16);   //num menos decena y centena
        }
        else if(num <16){     //Si nun menor a 100 y mayor a 10
            d2=0;                         //Centena igual a 0
            d1=num;                  //Decena igual a num dividido 10
        }
    }
