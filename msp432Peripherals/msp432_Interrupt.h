/*
 * msp432Interrupt.h
 *
 *  Created on: 24 juin 2017
 *      Author: Madara
 */

#ifndef MSP432PERIPHERALS_MSP432_INTERRUPT_H_
#define MSP432PERIPHERALS_MSP432_INTERRUPT_H_

/**
 *  @brief      Register callback for a particular interrupt pin.
 *  This function will override any function already registered.
 *  If @e cb is set to NULL, this interrupt will be disabled.
 *  @param[in]  cb          Function executed for this interrupt.
 *  @param[in]  pin         Port/pin number (INT_PIN_Pxx).
 *  @param[in]  lp_exit     Low-power mode exited after this interrupt
 *                          (INT_EXIT_LPMx).
 *  @return     0 if successful.
 */
int pin_int_cb(void (*cb)(void),
                      unsigned short pin,
                      unsigned char lp_exit);

#endif /* MSP432PERIPHERALS_MSP432_INTERRUPT_H_ */
