/*
 * esp.c
 *
 *  Created on: 6 juil. 2017
 *      Author: Madara
 */


#include <driverlib.h>
#include "esp.h"
#include "msp432_Clock.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



#define time_delay 250

static char rxBuffer[800];
unsigned int uartrxCnt = 0;




int search(char* rsp) {
    int pos_search = 0;
    int pos_text = 0;
    int len_search = strlen(rsp);
    int len_text = 800;

    for (pos_text = 0; pos_text < len_text - len_search;++pos_text)
    {
        if(rxBuffer[pos_text] == rsp[pos_search])
        {
            ++pos_search;
            if(pos_search == len_search)
            {
                return 1;
            }
        }
        else
        {
           pos_text -=pos_search;
           pos_search = 0;
        }
    }
    // no match
   return 0;
}



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







void clearBuffer(){
    int i;
    uartrxCnt = 0;
    for(i=0; i<100; i++)
        rxBuffer[i] = '\0'; //don't really need to clean the entire buffer
}

int esp_connectWiFi(char* ssid, char* password){

    char* tmp = (char*)malloc(sizeof(char) * 100);
    clearBuffer();
    char cmd[] = "AT+CWJAP=\"";
    unsigned int len1 = strlen(cmd) + strlen(ssid) ;
    strncpy(tmp, cmd, strlen(cmd));
    strncpy(tmp+strlen(cmd), ssid, strlen(ssid) );
    strcpy(tmp + len1, "\",\"");
    strncpy(tmp+ len1 + 3, password, strlen(password));
    strcpy(tmp+len1+3+strlen(password), "\"\r\n");
    ESP_OutString(tmp);
    MAP_PCM_gotoLPM0InterruptSafe();
    delay_ms(5000);

    if(search("WIFI GOT IP") == 0)
        return 1;
    free(tmp);
    return 0 ;
}

int esp_rst(){
    clearBuffer();
    ESP_OutString("AT+RST\r\n");
    MAP_PCM_gotoLPM0InterruptSafe();
    delay_ms(2000);

    if(search("ready") == 0)
        return 1;
    return 0;
}

int esp_setMode(espMode mode)
{
    clearBuffer();

    switch (mode){
    case Station_mode:
        ESP_OutString("AT+CWMODE=1\r\n");
        break;
    case SoftAP_mode:
        ESP_OutString("AT+CWMODE=2\r\n");
        break;
    default:
        ESP_OutString("AT+CWMODE=3\r\n");
        break;
    }

    MAP_PCM_gotoLPM0InterruptSafe();
    delay_ms(500);
    if(search("OK") == 0)
            return 1;
    return 0 ;
}


int esp_StartConnection(connectionType type, char* IP, char* Port){

    int i;
    char* tmp = (char*)malloc(sizeof(char) * 100);
    for(i=0; i < 100; i++)
        tmp[i] = '\0';

    clearBuffer();
    char cmd[] = "AT+CIPSTART=";
    strncpy(tmp, cmd, strlen(cmd));
    switch(type){
    case TCP:
        strcpy(tmp + strlen(cmd), "\"TCP\",\"");
        break;
    case UDP:
        strcpy(tmp + strlen(cmd), "\"UDP\",\""); //@todo: need to work on the UDP stuff. no completed
        break;
    case SSL:
        strcpy(tmp + strlen(cmd), "\"UDP\",\"");
        break;
    default:
        break;
    }

    strncpy(tmp+ strlen(tmp), IP, strlen(IP));
    strcpy(tmp+ strlen(tmp),"\",");
    strncpy(tmp+ strlen(tmp), Port, strlen(Port));
    strcpy(tmp+ strlen(tmp),"\r\n");
    ESP_OutString(tmp);
    MAP_PCM_gotoLPM0InterruptSafe();
    delay_ms(1000);

    free(tmp);
    if(search("CONNECT") == 0 && search("ALREADY CONNECT") == 0)
        return 1;
    return 0;
}



int esp_sendString( char *dataOut)
{

    int len;
    if(dataOut == NULL)
        return -1;

    clearBuffer();

    delay_ms(time_delay);
    len = strlen(dataOut) + 1;
    ESP_OutString("AT+CIPSEND=");

    if (len > 9)
    {
        ESP_OutChar((len / 10) + 48);
        ESP_OutChar((len % 10) + 48);
    }
    else
        ESP_OutChar(len + 48);

    ESP_OutString("\r\n");
    MAP_PCM_gotoLPM0InterruptSafe();
    delay_ms(100);

    if(search("OK") == 0)
        return 1;

    clearBuffer();
    ESP_OutString(dataOut);
    ESP_OutString("\r\n");
    MAP_PCM_gotoLPM0InterruptSafe();
    delay_ms(300);

    if(search("SEND OK") == 0)
        return -1;

    return 0;
}



int sendEmail(char* src, char* dst, char* msg)
{
    char SRC[100] = "";
    char DST[100] = "";
    char s[100] = "";
    char d[100] = "";

    sprintf(SRC,"MAIL FROM:<%s>.", src);
    sprintf(DST,"RCPT TO:<%s>", dst);
    sprintf(s, "From: %s <%s>.", src, src);
    sprintf(d, "To: %s <%s>.", dst, dst);

    esp_StartConnection(TCP, "152.163.0.69", "587");
    delay_ms(250);

    esp_sendString("Helo 192.168.43.144.");
    esp_sendString("AUTH LOGIN.");
    esp_sendString("anRhdGNoaW5AYW9sLmNvbQ==.");
    esp_sendString("aGVsbG9nb29kYnllMQ==.");
    //esp_sendString("MAIL FROM:<jtatchin@aol.com>.");
    esp_sendString(SRC);
    //esp_sendString("RCPT TO:<jordantatchin@gmail.com>.");
    esp_sendString(DST);
    esp_sendString("DATA.");
    //esp_sendString("From: jtatchin@aol.com <jtatchin@aol.com>.");
    esp_sendString(s);
    //esp_sendString("To: jordantatchin@gmail.com <jordantatchin@gmail.com>.");
    esp_sendString(d);
    esp_sendString("Subject: Orientation Changed.");
    ESP_OutString("AT+CIPSEND=3\r\n");
    delay_ms(1000);
    ESP_OutString(".\r\n");
    return 0;
}









void init_ESP8266_UART()
{
    // Selecting P3.2 and P3.3 in UART mode.
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    // Configuring UART Module
    MAP_UART_initModule(EUSCI_A2_BASE, &espConfig);
    // Enable UART module
    MAP_UART_enableModule(EUSCI_A2_BASE);
    MAP_UART_clearInterruptFlag(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);
    MAP_UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_EUSCIA2);
}



void ESP_OutChar(char data)
{
  while((EUSCI_A2->IFG & EUSCI_A_IE_TXIE) == 0);
  EUSCI_A2->TXBUF = data;
}


// Output string character by character
int ESP_OutString(char *dataOut)
{
  unsigned int i, len;
  len = strlen(dataOut);

  for(i=0; i<len ; i++)
  {
    ESP_OutChar(*dataOut++);
  }
  return len;
}


void receiveHandler(void){

    uint_fast8_t status = MAP_UART_getInterruptStatus(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);
    if(status == EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG){
        rxBuffer[uartrxCnt++] = EUSCI_A2->RXBUF;
    }

}
