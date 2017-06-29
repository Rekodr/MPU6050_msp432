
/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>



#include "msp432_Clock.h"
#include "msp432_I2C.h"
#include "msp432_Interrupt.h"
#include "mpu_setup.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "invensense.h"
#include "invensense_adv.h"

#include "mpu_Uart.h"

static volatile uint32_t aclk, mclk, smclk, hsmclk, bclk;

static void interrupTest(void)
{
    P2->OUT ^= BIT2;
}

int main(void){
    MAP_WDT_A_holdTimer();
    MAP_PCM_setPowerMode(PCM_LDO_MODE);
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
    //MAP_CS_initClockSignal(CS_SMCLK, CS_MODOSC_SELECT, CS_CLOCK_DIVIDER_8);
    //MAP_CS_initClockSignal(CS_ACLK, CS_MODOSC_SELECT, CS_CLOCK_DIVIDER_1);  Might need a second clock
    mclk = CS_getMCLK();

    P2->DIR |= BIT2;
    P2->OUT &= ~BIT2;

    hal_s hal = {};
    init_mpuUART();

    i2c_enable();
    MAP_Interrupt_enableMaster();
    setup_timer();
    mpu_setup(&hal);


    while(1){
        delay_ms(1000);
        P2->OUT ^= BIT2;
        run_self_test();
       // MAP_PCM_gotoLPM0();
    }
}



