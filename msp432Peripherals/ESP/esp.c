/*
 * esp.c
 *
 *  Created on: 6 juil. 2017
 *      Author: Madara
 */


#include <driverlib.h>
#include "esp.h"




// --- UART Configuration Parameter ---
const eUSCI_UART_Config espConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,         // SMCLK Clock Source
        26,                                    // BRDIV = ???  19
        0,                                      // UCxBRF = ?  0x55
        0,                                      // UCxBRS = ?
        EUSCI_A_UART_NO_PARITY,                 // No Parity
        EUSCI_A_UART_LSB_FIRST,                 // MSB First
        EUSCI_A_UART_ONE_STOP_BIT,              // One stop bit
        EUSCI_A_UART_MODE,                      // UART mode
        ~EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // over sampling
};




void init_ESP8266_UART()
{
    // Selecting P3.2 and P3.3 in UART mode.
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    // Configuring UART Module
    MAP_UART_initModule(EUSCI_A2_BASE, &espConfig);

    // Enable UART module
    MAP_UART_enableModule(EUSCI_A2_BASE);
}


// Output string character by character
int ESP_OutString(char *dataOut)
{
  unsigned int i, len;

  len = strlen(dataOut);

  for(i=0; i<len ; i++)
  {
    while((EUSCI_A2->IFG & 0x02) == 0);
    EUSCI_A2->TXBUF = *dataOut++;
  }
  return len;
}

void SendEmail(void)
{
    ESP_OutString("AT+CIPSTART=\"TCP\",\"152.163.0.69\",587\r\n");
    delay_ms(1000);
    ESP_OutString("AT+CIPSEND=21\r\n");
    delay_ms(1000);
    ESP_OutString("Helo 192.168.43.144\r\n");
    delay_ms(1000);
    ESP_OutString("AT+CIPSEND=12\r\n");
    delay_ms(1000);
    ESP_OutString("AUTH LOGIN\r\n");
    delay_ms(1000);
    ESP_OutString("AT+CIPSEND=26\r\n");
    delay_ms(1000);
    ESP_OutString("anRhdGNoaW5AYW9sLmNvbQ==\r\n");
    delay_ms(1000);
    ESP_OutString("AT+CIPSEND=22\r\n");
    delay_ms(1000);
    ESP_OutString("aGVsbG9nb29kYnllMQ==\r\n");
    delay_ms(1000);
    ESP_OutString("AT+CIPSEND=30\r\n");
    delay_ms(1000);
    ESP_OutString("MAIL FROM:<jtatchin@aol.com>\r\n");
    delay_ms(1000);
    ESP_OutString("AT+CIPSEND=32\r\n");
    delay_ms(1000);
    ESP_OutString("RCPT TO:<rekodr@mail.gvsu.edu>\r\n");
    delay_ms(1000);
    ESP_OutString("AT+CIPSEND=6\r\n");
    delay_ms(1000);
    ESP_OutString("DATA\r\n");
    delay_ms(1000);
    ESP_OutString("AT+CIPSEND=43\r\n");
    delay_ms(1000);
    ESP_OutString("From: jtatchin@aol.com <jtatchin@aol.com>\r\n");
    delay_ms(1000);
    ESP_OutString("AT+CIPSEND=49\r\n");
    delay_ms(1000);
    ESP_OutString("To: rekodr@mail.gvsu.edu <rekodr@mail.gvsu.edu>\r\n");
    delay_ms(1000);
    ESP_OutString("AT+CIPSEND=39\r\n");
    delay_ms(1000);
    ESP_OutString("Subject: comment ca va, mother fucker\r\n");
    delay_ms(1000);
    ESP_OutString("AT+CIPSEND=3\r\n");
    delay_ms(1000);
    ESP_OutString(".\r\n");
    delay_ms(1000);

 //   ESP_OutString();
  //  delay_ms(1);
    return 1;
}

