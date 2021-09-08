/*
 * File:   ProyectoSlave2.c
 * Author: Brayan Castillo y Juan Lux
 * Sensor de Temperatura
 * Created on 18 de agosto de 2021
 */
//*****************************************************************************
// Palabra de configuración
//*****************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
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
#define DHT11_PIN PORTAbits.RA0
#define DHT11_PIN_Direction TRISAbits.TRISA0

uint8_t bandera;
uint8_t     z;
uint8_t dato;
uint8_t T_byte1, T_byte2, RH_byte1, RH_byte2, CheckSum ;
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
void Start_Signal(void);
uint8_t Check_Response(void);
uint8_t Read_Data(uint8_t* dht_data);
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
            __delay_us(250);    
        }
        else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;               //Lectura del SSBUF para limpiar el buffer
            BF = 0;
            SSPBUF = dato;            //Enviar dato al buffer
            SSPCONbits.CKP = 1;       //Encender el SCL
            __delay_us(250);
            while(SSPSTATbits.BF);    //Esperar
        }
       
        PIR1bits.SSPIF = 0;           //Limpiar bandera
    }
    
}
//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        Start_Signal();                // Enviar señal al sensor
        if(Check_Response()) {         // Verificar respuesta
            if(Read_Data(&RH_byte1) || Read_Data(&RH_byte2) || Read_Data(&T_byte1) || Read_Data(&T_byte2) || Read_Data(&CheckSum)) {
            PORTDbits.RD0 = 1;
            PORTDbits.RD1 = 0;
            PORTDbits.RD2 = 0;
            PORTDbits.RD3 = 0;
                }
            else {                    //Si no hay error en el tiempo de espera
            if(CheckSum == ((RH_byte1 + RH_byte2 + T_byte1 + T_byte2) & 0xFF)) {
                // Si los datos son correctos
                PORTDbits.RD0 = 0;
                PORTDbits.RD1 = 1;
                PORTDbits.RD2 = 0;
                PORTDbits.RD3 = 0;
                dato = RH_byte1;
              
            }
        // Si hay error en los datos
            else {
                PORTDbits.RD0 = 0;
                PORTDbits.RD1 = 0;
                PORTDbits.RD2 = 1;
                PORTDbits.RD3 = 0;
            }
        }
        }
    // Si hay algun error con la señal
        else {
            PORTDbits.RD0 = 0;
            PORTDbits.RD1 = 0;
            PORTDbits.RD2 = 0;
            PORTDbits.RD3 = 1;
        }
        
        T1CONbits.TMR1ON = 0;                    // Apagar el Timer1
        __delay_ms(1000);                        // Esperar 1 segundo
    }
    
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    //Configuracion de los puertos
    ANSEL = 0x00;         
    ANSELH = 0;
    
    TRISB = 0;
    TRISD = 0;
    TRISE = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTD = 0;
    PORTE = 0;
    
    //Configuración del oscilador 
    OSCCONbits.IRCF2 = 1;       //Se configura el oscilador a 8Mhz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;         //Se utiliza el oscilador interno
    
    //Configuración del timer1
    T1CONbits.T1CKPS0 = 1;
    T1CONbits.T1CKPS1 = 0;
    TMR1H = 0;
    TMR1L = 0;
    
    I2C_Slave_Init(0x60);       //Direccion del Esclavo
}
//******************************************************************************
// Funciones
//******************************************************************************

void Start_Signal(void){
  DHT11_PIN_Direction = 0;                    // Configuraciones para
  DHT11_PIN = 0;                              // el envio de la señal al
  __delay_ms(25);                             // sensor
  DHT11_PIN = 1;                             
  __delay_us(25);                              
  DHT11_PIN_Direction = 1;                    // COnfigurar como entrada
}

uint8_t Check_Response(void) {
  TMR1H = 0;                                  // Resetear el Timer1
  TMR1L = 0;
  T1CONbits.TMR1ON = 1;                       
  while(!DHT11_PIN && TMR1L < 100);    // Esperar a DHT11_PIN en 1
  if(TMR1L > 99)                       // Si el tiempo de respuesta es > 99µS hay Error
    return 0;                          // Devolver un 0
  else {    
    TMR1H = 0;                                //Resetear el Timer1 
    TMR1L = 0;
    while(DHT11_PIN && TMR1L < 100);   // Esperar a DHT11_PIN en 0
    if(TMR1L > 99)                     // Si el tiempo de respuesta es > 99µS hay Error
      return 0;                        // Devolver 0 
    else
      return 1;                        // Devolver 1
  }
}

uint8_t Read_Data(uint8_t* dht_data) {
  short i;
  *dht_data = 0;
  for(i = 0; i < 8; i++){
    TMR1H = 0;                                // Resetear Timer1
    TMR1L = 0;
    while(!DHT11_PIN)                         // Esperar DHT11_PIN en 1
      if(TMR1L > 100) {                       // Si el tiempo > 100  hay error (Suele tomar 50µs)
        return 1;
      }
    TMR1H = 0;                                // Resetear Timer1
    TMR1L = 0;
    while(DHT11_PIN)                          // Esperar DHT11_PIN en 0
      if(TMR1L > 100) {                       // Si el tiempo> 100 hay error (Suele tomar 26-28µs para 0 y 70µs para 1)
        return 1;                             // Devolver 1, Error de tiempo de respuesta
      }
     if(TMR1L > 50)                           // Si tiempo > 50 el sensor envio 1
       *dht_data |= (1 << (7 - i));           // Mover el bit (7 - i)
  }
  return 0;                                   // Devolver 0, lectura de data correcta
}

