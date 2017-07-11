#include "driverlib.h"
#include "msp432_Clock.h"

/* Slave Address for I2C Slave */
#define SLAVE_ADDRESS 0x68

typedef enum {SENDING, READING} i2cMode;

static uint8_t* TXData;
static  uint32_t TXByteCtr;
static  uint8_t RXData[100];
static  uint32_t rxferIndex = 0;
static  uint32_t txferIndex = 0;
static  uint32_t RXByteCtr;

static i2cMode Mode = SENDING;

/* I2C Master Configuration Parameter */
const eUSCI_I2C_MasterConfig i2cConfig = {
        EUSCI_B_I2C_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        3000000,                                // SMCLK = 3MHz
        EUSCI_B_I2C_SET_DATA_RATE_400KBPS,      // Desired I2C Clock of 400khz
        0,                                      // No byte counter threshold
        EUSCI_B_I2C_NO_AUTO_STOP                // No Autostop
        };

int i2c_enable(void)
{
    /* Select Port 4 for I2C - Set Pin 4, 5 to input Primary Module Function,
     */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin( GPIO_PORT_P6, GPIO_PIN4 + GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION);

    MAP_I2C_initMaster(EUSCI_B1_BASE, &i2cConfig);

    MAP_I2C_setSlaveAddress(EUSCI_B1_BASE, SLAVE_ADDRESS);

    /* Set Master in transmit mode */
    MAP_I2C_setMode(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_MODE);

    MAP_I2C_enableModule(EUSCI_B1_BASE);

    /* Enable and clear the interrupt flag */
    MAP_I2C_clearInterruptFlag(
                                 EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0
                               | EUSCI_B_I2C_RECEIVE_INTERRUPT0
                               | EUSCI_B_I2C_NAK_INTERRUPT
                               );

    NVIC->ISER[0] |= 1 << ((INT_EUSCIB1 - 16) & 31);

    return 0;
}

int i2c_disable(void)
{
    return 0;
}

int i2c_write(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const *data)
{
    /* disable to the systick timer and the interrupt on port 4
     * when communicating
     * */
    MAP_SysTick_disableModule();
    MAP_Interrupt_disableInterrupt(INT_PORT4);

    Mode = SENDING;
    txferIndex = 0;
    TXByteCtr = length;
    TXData = (uint8_t*) data;

    MAP_I2C_setSlaveAddress(EUSCI_B1_BASE, slave_addr);
    /* Set Master in transmit mode */
    MAP_I2C_setMode(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_MODE);

    MAP_I2C_clearInterruptFlag(
                               EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0
                               | EUSCI_B_I2C_RECEIVE_INTERRUPT0 | EUSCI_B_I2C_NAK_INTERRUPT
                               );

    MAP_I2C_enableInterrupt(EUSCI_B1_BASE,
                              EUSCI_B_I2C_TRANSMIT_INTERRUPT0
                            | EUSCI_B_I2C_NAK_INTERRUPT
                            | EUSCI_B_I2C_STOP_INTERRUPT);

    /*start the communication*/
    MAP_I2C_masterSendMultiByteStart(EUSCI_B1_BASE, reg_addr);

    /*go sleep*/
    MAP_PCM_gotoLPM0InterruptSafe();
    MAP_I2C_disableInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0
                             | EUSCI_B_I2C_RECEIVE_INTERRUPT0 | EUSCI_B_I2C_STOP_INTERRUPT);
    setup_timer();
    MAP_Interrupt_enableInterrupt(INT_PORT4);
    return 0;
}

int i2c_read(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char *data)
{

    /* disable to the systick timer and the interrupt on port 4
     * when communicating
     * */
    MAP_SysTick_disableModule();
    MAP_Interrupt_disableInterrupt(INT_PORT4);

    int i;
    MAP_I2C_setSlaveAddress(EUSCI_B1_BASE, slave_addr);
    Mode = READING;
    RXByteCtr = length;
    //RXData = (unsigned char*)data;

    /* Set Master in transmit mode */
    MAP_I2C_setMode(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_MODE);
    MAP_I2C_clearInterruptFlag(
                                  EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0
                                | EUSCI_B_I2C_RECEIVE_INTERRUPT0
                                | EUSCI_B_I2C_NAK_INTERRUPT
                               );

   /* Enable master transmit interrupt */
   MAP_I2C_enableInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0 + EUSCI_B_I2C_NAK_INTERRUPT + EUSCI_B_I2C_STOP_INTERRUPT);


    /* protocol:
     *  1. Send start and address of the reg to read
     *  2. switch to receive mode
     *  3. send start in receive mode
     *  5. receive byte(s)
     */
    /* Send start and the first byte of the transmit buffer -> addr to read*/
    MAP_I2C_masterSendSingleByte(EUSCI_B1_BASE, reg_addr);

    MAP_I2C_disableInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0 + EUSCI_B_I2C_STOP_INTERRUPT);
    MAP_I2C_setMode(EUSCI_B1_BASE, EUSCI_B_I2C_RECEIVE_MODE);
    rxferIndex = 0;

   /* no need to use the interrupt handler for a single byte*/
    if(length == 1){
        data[0] = MAP_I2C_masterReceiveSingleByte(EUSCI_B1_BASE);
    }
    else{
        MAP_I2C_enableInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_RECEIVE_INTERRUPT0);
        /*send a Start and then go sleep*/
        MAP_I2C_masterReceiveStart(EUSCI_B1_BASE);
        MAP_PCM_gotoLPM0InterruptSafe();

        /*copy the RX buffer into the *data param*/
        for(i = 0; i < length; i++)
            data[i] = RXData[i];
    }

    MAP_I2C_disableInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0 + EUSCI_B_I2C_RECEIVE_INTERRUPT0 + EUSCI_B_I2C_STOP_INTERRUPT);
    setup_timer();
    MAP_Interrupt_enableInterrupt(INT_PORT4);
    return 0;
}

void I2C_IRQHandler(void)
{
    uint_fast16_t status;
    MAP_Interrupt_disableMaster();

    /*determine the src of the interrupt*/
    status = MAP_I2C_getEnabledInterruptStatus(EUSCI_B1_BASE);
    MAP_I2C_clearInterruptFlag(EUSCI_B1_BASE, status);

    if (status & EUSCI_B_I2C_NAK_INTERRUPT)
    {
        MAP_I2C_masterSendStart(EUSCI_B1_BASE);
    }
    else if (status & EUSCI_B_I2C_TRANSMIT_INTERRUPT0)
    {
        switch (Mode)
        {
        case SENDING:
            /* Check the byte counter */
            if (TXByteCtr)
            {
                /* Send the next data and decrement the byte counter */
                MAP_I2C_masterSendMultiByteNext(EUSCI_B1_BASE, TXData[txferIndex++]);
                TXByteCtr--;
                MAP_Interrupt_enableSleepOnIsrExit();
            }
            else
            {
                MAP_I2C_masterSendMultiByteStop(EUSCI_B1_BASE);
                txferIndex = 0;
                MAP_Interrupt_disableSleepOnIsrExit();
            }
            break;
        case READING:
            break;
        default:
            break;
        }

    }
    else if (status & EUSCI_B_I2C_RECEIVE_INTERRUPT0)
    {
        switch (Mode)
        {
        case READING:
            if(RXByteCtr > 1)
            {
               /* prepare the Stop signal*/
                if (RXByteCtr == 2)
                {
                    MAP_I2C_masterReceiveMultiByteStop(EUSCI_B1_BASE);
                }
                /*receiv byte and go sleep on exit of the ISR*/
                RXData[rxferIndex++] = MAP_I2C_masterReceiveMultiByteNext(EUSCI_B1_BASE);
                RXByteCtr--;
                MAP_Interrupt_enableSleepOnIsrExit();
            }
            else if(RXByteCtr == 1) // getting the last byte and wake up on exit of the ISR
            {
                RXData[rxferIndex++] = MAP_I2C_masterReceiveMultiByteNext(EUSCI_B1_BASE);
                MAP_I2C_disableInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_RECEIVE_INTERRUPT0);
                MAP_I2C_setMode(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_MODE);
                rxferIndex = 0;
                MAP_Interrupt_disableSleepOnIsrExit();
            }

            break;
        default:
            MAP_Interrupt_enableSleepOnIsrExit();
            break;
        }
    }
    MAP_Interrupt_enableMaster();
}
