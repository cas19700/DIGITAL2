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

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef UART_H
#define	UART_H
#include <xc.h> // include processor files - each processor file is guarded.  

void letras(char letra[]);

void ubicacion(char ubic);

#endif	/* UART_H */

