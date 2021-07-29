/*
 *Archivo:          Lab01D2.c
 *Dispositivo:	    PIC16F887
 *Autor:            Brayan Castillo
 *Compilador:	    XC8
 *Programa:         Interrupciones y librerias
 *Hardware:         Displays, botones y LEDS
 *Creado:           15 de julio del 2021
 *Ultima modificacion:	22 de julio del 2021
*/

#include <stdint.h>
#include <pic16f887.h>
#include "ADCCALL.h"

#define _XTAL_FREQ 4000000  //Frecuencia

void ADCCALL(void) {
    
        if(ADCON0bits.GO == 0){         //Si se apaga el GO entrar al if
            if(ADCON0bits.CHS == 5){    //Cambiar de canal
                ADCON0bits.CHS = 6;
            }
            else{                       //Cambiar de canal
            ADCON0bits.CHS = 5;
            }
            __delay_us(50);             //Delay del cambio de canal
            ADCON0bits.GO = 1;          //Volver a setear el GO
        }
}
