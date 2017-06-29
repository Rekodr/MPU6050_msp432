
#include <msp432_Clock.h>
#include "driverlib.h"



typedef struct {
    volatile uint32_t timestamp;
    volatile unsigned char enable_Delay;
    volatile unsigned long timer_remaining_ms;
} Clock;



static Clock counter = {
                        .timestamp = 0,
                        .enable_Delay = 0,
                        .timer_remaining_ms = 0
                        } ;


void setup_timer(void) {
    SysTick->CTRL = 0;      /*disable SysTick during setup*/
    SysTick->LOAD = 0xBBB;  /*max: 3003 => 1 ms*/
    SysTick->VAL = 0;       /*any write to current clears it*/
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk /*set Systick default clock source*/
                  |  SysTick_CTRL_TICKINT_Msk;   /*enable Systick interrupt*/

    counter.enable_Delay = 0;
    SysTick-> CTRL |= SysTick_CTRL_ENABLE_Msk;  /*enable SysTick*/
    MAP_Interrupt_enableMaster();
}

int get_timestamp(unsigned long* time_ms){

    if(!time_ms){
        return 1;
    }
    *time_ms = counter.timestamp;
    return 0;
}

/* ----Configurable Systick delay fromm 1 ms to .... ms----*/
void delay_ms(unsigned long ms){

    MAP_SysTick_disableModule();
    if(counter.enable_Delay)
        return;

   counter.timer_remaining_ms = ms;
   setup_timer();
   counter.enable_Delay = 1;
   MAP_Interrupt_enableSleepOnIsrExit();
   MAP_PCM_gotoLPM0(); /*go to slee...*/
}


void systic_handler(void){
   // MAP_Interrupt_disableMaster();
    counter.timestamp++;
    if(counter.timer_remaining_ms){
        counter.timer_remaining_ms--;
        if(counter.timer_remaining_ms == 0){
            counter.enable_Delay = 0;
            MAP_Interrupt_disableSleepOnIsrExit();
        }
    }
    if(counter.enable_Delay == 1)
        MAP_Interrupt_enableSleepOnIsrExit();
    else
        MAP_Interrupt_disableSleepOnIsrExit();

   // MAP_Interrupt_enableMaster();
}
