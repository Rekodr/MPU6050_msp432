
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

static volatile uint32_t mclk;


int main(void){

   //
    init_ESP8266_UART();
    MAP_Interrupt_enableMaster();
    //esp_rst();


    esp_setMode(Station_mode);
    delay_ms(1000);
    ESP_OutString("AT+CIPMUX=1\r\n");
    esp_connectWiFi("Blb", "Madara297");
    delay_ms(1000);
  // esp_cipStart(TCP, "192.168.88.35", "80");
  //  ESP_OutString("AT+PING=\"www.google.com\"\r\n");
   ESP_OutString("AT+CIPSTART=4,\"TCP\",\"184.106.153.149\",80\r\n");
    while(1){

    }
    return 0;
}



/*
int main(void){



    unsigned char  new_temp = 0;
    unsigned long timestamp;
    unsigned long sensor_timestamp;
    int new_data = 0;


    MAP_WDT_A_holdTimer();
    MAP_PCM_setPowerMode(PCM_LDO_MODE);
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
    mclk = CS_getMCLK();
    init_mpuUART();

    i2c_enable();
    MAP_Interrupt_enableMaster();
    setup_timer();

    hal_s hal = {};

     Initialize HAL state variables.
        hal.sensors = ACCEL_ON | GYRO_ON;
        hal.dmp_on = 1;
        hal.report = 0;
        hal.rx.cmd = 0;  // not implemented
        hal.next_pedo_ms = 0;
        hal.next_compass_ms = 0;
        hal.next_temp_ms = 0;

    activate feature to activate
    hal.dmp_features =  DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_GYRO_CAL;
                        //DMP_FEATURE_6X_LP_QUAT
                        //DMP_FEATURE_TAP
                        //DMP_FEATURE_SEND_RAW_ACCEL
                        //DMP_FEATURE_SEND_CAL_GYRO

    mpu_setup(&hal);
    while(1){

        new_data = 0;
        get_timestamp(&timestamp);

         Temperature data doesn't need to be read with every gyro sample.
         * Let's make them timer-based like the compass reads.

        if (timestamp > hal.next_temp_ms) {
            hal.next_temp_ms = timestamp + TEMP_READ_MS;
            new_temp = 1;
        }

        if (hal.motion_int_mode) {
             Enable motion interrupt.
            mpu_lp_motion_interrupt(500, 3000, 5);
             Wait for the MPU interrupt.
            while (!hal.new_gyro);
              MAP_PCM_gotoLPM0InterruptSafe();
             Restore the previous sensor configuration.
            mpu_lp_motion_interrupt(0, 0, 0);
            hal.motion_int_mode = 0;
        }

        if (!hal.sensors || !hal.new_gyro) {
            MAP_PCM_gotoLPM0InterruptSafe();
           continue;
        }

        if (hal.new_gyro && hal.dmp_on) {
            short gyro[3], accel_short[3], sensors;
            unsigned char more;
            long accel[3], quat[4], temperature;
            dmp_read_fifo(gyro, accel_short, quat, &sensor_timestamp, &sensors, &more);
            if (!more)
                hal.new_gyro = 0;
            if (sensors & INV_XYZ_GYRO) {
                 Push the new data to the MPL.
                inv_build_gyro(gyro, sensor_timestamp);
                new_data = 1;
            }
            if (sensors & INV_XYZ_ACCEL) {
                new_data = 1;
            }
            if (sensors & INV_WXYZ_QUAT) {
                new_data = 1;
            }
        }

        if (new_data) {
                switch (hal.orient){
                case Portrait:
                    // do something here
                    break;
                case Landscape:
                    // do something here
                    break;
                case Reverse_Portrait:
                    // do something here
                    break;
                case Reverse_Landscape:
                    // do something here
                    break;
                default:
                    break;

                }
        }
    }

}*/



