/*
 * File:   ProyectoMaster.c
 * Author: Brayan Castillo y Juan Lux
 * I2C Master
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
#include <stdio.h>
#include <stdlib.h>
#include <pic16f887.h>
#include "I2C.h"
#include "LCD.h"
#include "UART.h"
#include <xc.h>

//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000

#define RS RC0
#define EN RC1
#define D0 RD0
#define D1 RD1
#define D2 RD2
#define D3 RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

uint8_t   Stemp = 0;     //Variable para el Sensor del basurero
uint8_t   S2 = 0;        //Variable para el Sensor de humedad
uint8_t   cont = 0;      //Variable para el Contador del ultrasonico
uint8_t   contu = 0;     //Variable para el Contador del ultrasonico Dec y Uni

//Variables para enviar a Python
char humedad[] = "00%  ";
char Sensor1[] = "000  ";
char Stemp1[]  = "000  ";


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
    return;
}
//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();                //Configuraciones
    Lcd_Init();             //Iniciar el LCD
    Lcd_Clear();
    while(1){

        I2C_Master_Start();         //Llamar la direccion 71 y leerla
        I2C_Master_Write(0x71);
        Stemp = I2C_Master_Read(0); //Guardar en una variable
        I2C_Master_Stop();
        Stemp1[0]= Stemp + 48;      //Guardar el valor y pasarlo a ASCII
        __delay_ms(200);
        
        I2C_Master_Start();         //Llamar la direccion 51 y leerla
        I2C_Master_Write(0x51);
        cont = I2C_Master_Read(0);  //Guardar la variable
        I2C_Master_Stop();
        Sensor1[0] = cont/100 + 48; //Dividir en centenas, decenas y unidades
        contu = cont%100;           //y pasarlo a ASCII
        Sensor1[1] = contu/10 + 48;
        Sensor1[2] = contu%10 + 48;
        __delay_ms(200);
        
        I2C_Master_Start();         //Llmara la direccion 61 y leerla
        I2C_Master_Write(0x61);
        S2 = I2C_Master_Read(0);
        I2C_Master_Stop();
        humedad[0] = S2/10 + 48;    //Dividir en centenas y unidades 
        humedad[1] = S2%10 + 48;    //y pasarlo a ASCII
        __delay_ms(200);
          
        
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("S1:   S2:   S3:");   //Titulos en la línea 1
        __delay_ms(100);
        if(Stemp == 1){
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String("Open ");         //Si es 1 basurero abierto
            PORTAbits.RA0 = 1;
            PORTAbits.RA1 = 0;
            
        }
        else{
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String("Close");         //Si es 0 basurero cerrado
            PORTAbits.RA1 = 1;
            PORTAbits.RA0 = 0;
           
        }
        __delay_ms(100);
        Lcd_Set_Cursor(2,7);          //Escribir los valores de humedad
        Lcd_Write_String(humedad);
        __delay_ms(100);
        Lcd_Set_Cursor(2,13);          //Escribir los valores del contador
        Lcd_Write_String(Sensor1);
        __delay_ms(100);
        
         ubicacion(44);                 //Separar los valores por comas
         ubicacion(humedad[0]);         //y enviarlos a Python
         ubicacion(humedad[1]);
         ubicacion(44);
         ubicacion(Stemp1[0]);
         ubicacion(44);
         ubicacion(Sensor1[0]);
         ubicacion(Sensor1[1]);
         ubicacion(Sensor1[2]);
         ubicacion(44);

       __delay_ms(100);
       PORTAbits.RA2 = ~PORTAbits.RA2;
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    //Configuracion de los puertos
    ANSEL = 0;
    ANSELH = 0;
    
    TRISA = 0;
    TRISB = 0;
    TRISC = 0b11000000;
    TRISD = 0;
    TRISE = 0;
    
    PORTB = 0;
    PORTD = 0;
    PORTA = 0; 
    PORTE = 0;
    PORTC = 0;
    
    //Configuracion del oscilador 8MHz
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
    PIR1bits.RCIF = 0;
    PIE1bits.RCIE = 1;

    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
}
//******************************************************************************
//Funciones
//****************************************************************************** 


