
#include <driverlib.h>
#include "esp.h"
#include "msp432_Clock.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



#define aolKey_a "anRhdGNoaW5AYW9sLmNvbQ=="
#define aolKey_b "aGVsbG9nb29kYnllMQ=="
#define aol_ip "152.163.0.69"
#define aol_serverPort "587"


#define time_delay 250
#define t_tdelay   200
#define MSG_SIZE 80
static char rxBuffer[800];
unsigned int uartrxCnt = 0;




int search(char* buffer, char* rsp) {
    int pos_search = 0;
    int pos_text = 0;
    int len_search = strlen(rsp);
    int len_text = 800;

    for (pos_text = 0; pos_text < len_text - len_search;++pos_text)
    {
        if(buffer[pos_text] == rsp[pos_search])
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
    free(tmp);
    if(search(rxBuffer, "WIFI GOT IP") == 0)
        return 1;
    return 0 ;
}

int esp_rst(){
    clearBuffer();
    ESP_OutString("AT+RST\r\n");
    MAP_PCM_gotoLPM0InterruptSafe();
    delay_ms(2000);

    if(search(rxBuffer, "ready") == 0)
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
    delay_ms(200);
    if(search(rxBuffer, "OK") == 0)
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
    if(search(rxBuffer,"CONNECT") == 0 && search(rxBuffer,"ALREADY CONNECT") == 0)
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
    len = strlen(dataOut)+2;
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

    if(search(rxBuffer, "OK") == 0)
        return 1;

    clearBuffer();
    ESP_OutString(dataOut);
    ESP_OutString("\r\n");
    MAP_PCM_gotoLPM0InterruptSafe();
    delay_ms(50);

    if(search(rxBuffer, "SEND OK") == 0)
        return -1;

    return 0;
}



int sendEmail(char* src, char* dst, char* msg)
{
    char SRC[MSG_SIZE] = "";
    char DST[MSG_SIZE] = "";
    char s[MSG_SIZE] = "";
    char d[MSG_SIZE] = "";
    char MSG[MSG_SIZE] = "";

    sprintf(SRC,"MAIL FROM:<%s>", src);
    sprintf(DST,"RCPT TO:<%s>", dst);
    sprintf(s, "From: %s <%s>", src, src);
    sprintf(d, "To: %s <%s>", dst, dst);
    sprintf(MSG, "New orientation: %s", msg);
    esp_StartConnection(TCP, aol_ip, aol_serverPort);
    delay_ms(200);

    esp_sendString("Helo 192.168.43.144");
    delay_ms(t_tdelay);
    esp_sendString("AUTH LOGIN");
    delay_ms(t_tdelay);
    esp_sendString(aolKey_a);
    delay_ms(t_tdelay);
    esp_sendString(aolKey_b);
    delay_ms(t_tdelay);
    esp_sendString(SRC);
    delay_ms(t_tdelay);
    esp_sendString(DST);
    delay_ms(t_tdelay);
    esp_sendString("DATA");
    delay_ms(t_tdelay);
    esp_sendString(s);
    delay_ms(t_tdelay);
    esp_sendString(d);
    delay_ms(t_tdelay);
    esp_sendString("Subject: Orientation Changed.");
    delay_ms(t_tdelay);
     esp_sendString(MSG);
    delay_ms(t_tdelay);
    esp_sendString(".\r\n");
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

