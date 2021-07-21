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
#include "DISPLAY.h"

uint8_t Display[16] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x67,0x77,0x7C,0x39,0x5E,0x79,0x71};
                        //Array para la tabla


uint8_t Displayhex(uint8_t var_temp){ //Funciona para la tabla
   uint8_t tabla = Display[var_temp];
    return tabla;
    
}