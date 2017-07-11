




#ifndef MPU_UART_H_
#define MPU_UART_H_

// --- Standard Includes ---
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


/**
 *  @brief      Initial the uart module.
 */
void init_mpuUART(void);

/**
 *  @brief      sends a single byte via uart.
 *  @param[in]  data    byte to be send.
 */
void UART_OutChar(char data);

/**
 *  @brief      Read a char from the uart bufffer *
 *  @return     read char if successful.
 */
char UART_InChar(void);


int fputc(int ch, FILE *f);
int fgetc(FILE* f);
int UART_OutString(char *dataOut);
int fputs(const char *_ptr, register FILE *_fp);

#endif
