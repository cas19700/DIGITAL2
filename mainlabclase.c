#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

uint8_t F = 0;
uint8_t PB1;
/**
 * Semáforo
 * IE-3027
 * Editado por Brayan Castillo
 */
int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); //Configuracion del oscilador
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);                                            //Habilitamos el puerto F
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);           //Habilitamos los puertos como salidas
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);                                      //Habilitamos los puertos como entradas
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);//Habilitamos los pullups
    while (1){

        PB1 = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);             //Leemos el boton

        if(PB1 == 0){
            if(F == 0){                                                                   //Realizar la rutina del semaforo
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x08);//Verde
                SysCtlDelay(15000000);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
                SysCtlDelay(5000000);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x08);
                SysCtlDelay(5000000);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
                SysCtlDelay(5000000);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x08);
                SysCtlDelay(5000000);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
                SysCtlDelay(5000000);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x0A);//Amarillo
                SysCtlDelay(20000000);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x02);//Rojo
                SysCtlDelay(20000000);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
            }
            F = 1;  //Bandera en 1
        }else{
            F = 0;  //Bandera en 0
        }
    }
}
