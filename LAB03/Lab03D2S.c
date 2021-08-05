//*****************************************************************************
/*
 * File:   main.c
 * Author: Pablo
 * Ejemplo de implementación de la comunicación SPI 
 * Código Esclavo
 * Created on 10 de febrero de 2020, 03:32 PM
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
#include "SPI.h"
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000


uint8_t   num = 0;     //Variable para el numero del adc
uint8_t   num1 = 0;     //Variable para el numero1 del adc
uint8_t   band;
uint8_t   var = 0;     //Variable para el numero1 del adc
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
void __interrupt() isr(void){
   if(SSPIF == 1){
        num = spiRead();
        spiWrite(var);
        SSPIF = 0;
    }
}
//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    setup();
    band = 5;
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        if (ADCON0bits.GO == 0) {
            ADCON0bits.GO = 1;
            if (band == 5 & ADCON0bits.CHS == 5){
                var = ADRESH;
                PORTD = ADRESH;
                //PORTB++;
                band = 6;
                ADCON0bits.CHS = 6;
            }
            else if(band == 6 & ADCON0bits.CHS == 6){
                var = ADRESH;
                PORTB = ADRESH;
                //PORTD++;
                band = 5;
                ADCON0bits.CHS = 5;
       }
            __delay_us(50);
        }
       __delay_ms(1000);
    
    }
    //return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0b01100000;
    ANSELH = 0;
    
    TRISB = 0;
    TRISD = 0;
    TRISE = 0x03;
    PORTB = 0;
    PORTD = 0;
    //Configuracion del oscilador
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;
    //Configuración del ADC
    ADCON1bits.ADFM = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    ADCON0bits.ADCS = 1;
    ADCON0bits.CHS = 5;
    ADCON0bits.ADON = 1;
    __delay_us(50);
    //Configuracion de interrupciones
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
    TRISAbits.TRISA5 = 1;       // Slave Select
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
   
}
