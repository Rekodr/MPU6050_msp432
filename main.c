
/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include "esp.h"


#include "msp432_Clock.h"
#include "msp432_I2C.h"
#include "msp432_Interrupt.h"
#include "mpu_setup.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "invensense.h"
#include "invensense_adv.h"

#include "mpu_Uart.h"
#include "esp.h"


#define accessPoint "Blb"
#define password "Madara297"
#define emailSrc "jtatchin@aol.com"
#define emailDst "rekodr@mail.gvsu.edu"

int main(void){

    unsigned char  new_temp = 0;
    unsigned long timestamp;
    unsigned long sensor_timestamp;
    int new_data = 0;

    MAP_WDT_A_holdTimer();

    /*Use the LDO as power source mode
    * and set the core level to CORE1
    */
    MAP_PCM_setPowerMode(PCM_LDO_MODE);
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);


    /*Using UART to debug the mpu.
    * uncomment the line below in order to do so
    */
    init_mpuUART();

   /* Initialize the UART for esp8266*/
    init_ESP8266_UART();

    /*Initialze I2C for the mpu6050*/
    i2c_enable();
    MAP_Interrupt_enableMaster();

    /*setup the SysTick timer*/
    setup_timer();

    /*set the ESP8266 in station mode: only using wi-fi*/
    esp_setMode(Station_mode);

    /*connect to a wifi access point*/
    esp_connectWiFi(accessPoint, password);

    setup_timer();

    /*Initialize HAL state variables.
    * hal_s is used to configure the MPU
    */
    hal_s hal = {};
        hal.sensors = ACCEL_ON | GYRO_ON; //set the sensors to use
        hal.dmp_on = 1;                  // turn on the DMP
        hal.report = 0;
        hal.rx.cmd = 0;                   // not implemented
        hal.next_pedo_ms = 0;
        hal.next_compass_ms = 0;          // this cannot be use with the shield
        hal.next_temp_ms = 0;

   /*list of features to activate
    *For this project we are only using the orientation feature.
    *since there is really no need to get all the features.
    */
    hal.dmp_features =  DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_GYRO_CAL;
                        //DMP_FEATURE_6X_LP_QUAT
                        //DMP_FEATURE_TAP
                        //DMP_FEATURE_SEND_RAW_ACCEL
                        //DMP_FEATURE_SEND_CAL_GYRO

    /*setup the mpu6050*/
    mpu_setup(&hal);


    while(1){
        new_data = 0;
        get_timestamp(&timestamp);
        if (hal.motion_int_mode) {
            /*Enable motion interrupt.*/
            mpu_lp_motion_interrupt(500, 3000, 5);
            /*Wait for the MPU interrupt.*/
            while (!hal.new_gyro);
              MAP_PCM_gotoLPM0InterruptSafe(); //go sleep

            mpu_lp_motion_interrupt(0, 0, 0);// on wake disable the mpu interrupt
            hal.motion_int_mode = 0;
        }

        /*check if the there is new data
        * here this is not needed since were not using the sensors
        */
        if (!hal.sensors || !hal.new_gyro) {
            MAP_PCM_gotoLPM0InterruptSafe();
          // continue;
        }

        /*check if there is new data and the DMP is ON*/
        if (hal.new_gyro && hal.dmp_on) {
            short gyro[3], accel_short[3], sensors;
            unsigned char more;
            long accel[3], quat[4], temperature;
            /*read the dmp data from the mpu*/
            dmp_read_fifo(gyro, accel_short, quat, &sensor_timestamp, &sensors, &more);
            if (!more)  //check if there is more data
                hal.new_gyro = 0;
           /* check the type of data that was read from the dmp*/
            if (sensors & INV_XYZ_GYRO) {
          /*Push the new data to the MPL.*/
                //inv_build_gyro(gyro, sensor_timestamp);
                new_data = 1;
            }
            if (sensors & INV_XYZ_ACCEL) {
                new_data = 1;
            }
            if (sensors & INV_WXYZ_QUAT) {
                new_data = 1;
            }else
                new_data = 1;
        }

      /*  sending email depending on the mpu orientation*/
        if (new_data) {
                switch (hal.orient){
                case Portrait:
                    sendEmail(emailSrc, emailDst, "Portrait");
                    break;
                case Landscape:
                    sendEmail(emailSrc, emailDst, "Landscape");
                    break;
                case Reverse_Portrait:
                    sendEmail(emailSrc, emailDst, "Reverse_Portrait");
                    break;
                case Reverse_Landscape:
                    sendEmail(emailSrc, emailDst, "Reverse_Landscape");
                    break;
                default:
                    break;
                }
        }
    }
}



