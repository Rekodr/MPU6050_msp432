
/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>



#include "msp432_Clock.h"
#include "msp432_I2C.h"
#include "msp432_Interrupt.h"
#include "mpu_setup.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "invensense.h"
#include "invensense_adv.h"

#include "mpu_Uart.h"

static volatile uint32_t mclk;







int main(void){



    unsigned char  new_temp = 0;
    unsigned long timestamp;

    MAP_WDT_A_holdTimer();
    MAP_PCM_setPowerMode(PCM_LDO_MODE);
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
    mclk = CS_getMCLK();

    P2->DIR |= BIT2;
    P2->OUT &= ~BIT2;

    init_mpuUART();

    i2c_enable();
    MAP_Interrupt_enableMaster();
    setup_timer();

    hal_s hal = {};

    /* Initialize HAL state variables. */
        hal.sensors = ACCEL_ON | GYRO_ON;
        hal.dmp_on = 1;
        hal.report = 0;
        hal.rx.cmd = 0;  // not implemented
        hal.next_pedo_ms = 0;
        hal.next_compass_ms = 0;
        hal.next_temp_ms = 0;

    /*activate feature to activate*/
    hal.dmp_features = DMP_FEATURE_6X_LP_QUAT
                           // | DMP_FEATURE_TAP
                           // | DMP_FEATURE_ANDROID_ORIENT
                            | DMP_FEATURE_SEND_RAW_ACCEL
                            | DMP_FEATURE_SEND_CAL_GYRO
                            | DMP_FEATURE_GYRO_CAL;

    mpu_setup(&hal);

    hal.report = PRINT_GYRO;
//
//    while(1){
//        delay_ms(1000);
//        run_self_test();
//    }
    while(1){
        unsigned long sensor_timestamp;
        int new_data = 0;
        //if (rx_new)
            /* A byte has been received via USB. See handle_input for a list of
             * valid commands.
             */
           // handle_input();
            get_timestamp(&timestamp);

        /* Temperature data doesn't need to be read with every gyro sample.
         * Let's make them timer-based like the compass reads.
         */
        if (timestamp > hal.next_temp_ms) {
            hal.next_temp_ms = timestamp + TEMP_READ_MS;
            new_temp = 1;
        }

        if (hal.motion_int_mode) {
            /* Enable motion interrupt. */
            mpu_lp_motion_interrupt(500, 1, 5);
            /* Notify the MPL that contiguity was broken. */
            //inv_accel_was_turned_off();
            //inv_gyro_was_turned_off();
            //inv_compass_was_turned_off();
            //inv_quaternion_sensor_was_turned_off();
            /* Wait for the MPU interrupt. */
            while (!hal.new_gyro);
              MAP_PCM_gotoLPM0InterruptSafe();
            /* Restore the previous sensor configuration. */
            mpu_lp_motion_interrupt(0, 0, 0);
            hal.motion_int_mode = 0;
        }

        if (!hal.sensors || !hal.new_gyro) {
            MAP_PCM_gotoLPM0InterruptSafe();
            /* Put the MSP430 to sleep until a timer interrupt or data ready
             * interrupt is detected.
             */
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
                /* Push the new data to the MPL. */

                inv_build_gyro(gyro, sensor_timestamp);

               printf("gyro: %7.4f %7.4f %7.4f\n\r",
                           gyro[0]/65536.f,
                           gyro[1]/65536.f,
                           gyro[2]/65536.f);


                new_data = 1;
//                if (new_temp) {
//                    new_temp = 0;
//                    /* Temperature only used for gyro temp comp. */
//                    mpu_get_temperature(&temperature, &sensor_timestamp);
//                    inv_build_temp(temperature, sensor_timestamp);
//                }
            }
            if (sensors & INV_XYZ_ACCEL) {
//                accel[0] = (long)accel_short[0];
//                accel[1] = (long)accel_short[1];
//                accel[2] = (long)accel_short[2];
//                inv_build_accel(accel, 0, sensor_timestamp);
//                new_data = 1;
            }
            if (sensors & INV_WXYZ_QUAT) {
                //inv_build_quat(quat, 0, sensor_timestamp);
                new_data = 1;
            }
        }

//        if (new_data) {
//            if(inv_execute_on_data()) {
//                MPL_LOGE("ERROR execute on data\n");
//            }
//
//            /* This function reads bias-compensated sensor data and sensor
//             * fusion outputs from the MPL. The outputs are formatted as seen
//             * in eMPL_outputs.c. This function only needs to be called at the
//             * rate requested by the host.
//             */
//            read_from_mpl();
//        }
    }

}



