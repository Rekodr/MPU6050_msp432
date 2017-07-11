
#include "msp432_Interrupt.h"
#include "driverlib.h"
#include "msp432_Clock.h"


static void (*interruptRoutine)(void);


void setup(){
    /* Clearing/Enabling interrupts for GPIO P4.6 */
        MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN6);

        MAP_GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN6);
        MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN6, GPIO_HIGH_TO_LOW_TRANSITION);
        MAP_GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN6);

        /*activate PORT4 interrupt*/
        NVIC->ISER[1] |= 1 << ((INT_PORT4 - 16) & 31);
}

int pin_int_cb(void (*cb)(void), unsigned short pin, unsigned char lp_exit){

    setup();
    interruptRoutine = cb;
    return 0;
}


void PORT4_IRQHandler(void)
{
    uint_fast16_t status;
    MAP_SysTick_disableModule();
    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P4, status);

    /* If P4.6 was interrupted, initiate an I2C read */
    if (status & GPIO_PIN6)
    {
        interruptRoutine() ;
    }
    setup_timer();
    MAP_Interrupt_disableSleepOnIsrExit();
}
