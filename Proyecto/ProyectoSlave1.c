/*
 * File:   ProyectoSlave1.c
 * Author: Brayan Castillo
 * Proyecto I2C Esclavo
 * Created on  agosto de 2021
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
uint8_t     num = 0;     //Variable para el numero del adc
uint8_t     num1 = 0;     //Variable para el numero1 del adc
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
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            PORTD = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(250);    
        }
        else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = dato;         
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
    if(RBIF == 1)                 //Makes sure that it is PORTB On-Change Interrupt
    {
        INTCONbits.RBIE = 0;

        if(RB4 == 1)                  //If ECHO is HIGH
            T1CONbits.TMR1ON = 1;                    //Start Timer
        if(RB4 == 0)                  //If ECHO is LOW
        {
            T1CONbits.TMR1ON = 0;                    //Stop Timer
            a = (TMR1L | (TMR1H<<8))/58.82;  //Calculate Distance
        }
        INTCONbits.RBIF = 0;
        INTCONbits.RBIE = 1;
 
    return;
    }
    
    
}
//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    T1CON = 0x10;
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        
    if (PORTCbits.RC0 == 1){  //se lee el pin del sensor IR, 
            PORTBbits.RB1  = 1;    //Se activa la funcion cuando detecta obstaculo
            PORTBbits.RB2  = 0;    //Se activa la funcion cuando detecta obstaculo
                          //En esta progra se utilizo un led para indicar obstaculo
        }
        else{
            PORTBbits.RB2 = 1;   //Se activa otra funcion al no detectar obstaculo
            PORTBbits.RB1 = 0;   //Se activa otra funcion al no detectar obstaculo
        }
       TMR1H = 0;                  //Sets the Initial Value of Timer
       TMR1L = 0;                  //Sets the Initial Value of Timer

        PORTBbits.RB0 = 1;               //TRIGGER HIGH
        __delay_us(10);               //10uS Delay
        PORTBbits.RB0 = 0;               //TRIGGER LOW

        __delay_ms(100); //Waiting for ECHO
        a = a + 1; //Error Correction Constant
        PORTAbits.RA3 = 1;  
        if(a>=2 && a<=400)          //Check whether the result is valid or not
        {
            if(a<12){
             PORTAbits.RA0 = 1;
             PORTAbits.RA1 = 0;
             dato = 1;
            }
            else{
             PORTAbits.RA0 = 0;
             PORTAbits.RA1 = 1;
             dato = 0;
             }}
        else
        {
            PORTAbits.RA2 = 1;
        }
        __delay_ms(400);
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
    TRISB = 0b00010000;           //RB4 as Input PIN (ECHO)
    TRISD = 0x00; // LCD Pins as Output
    TRISA = 0x00; // LCD Pins as Output
    TRISC = 0b00000001; // LCD Pins as Output
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
     //Configuracion del oscilador
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;
    
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB4 = 1;
 
  
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    IOCBbits.IOCB4 = 1;
    
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    I2C_Slave_Init(0x50);   
   
}
