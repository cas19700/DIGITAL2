/*
 * File:   ProyectoSlave3.c
 * Author: Brayan Castillo y Juan Lux
 * Sensor Infrarojo
 * Created on 18 de agosto de 2021
 */
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
#include <stdint.h>
#include <pic16f887.h>
#include "I2C.h"
#include <xc.h>
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000
uint8_t         a;
uint8_t     z;
uint8_t     dato;
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
void __interrupt() isr(void){
   if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Leer el valor del buffer
            SSPCONbits.SSPOV = 0;       // Limpiar bandera
            SSPCONbits.WCOL = 0;        // Limpiar el bit de colision
            SSPCONbits.CKP = 1;         // Encender el SCL
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            PORTD = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(250);    
        }
        else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;                 //Lectura del SSBUF para limpiar el buffer
            BF = 0;
            SSPBUF = dato;              //Enviar dato al buffer
            SSPCONbits.CKP = 1;         //Encender el SCL
            __delay_us(250);
            while(SSPSTATbits.BF);      //Esperar
        }
       
        PIR1bits.SSPIF = 0;             //Limpiar bandera
    }
    
}
//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();                    //Llamar las configuraciones
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        
    if (PORTCbits.RC0 == 1){  //se lee el pin del sensor IR, 
            PORTBbits.RB1  = 1;    //Se activa el LED cuando no detecta obstaculo
            PORTBbits.RB2  = 0;    //Se desactiva el LED cuando no detecta obstaculo
            __delay_ms(1000);      //Esperar 1 segundo
            CCPR1L = 128;          //Mover el servo
            dato = 0;              //Escribir en dato
        }
        else{
            CCPR1L = 250;          //Mover el servo
            __delay_ms(1000);      //Esperar 1 segundo
            PORTBbits.RB2 = 1;     //Se activa el LED al detectar obstaculo
            PORTBbits.RB1 = 0;     //Se desactiva el LED al detectar obstaculo
            dato = 1;              //Escribir en dato
        }
       
        }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    //  Configuracion de puertos
    ANSEL = 0x00;
    ANSELH = 0x00;
     //  Declaramos puertos como entradas o salidas
    TRISB = 0b00000000;           
    TRISD = 0x00; 
    TRISA = 0x00; 
    TRISC = 0b00000001; 
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
     //Configuracion del oscilador a 8MHz
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;
    //Configuración del PWM
    TRISCbits.TRISC2 = 1;       //CCP1 como entrada para poder configurar
    
    PR2 = 250;                  //tmr2 en 2ms 
    CCP1CONbits.P1M = 0;        //Modo Single Output
    CCP1CONbits.CCP1M = 0b00001100; //Activamos el PWM
    CCPR1L = 0x0f;
    CCP1CONbits.DC1B = 0;
    PIR1bits.TMR2IF = 0;
    T2CONbits.T2CKPS = 0b11;    //Prescaler en 1:16
    T2CONbits.TMR2ON = 1;       //Encender el timer2
    while(!PIR1bits.TMR2IF);    //Ciclo del tmr2
    PIR1bits.TMR2IF = 0;
    TRISCbits.TRISC2 = 0;       //CCP1 como salida
    INTCONbits.PEIE = 1;        //Interrupciones perifericas
    INTCONbits.GIE = 1;         //Interrupciones globales
    I2C_Slave_Init(0x70);       //Direccion del Esclavo
   
}

