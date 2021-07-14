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
uint8_t    num2 = 0;     //Variable para el numero1
uint8_t    num3 = 0;     //Variable para el numero2
uint8_t    seg = 0;     //Variable para segundos
uint8_t    band = 0;    //Variable banderas para el display
uint8_t    inicio = 0;     //Variable de inicio
uint8_t    pushp = 0;     //Variable pushpresionado
uint8_t    g1 = 0;     //Variable ganador1
uint8_t    g2 = 0;     //Variable ganador2
uint8_t    disp = 0;     //Variable display
uint8_t    uno = 1;     //Variable uno
uint8_t    dos = 2;     //Variable dos
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
        seg++;                          //incrementar la variable por interrup.
        if(seg == 20){                  //al llegar a 20 se cumple un segundo
            if(num == 0){
                inicio = 0;             //Poner inicio y la interrupcion en 0
                INTCONbits.T0IE = 0;     
                
            }
            else{
                num--;                  //si no seguir decrementando
            }
            seg = 0;                    //reiniciar variables
            rst_tmr0();
    }
        else{
            rst_tmr0();
        }
        }
      
    
	if(RBIF == 1){              //Interrupcion del Puerto B         
            if(RB0 == 1){
                if ((num == 3)&&((g1 == 1)||(g2 == 1))){ //Resetear
                setup();
                num = 3;
                num2 = 0;
                num3 = 0;
                g1 = 0;
                g2 = 0;
                inicio = 0;
                disp = 0;
                }
                else if (num == 3){
                inicio = 1;
                INTCONbits.T0IE = 1;       //Iniciar el juego
                rst_tmr0();
                }}
            /*if(RB1 == 1 && RB2 == 1 && num == 0){
                if(RB1 == 0){
                    num2 = num2*2;
                    PORTC = num2;
                }
                else{
                num2 = num2*2;
                PORTC = num2;               //Decrementar el Puerto A
            }
            }*/
            if(RB1 == 1 && num == 0){       //Boton 1
                if(RB2 == 1){
                    return;
                }
                else{
                if(num2 == 0){              //La primera vez poner en 1
                    num2 = 1;
                }
                else{
                num2 = num2*2;
                PORTC = num2;               //Multiplicar por 2 y mover
            }
                }}    
            else if(RB2 == 1 && num == 0){  //Boton 2
                
                
                if(num3 == 0){
                    num3 = 1;               //Primera vez en 1
                }
                else{
                num3 = num3*2;
                PORTD = num3;               //Multiplicar por 2 y mover
            }
                }
           
        INTCONbits.RBIF = 0;    //Limpiar la bandera 
      }
     }
    
//******************************************************************************
//Ciclo Principal
//******************************************************************************
    
    void main(void){
        setup();                //Llamar las configutaciones
        while(1)                //Siempre realizar el ciclo
        {
        if(disp == 0){
        PORTA = Display[num];           //Elegir variable del 7 segmentos
        }
        else if(disp == 1){
        PORTA = Display[uno];
        }
        else if(disp == 2){
        PORTA = Display[dos];
        }
        
        if(num2 == 128 || num3 == 128){// Mostrar en el 7 segmentos quien gano
            if(num2 == 128){
                g1 = 1;
                disp = 1;
            }
            else if (num3 = 128){
                g2 = 1;
                disp = 2;
            }
            inicio = 0;
            num = 3;
        }
        else{
            PORTC = num2;
            PORTD = num3;
        }
        if(g1 == 1){                    //Encender LED del ganador
                PORTBbits.RB3 = 1;
            }
        else if (g2 == 1){
                PORTBbits.RB4 = 1;    
            }
        if(num == 3){                   //Control del semaforo
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
    
