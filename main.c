#include <stdint.h>
#include <stdbool.h>
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"

/**
 * PostLab
 * IE-3027
 * Editado por Brayan Castillo
 */

uint32_t ui32Period;

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);     //Configuración del oscilador
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);                                                //Se habilita el reloj para el puerto F
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);               //Se habilita los pines de F para salidas
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);                                               //Se habilita el reloj para el temporizador
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);                                            //Se configura el timer0 como temporizador períodico
    ui32Period = (SysCtlClockGet())/2;                                                         //Se realiza el cálculo del período para el temporizador (1seg)
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period-1);                                          //Se establece el período del temporizador
    IntEnable(INT_TIMER0A);                                                                    //Se habilita la interrupción por el TIMER0A
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);                                           //Se establece que exista la interrupción por Timeout
    IntMasterEnable();
    TimerEnable(TIMER0_BASE, TIMER_A);

    while(1){
}
}

void Timer0IntHandler(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);                 //Se limpia la interrupción del timer

    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3) == 0)     //Leer el puerto F
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x0E);  //Encender color Blanco
    }
    else
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);  //Apagar los LEDS
    }
}
