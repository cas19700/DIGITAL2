/*
 * File:   ProyectoSlave2.c
 * Author: Brayan Castillo
 * Sensor de Temperatura
 * Created on agosto de 2021
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
#include <xc.h>
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000
#define DHT11_PIN PORTAbits.RA0
#define DHT11_PIN_Direction TRISAbits.TRISA0

uint8_t bandera;
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
// Main
//*****************************************************************************
void main(void) {
    setup();
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        Start_Signal();                // Send start signal to the sensor
 
        if(Check_Response()) {         // Check if there is a response from sensor (If OK start reding humidity and temperature data)
        // Read (and save) data from the DHT11 sensor and check time out errors
            if(Read_Data(&RH_byte1) || Read_Data(&RH_byte2) || Read_Data(&T_byte1) || Read_Data(&T_byte2) || Read_Data(&CheckSum)) {
            PORTBbits.RB0 = 1;
        }
        else {                                               // If there is no time out error
            if(CheckSum == ((RH_byte1 + RH_byte2 + T_byte1 + T_byte2) & 0xFF)) {
                // If there is no checksum error
                PORTBbits.RB0 = 0;
                PORTBbits.RB1 = 0;
                PORTBbits.RB2 = 0;
                dato = T_byte1;
                PORTD = dato;
            }
        // If there is a checksum error
            else {
                PORTBbits.RB1 = 1;
            }
        }
        }
    // If there is a response (from the sensor) problem
        else {
            PORTBbits.RB2 = 1;
        }
        
        if(dato >= 25){
            PORTEbits.RE0 = 1;
        }
        else{
            PORTEbits.RE0 = 0;
        }
        
        TMR1ON = 0;                        // Disable Timer1 module
        __delay_ms(1000);                        // Wait 1 second
    }
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0x00;               //Se habilita la entrada analógica del pin 0
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
}
//******************************************************************************
// Funciones
//******************************************************************************

void Start_Signal(void){
  DHT11_PIN_Direction = 0;                    // Configure connection pin as output
  DHT11_PIN = 0;                              // Connection pin output low
  __delay_ms(25);                               // Wait 25 ms
  DHT11_PIN = 1;                              // Connection pin output high
  __delay_us(25);                               // Wait 25 us
  DHT11_PIN_Direction = 1;                    // Configure connection pin as input
}

uint8_t Check_Response(void) {
  TMR1H = 0;                                  // Reset Timer1
  TMR1L = 0;
  TMR1ON = 1;                             // Enable Timer1 module
  while(!DHT11_PIN && TMR1L < 100);           // Wait until DHT11_PIN becomes high (cheking of 80µs low time response)
  if(TMR1L > 99)                              // If response time > 99µS  ==> Response error
    return 0;                                 // Return 0 (Device has a problem with response)
  else {    TMR1H = 0;                        // Reset Timer1
    TMR1L = 0;
    while(DHT11_PIN && TMR1L < 100);          // Wait until DHT11_PIN becomes low (cheking of 80µs high time response)
    if(TMR1L > 99)                            // If response time > 99µS  ==> Response error
      return 0;                               // Return 0 (Device has a problem with response)
    else
      return 1;                               // Return 1 (response OK)
  }
}

uint8_t Read_Data(uint8_t* dht_data) {
  short i;
  *dht_data = 0;
  for(i = 0; i < 8; i++){
    TMR1H = 0;                                // Reset Timer1
    TMR1L = 0;
    while(!DHT11_PIN)                         // Wait until DHT11_PIN becomes high
      if(TMR1L > 100) {                       // If low time > 100  ==>  Time out error (Normally it takes 50µs)
        return 1;
      }
    TMR1H = 0;                                // Reset Timer1
    TMR1L = 0;
    while(DHT11_PIN)                          // Wait until DHT11_PIN becomes low
      if(TMR1L > 100) {                       // If high time > 100  ==>  Time out error (Normally it takes 26-28µs for 0 and 70µs for 1)
        return 1;                             // Return 1 (timeout error)
      }
     if(TMR1L > 50)                           // If high time > 50  ==>  Sensor sent 1
       *dht_data |= (1 << (7 - i));           // Set bit (7 - i)
  }
  return 0;                                   // Return 0 (data read OK)
}

