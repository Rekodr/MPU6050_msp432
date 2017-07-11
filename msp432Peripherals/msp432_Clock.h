/*
 * delay.h
 *
 *  Created on: 24 juin 2017
 *      Author: Madara
 */

#ifndef MSP432PERIPHERALS_MSP432_CLOCK_H_
#define MSP432PERIPHERALS_MSP432_CLOCK_H_


/**
 *  @brief      setup the Systick timer
 *  @param[in]  none.
 */
void setup_timer(void);

/**
 *  @brief      get the time stamp since the mcu the timer was setup.
 *  @param[out] time_ms time stamp in milliseconds
 */
int get_timestamp(unsigned long* time_ms);

/**
 *  @brief      delay for x milliseconds.
 */
void delay_ms(unsigned long ms);


void systic_handler(void);


#endif /* MSP432PERIPHERALS_MSP432_CLOCK_H_ */
