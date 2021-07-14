/*
 *Archivo:          Lab0D2.c
 *Dispositivo:	    PIC16F887
 *Autor:            Brayan Castillo
 *Compilador:	    XC8
 *Programa:         Juego de Carreras
 *Hardware:         Displays, botones y LEDS
 *Creado:           10 de julio del 2021
 *Ultima modificacion:	14 de julio del 2021
*/
//******************************************************************************
//Importaciòn de librerias
//****************************************************************************** 

#include <xc.h>
#include <stdint.h>


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
#define _tmr0_value 61 //Prescaler debe estar en 256 para 1s 

//******************************************************************************
//Variables
//******************************************************************************
uint8_t    num = 3;     //Variable para el numero
uint8_t    num2 = 0;     //Variable para el numero
uint8_t    num3 = 0;     //Variable para el numero
uint8_t    seg = 0;     //Variable para el numero
uint8_t    band = 0;    //Variable banderas para el display
uint8_t    cent = 0;    //Variable centenas
uint8_t    dece = 0;    //Variable decenas
uint8_t    uni = 0;     //Variable unidades
uint8_t    inicio = 0;     //Variable unidades
int Display[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x67};
                        //Array para la tabla

//******************************************************************************
//Prototipos de Funciones
//******************************************************************************

void setup(void);
void rst_tmr0(void);
void __interrupt() isr(void)
    { 
    
    if(T0IF == 1 && inicio == 1){
        seg++;
        if(seg == 20){
            if(num == 0){
                inicio = 0;
                INTCONbits.T0IE = 0;       //Incrementar el Puerto A
                
            }
            else{
                num--;
            }
            seg = 0;
            rst_tmr0();
    }
        else{
            rst_tmr0();
        }
        }
      
    
	if(RBIF == 1){              //Interrupcion del Puerto B
                        
            if(RB0 == 0){
                if (num == 0){
                setup();
                num = 3;
                }
                else{
                inicio = 1;
                INTCONbits.T0IE = 1;       //Incrementar el Puerto A
                rst_tmr0();
                }}
            else if(RB1 == 0 && num == 0){
                if(num2 == 0){
                    num2 = 1;
                }
                else{
                num2 = num2*2;
                PORTC = num2;               //Decrementar el Puerto A
            }
            }    
            else if(RB2 == 0 && num == 0){
                if(num3 == 0){
                    num3 = 1;
                }
                else{
                num3 = num3*2;
                PORTD = num3;        //Decrementar el Puerto A
            }
            }
            else {
            }
        INTCONbits.RBIF = 0;    //Limpiar al bandera 
      }
     }
    
//******************************************************************************
//Ciclo Principal
//******************************************************************************
    
    void main(void){
        setup();                //Llamar las configutaciones
        while(1)                //Siempre realizar el ciclo
        {PORTA = Display[num];           //La variable numero siempre igual al puerto A
        if(num2 == 128 || num3 == 128){
            inicio = 0;
            num = 3;
        }
        else{
            PORTC = num2;
            PORTD = num3;
        }
        if(num == 3){
            PORTEbits.RE0 = 1;
        }
        else if(num == 2){
            PORTEbits.RE1 = 1;
            PORTEbits.RE0 = 0;
        }
        else if(num == 1){
            PORTEbits.RE2 = 1;
            PORTEbits.RE1 = 0;
        }
        else if(num == 0){
            PORTEbits.RE2 = 1;
            PORTEbits.RE1 = 1;
            PORTEbits.RE0 = 1;
        }
        //unidades();             //Llamar la funcion para separar unidades
        }
    }
    
    
//******************************************************************************
//Configuracion
//******************************************************************************

    void setup(void){
    //  Configuracion de puertos
    ANSEL = 0x00;
    ANSELH = 0x00;
    //  Declaramos puertos como entradas o salidas
    TRISA = 0x00;
    TRISC = 0x00;
    TRISD = 0x00;
    TRISE = 0x00;
    TRISB = 0b00000111;
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
    //Configuracion del Timer0
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.T0SE = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;
    rst_tmr0();
    //Pullups
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB0 = 1;
    WPUBbits.WPUB1 = 1;
    WPUBbits.WPUB2 = 1;
    //Configuracion de interrupciones
    INTCONbits.T0IE = 0;
    INTCONbits.T0IF = 0;
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    //PUERTOB
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;
    IOCBbits.IOCB2 = 1;
    INTCONbits.GIE = 1;
    
    }
    void rst_tmr0(void){
    TMR0 = _tmr0_value;
    INTCONbits.T0IF = 0;
    }
   
//******************************************************************************
//Funciones
//******************************************************************************   
    
