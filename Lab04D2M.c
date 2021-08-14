/*
 * File:   Lab04D2M.c
 * Author: Brayan Castillo
 * Lab04 I2C Master
 * Created on 9 de agosto de 2021
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
#include <xc.h>
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000

uint8_t   Stemp = 0;     //Variable para el numero1 del adc
uint8_t   S2 = 0;     //Variable para el numero1 del adc
int16_t    Sensor = 0;     //Variable para el numero1 del adc
float     S1 = 0;     //Variable para el numero del adc
char vol1[10];
char vol2[10];
char vol3[20];

//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
float valLCD(uint8_t vart);
//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    Lcd_Init();             //Iniciar el LCD
    Lcd_Clear();
    while(1){
        I2C_Master_Start();
        I2C_Master_Write(0x80);
        I2C_Master_Write(0xF3);
        I2C_Master_Stop();
        __delay_ms(200);
       
        I2C_Master_Start();
        I2C_Master_Write(0x81);
        Sensor = I2C_Master_Read(0);
        I2C_Master_Stop();
        
        __delay_ms(200);
        
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        Stemp = I2C_Master_Read(0);
        I2C_Master_Stop();
        S1 = valLCD(Stemp);             //Llamar la funcion para la conversion
        __delay_ms(200);
        
        I2C_Master_Start();
        I2C_Master_Write(0x61);
        S2 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
          
        
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("S1:   S2:    S3:");   //Titulos en la línea 1
        
        __delay_ms(100);
        Lcd_Set_Cursor(2,1);
        sprintf(vol1, "%3.2fV",S1);      //Escribir los valores del voltaje 1
        Lcd_Write_String(vol1);
        
        __delay_ms(100);
        Lcd_Set_Cursor(2,7);          //Escribir los valores del voltaje 2
        sprintf(vol2, "%3.0i",S2);
        Lcd_Write_String(vol2);
        __delay_ms(100);
        Lcd_Set_Cursor(2,14);          //Escribir los valores del contador
        sprintf(vol3, "%5.2i",Sensor);
        Lcd_Write_String(vol3);
        __delay_ms(100);
         
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    TRISD = 0;
    PORTB = 0;
    PORTD = 0;
     //Configuracion del oscilador
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;
   
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
}
//******************************************************************************
//Funciones
//****************************************************************************** 
float valLCD(uint8_t var){ //Funcion para pasar de 0 y 255 a 0 y 5V
       float numt;
       numt = var * 0.0196;
       return numt;
    }
