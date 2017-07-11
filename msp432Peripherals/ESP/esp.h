#ifndef MSP432PERIPHERALS_ESP_ESP_H_
#define MSP432PERIPHERALS_ESP_ESP_H_



typedef enum{Station_mode, SoftAP_mode, SoftAP_Station_mode} espMode;
typedef enum{TCP, UDP, SSL} connectionType;


/**
 *  @brief      Initialize the uart on EUSCI_A2.
 *  with a baud rate of 9600. the clock is assumed to be 3MHZ
 *  @param[in]  none.
 */
void init_ESP8266_UART();

/**
 *  @brief      Send a string to the ESP8266 via uart.
 *  @param[in]  *dataOut    string to be sent.
 *  @param[out] int         length of the sent string
 */
int ESP_OutString(char *dataOut);

/**
 *  @brief      Send a character to the ESP8266 via uart.
 *  @param[in]  data    character to be sent.
 */
void ESP_OutChar(char data);

/**
 *  @brief      connect the esp8266 to a Wi-Fi access-point.
 *  @param[in]  ssid        ssid.
 *  @param[in]  password    password.
 *  @param[out] int         0: success 1: failure
 */
int esp_connectWiFi(char* ssid, char* password);

/**
 *  @brief      Set the using mode of the esp8266.
 *  @param[in]  mode    using mode: Station_mode, SoftAP_mode, SoftAP_Station_mode.
 *  @param[out] int     0: success 1: failure.
 */
int esp_setMode(espMode mode);

/**
 *  @brief      Resest the esp8266.
 *  @param[in]  none.
 *  @param[out] int     0: success 1: failure.
 */
int esp_rst();

/**
 *  @brief      start a connection with a server.
 *  @param[in]  type    connection type: TCP, UDP, SSL.
 *  @param[in]  IP      ip address.
 *  @param[in]  Port    server port.
 *  @param[out] int     0: success 1: failure.
 */
int esp_startConnection(connectionType type, char* IP, char* Port);

/**
 *  @brief      Send data to a server. see CIPSEND in esp8266 at cmd datasheet
 *              the length of the data is calculated automatically and \r and \n
 *              are added at the end of the string.
 *  @param[in]  *dataOut    data to be sent
 *  @param[out] int     0: success 1|-1: failure.
 */
int esp_sendString( char *dataOut);

int sendEmail(char* src, char* dst, char* msg);


/**
 *  @brief      search a words or string into a string.
 *  @param[in]  *buffer buffer to search into
 *  @param[in]  *str    string to find
 *  @param[out] int     0: found 1: not found.
 */
int search(char* buffer, char* str);

#endif /* MSP432PERIPHERALS_ESP_ESP_H_ */
