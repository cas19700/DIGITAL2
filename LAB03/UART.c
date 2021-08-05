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

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pic16f887.h>
#include "UART.h"
void letras(char letra[]){
        int i = 0;
        while (letra[i] != '\0'){       //Leer el array hasta encontrar un \0
        ubicacion(letra[i]);            //Llamar funcion de ubicacion del array
        i++;
        
     }
      
    }
void ubicacion(char ubic){
       while(TXIF == 0);                //Mientras TXIF este en 0
       TXREG = ubic;                    //TXREG va a ser igual a la ubicacion
                               //del array
   }

