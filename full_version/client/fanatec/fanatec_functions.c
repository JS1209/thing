#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <hidapi/hidapi.h>
#include <unistd.h>
#include "../global.h"
#include "../network/client.h"
#include "../errorhandling/error_handling.h"
#include "../additional/functions.h"

void send_wheel_data() {
    uint16_t raw_angle;
    double angle_deg;
    unsigned char buf[64];
    uint16_t raw_center = wheel_base.steering_center;
    uint16_t raw_min = wheel_base.steering_min;
    uint16_t raw_max = wheel_base.steering_max;
    while (1) {
        int res = hid_read(handle, buf, sizeof(buf));
        if (res > 0) {
            raw_angle = buf[17] | (buf[18] << 8);

            if (raw_angle >= raw_center) {
                angle_deg = ((double)(raw_angle - raw_center) / (raw_max - raw_center)) * (RULE_MAX_ROTATION / 2);
            } else {
                angle_deg = ((double)(raw_angle - raw_center) / (raw_center - raw_min)) * (RULE_MAX_ROTATION / 2);
            }

            //client_receive();
            printf("\033[A");
            char message[100];
            if (buf[1] & 0x40) {
                message[0] = 'C';
                client_send(message);
                printf("\n");
                break;
            }
            snprintf(message, sizeof(message), "%lf", angle_deg);
            client_send(message);

            printf("\033[B\rSteering angle: %7.2f°  ", angle_deg);
            fflush(stdout);
        } else if (res < 0) {
            fprintf(stderr, "\nError reading from wheel\n");
        }
    }
}