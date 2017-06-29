


#include "mpu_Uart.h"
#include "driverlib.h"



// --- UART Configuration Parameter ---
const eUSCI_UART_Config uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,         // SMCLK Clock Source
        19,                                    // BRDIV = ???  19
        8,                                      // UCxBRF = ?  0x55
        0,                                      // UCxBRS = ?
        EUSCI_A_UART_NO_PARITY,                 // No Parity
        EUSCI_A_UART_LSB_FIRST,                 // MSB First
        EUSCI_A_UART_ONE_STOP_BIT,              // One stop bit
        EUSCI_A_UART_MODE,                      // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // over sampling
};





void init_mpuUART()
{
    // Selecting P1.2 and P1.3 in UART mode.
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2
                                                   | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);  // Configuring UART Module
    MAP_UART_enableModule(EUSCI_A0_BASE);                // Enable UART module
}



// Wait for last byte to be sent, then output character by placing in transmit buffer
void UART_OutChar(char data)
{
  while((EUSCI_A0->IFG & EUSCI_A_IE_TXIE) == 0);
  EUSCI_A0->TXBUF = data;
}

char UART_InChar(void)
{
    while((EUSCI_A0->IFG & EUSCI_A_IE_RXIE) == 0);
    EUSCI_A0->IFG &= ~EUSCI_A_IE_RXIE;
    return EUSCI_A0->RXBUF;
}

// For using printf()
int fputc(int ch, FILE *f)
{
    // You need to complete
    char tmp = ch;
    UART_OutChar(tmp);
    return 1;
}


int fgetc(FILE* f){
    return UART_InChar();
}


// Output string character by character
int UART_OutString(char *dataOut)
{
  unsigned int i, len;

  len = strlen(dataOut);

  for(i=0; i<len ; i++)
  {
    while((EUSCI_A0->IFG & 0x02) == 0);
    EUSCI_A0->TXBUF = *dataOut++;
  }
  return len;
}

//For using printf()
int fputs(const char *_ptr, register FILE *_fp)
{
    unsigned int i, len;

      len = strlen(_ptr);

      for(i=0; i<len ; i++)
      {
        while((EUSCI_A0->IFG & 0x02) == 0);
        UART_OutChar(*_ptr++);
      }
      return len;
}


