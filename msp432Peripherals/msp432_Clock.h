/*
 * delay.h
 *
 *  Created on: 24 juin 2017
 *      Author: Madara
 */

#ifndef MSP432PERIPHERALS_MSP432_CLOCK_H_
#define MSP432PERIPHERALS_MSP432_CLOCK_H_


void setup_timer(void);
int get_timestamp(unsigned long* time_ms);
void delay_ms(unsigned long ms);
void systic_handler(void);


#endif /* MSP432PERIPHERALS_MSP432_CLOCK_H_ */
