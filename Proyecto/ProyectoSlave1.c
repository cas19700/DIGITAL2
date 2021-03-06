/*
 * File:   ProyectoSlave1.c
 * Author: Brayan Castillo y Juan Lux
 * Sensor Ultrasonico
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
uint8_t     dato = 0;
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
            SSPCONbits.SSPOV = 0;       // Limpiar banderas
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
    if(RBIF == 1)                 //Si sucede la interrupcion
    {
        INTCONbits.RBIE = 0;      

        if(RB4 == 1)                  //Si ECHO se activo
            T1CONbits.TMR1ON = 1;     //Encender el timer
        if(RB4 == 0)                  //Si ECHO no se activo
        {
            T1CONbits.TMR1ON = 0;     //Detener el timer
            a = (TMR1L | (TMR1H<<8))/58.82;  //Calculo de la distancia
        }
        INTCONbits.RBIF = 0;          //Limpiar la bandera
        INTCONbits.RBIE = 1;
 
    return;
    }
    
    
}
//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();        //Llamar configuraciones y activar bit de T1CON
    T1CON = 0x10;
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
       TMR1H = 0;                  //Reiniciar valores del timer1
       TMR1L = 0;                  

        PORTBbits.RB0 = 1;            //Enviar pulso y esperar 10uS
        __delay_us(10);               
        PORTBbits.RB0 = 0;            //Detener el pulso

        __delay_ms(100);              //Delay para esperar el ECHO
        a = a + 1;                    //Ajuste del sensor ultrasonico
        PORTAbits.RA3 = 1;  
        if(a>=2 && a<=400)          //Verificar que el valor este en el rango
        {
            if(a<15){               //Si es menor a 15 aumentar contador y
             PORTAbits.RA0 = 1;     //Encender LED RA0
             PORTAbits.RA1 = 0;
             dato=dato++;
             
            }
            else{
             PORTAbits.RA0 = 0;     //Caso contrario no aumentar contador y 
             PORTAbits.RA1 = 1;     //Encender LED RA1
             
             }}
        else
        {
            PORTAbits.RA2 = 1;      //Si no se encuentra en rango encender RA2
        }
        __delay_ms(700);
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
    TRISB = 0b00010000;           //RB4 configurado como ECHO
    TRISD = 0x00;
    TRISA = 0x00;
    TRISC = 0b00000000;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
     //Configuracion del oscilador a 8MHz
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;
    //Configuracion del Interrupt On-Change del Puerto B
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB4 = 1;
 
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    IOCBbits.IOCB4 = 1;
    
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    I2C_Slave_Init(0x50);       //Direccion del Esclavo
   
}
