#ifndef MSP432PERIPHERALS_ESP_ESP_H_
#define MSP432PERIPHERALS_ESP_ESP_H_



typedef enum{Station_mode, SoftAP_mode, SoftAP_Station_mode} espMode;
typedef enum{TCP, UDP, SSL} connectionType;

void init_ESP8266_UART();
int ESP_OutString(char *dataOut);
void ESP_OutChar(char data);
int sendEmail(char* src, char* dst, char* msg);
int esp_connectWiFi(char* ssid, char* password);
int esp_setMode(espMode mode);
int esp_rst();
int esp_startConnection(connectionType type, char* IP, char* Port);
int esp_sendString( char *dataOut);
#endif /* MSP432PERIPHERALS_ESP_ESP_H_ */