




#ifndef MPU_UART_H_
#define MPU_UART_H_

// --- Standard Includes ---
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void init_mpuUART(void);
void UART_OutChar(char data);
char UART_InChar(void);
int fputc(int ch, FILE *f);
int fgetc(FILE* f);
int UART_OutString(char *dataOut);
int fputs(const char *_ptr, register FILE *_fp);

#endif
