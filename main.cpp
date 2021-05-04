/*
 * Copyright (c) 2017-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

#if defined(TARGET_NUCLEO_G474RE)
#define CAN_1_RD PA_11
#define CAN_1_TD PA_12
#define CAN_2_RD PB_5
#define CAN_2_TD PB_6
CAN can1(CAN_1_RD, CAN_1_TD);
CAN can2(CAN_2_RD, CAN_2_TD);
#elif defined(TARGET_NRF52840_DK)
#include "TCAN4551.h"
TCAN4551 can1(p0, p1, p2, p3, p4);
TCAN4551 can2(p5, p6, p7, p8, p9);
#else
#error No target pin mapping defined
#endif


mbed::Ticker ticker;
mbed::DigitalOut led1(LED1);
//DigitalOut led2(LED2);
/** The constructor takes in RX, and TX pin respectively.
  * These pins, for this example, are defined in mbed_app.json
  */
char counter = 0;

void send()
{
    printf("send()\n");
    can1.attach(mbed::callback([](){
        // do nothing
    }), CAN::RxIrq);
    if (can1.write(CANMessage(1337, &counter, 1))) {
        printf("wloop()\n");
        counter++;
        printf("Message sent: %d\n", counter);
    }
    led1 = !led1;
}

int main()
{
    printf("main()\n");
    ticker.attach(&send, 1);
    CANMessage msg;
    while (1) {
        printf("loop()\n");
        if (can2.read(msg)) {
            printf("Message received: %d\n", msg.data[0]);
            //led2 = !led2;
        }
        ThisThread::sleep_for(200);
    }
}
