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
#include "ADCCONF.h"
void initADC(void) {
    //Configuración del ADC
    ADCON1bits.ADFM = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    ADCON0bits.ADCS = 1;
    ADCON0bits.CHS = 5;
    ADCON0bits.ADON = 1;
    //Configuracion de interrupciones
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
}
