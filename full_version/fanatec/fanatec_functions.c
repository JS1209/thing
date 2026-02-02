#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <hidapi/hidapi.h>
#include <unistd.h>
#include "global.h"
#include "fanatec_functions.h"
#include "../client/client.h"

int openWheel() {
    if (hid_init()) {
        fprintf(stderr, "ERROR: Failed to initialize HIDAPI");
        return 1;
    }

    handle = hid_open(0x0eb7, 0x0004, NULL);
    if (!handle) {
        fprintf(stderr, "ERROR: Unable to open Fanatec wheel");
        return 1;
    }
    return 0;
}


int calibrateSteering() {
    unsigned char buf[64];
    int pressable = 0;

    uint16_t raw_inp = 0;
    printf("--- Centre wheel and press X.\n\n");
    while (1) {
        if (hid_read(handle, buf, sizeof(buf)) > 0) {
            raw_inp = buf[17] | (buf[18] << 8);
            if (buf[1] == 0x08) pressable = 1;
            if (buf[1] & 0x20) {      // Press X
                if (pressable == 1) {
                    pressable = 0;
                    break;
                }
            }
        }
    }
    wheel_base.steering_center = raw_inp;

    raw_inp = 0;
    printf("--- Turn wheel fully left and press X.\n\n");
    while (1) {
        if (hid_read(handle, buf, sizeof(buf)) > 0) {
            raw_inp = buf[17] | (buf[18] << 8);
            if (buf[1] == 0x08) pressable = 1;
            if (buf[1] & 0x20) {      // Press X
                if (pressable == 1) {
                    pressable = 0;
                    break;
                }
            }
        }
    }
    wheel_base.steering_min = raw_inp;
    
    raw_inp = 0;
    printf("--- Turn wheel fully right and press X.\n\n");
    while (1) {
        if (hid_read(handle, buf, sizeof(buf)) > 0) {
            raw_inp = buf[17] | (buf[18] << 8);
            if (buf[1] == 0x08) pressable = 1;
            if (buf[1] & 0x20) {      // Press X
                if (pressable == 1) {
                    pressable = 0;
                    break;
                }
            }
        }
    }
    wheel_base.steering_max = raw_inp;

    printf("--- Steering angles succesfully calibrated.\n\n");
    return 0;
}

int calibratePedals() {
    unsigned char buf[64];
    uint16_t raw_min, raw_inp;
    int pressable = 0;

    printf("--- Fully press the throttle, and press X.\n\n");
    while (1) {
        if (hid_read(handle, buf, sizeof(buf)) > 0) {
            raw_inp = 65535 - (buf[19] | (buf[20] << 8));
            if (buf[1] == 0x08) pressable = 1;
            if (buf[1] & 0x20) {      // Press X
                if (pressable == 1) {
                    pressable = 0;
                    wheel_base.throttle_max = raw_inp;
                    break;
                }
            }
        }
    }
    usleep(100);
    printf("--- Fully relieve the throttle, and press X.\n\n");
        while (1) {
        if (hid_read(handle, buf, sizeof(buf)) > 0) {
            raw_inp = 65535 - (buf[19] | (buf[20] << 8));
            if (buf[1] == 0x08) pressable = 1;
            if (buf[1] & 0x20) {      // Press X
                if (pressable == 1) {
                    pressable = 0;
                    wheel_base.throttle_min = raw_inp;
                    break;
                }
            }
        }
    }
    usleep(100);
    printf("--- Fully press the brake, and press X.\n\n");
    while (1) {
        if (hid_read(handle, buf, sizeof(buf)) > 0) {
            raw_inp = 65535 - (buf[21] | (buf[22] << 8));
            if (buf[1] == 0x08) pressable = 1;
            if (buf[1] & 0x20) {      // Press X
                if (pressable == 1) {
                    pressable = 0;
                    wheel_base.brake_max = raw_inp;
                    break;
                }
            }
        }
    }
    usleep(100);
    printf("--- Fully relieve the brake, and press X.\n\n");
        while (1) {
        if (hid_read(handle, buf, sizeof(buf)) > 0) {
            raw_inp = 65535 - (buf[21] | (buf[22] << 8));
            if (buf[1] == 0x08) pressable = 1;
            if (buf[1] & 0x20) {      // Press X
                if (pressable == 1) {
                    pressable = 0;
                    wheel_base.brake_min = raw_inp;
                    break;
                }
            }
        }
    }
    usleep(100);
    printf("--- Fully press the clutch, and press X.\n\n");
    while (1) {
        if (hid_read(handle, buf, sizeof(buf)) > 0) {
            raw_inp = 65535 - (buf[23] | (buf[24] << 8));
            if (buf[1] == 0x08) pressable = 1;
            if (buf[1] & 0x20) {      // Press X
                if (pressable == 1) {
                    pressable = 0;
                    wheel_base.clutch_max = raw_inp;
                    break;
                }
            }
        }
    }
    usleep(100);
    printf("--- Fully relieve the clutch, and press X.\n\n");
    while (1) {
        if (hid_read(handle, buf, sizeof(buf)) > 0) {
            raw_inp = 65535 - (buf[23] | (buf[24] << 8));
            if (raw_inp < raw_min) raw_min = raw_inp;
            if (buf[1] == 0x08) pressable = 1;
            if (buf[1] & 0x20) {      // Press X
                if (pressable == 1) {
                    pressable = 0;
                    wheel_base.clutch_min = raw_inp;
                    break;
                }
            }
        }
    }
    usleep(100);
    printf("\n\n----- Calibration done. ------\n\n");
    return 0;
}

int setupWheelBase() {
    openWheel();
    //if (!handle) return 1;

    calibrateSteering();
    calibratePedals();
    
    printf("\n---------- FULL REPORT: ----------\n");
    printf("Value steering_center: %u\n", wheel_base.steering_center);
    printf("Value steering_min: %u\n", wheel_base.steering_min);
    printf("Value steering_max: %u\n", wheel_base.steering_max);
    printf("Value throttle_min: %u\n", wheel_base.throttle_min);
    printf("Value throttle_max: %u\n", wheel_base.throttle_max);
    printf("Value brake_min: %u\n", wheel_base.brake_min);
    printf("Value brake_max: %u\n", wheel_base.brake_max);
    printf("Value clutch_min: %u\n", wheel_base.clutch_min);
    printf("Value clutch_max: %u\n", wheel_base.clutch_max);
    printf("-------------------------------------\n");

    return 0;
}

void sendWheelData() {
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
                angle_deg = ((double)(raw_angle - raw_center) / (raw_max - raw_center)) * RULE_MAX_ROTATION;
            } else {
                angle_deg = ((double)(raw_angle - raw_center) / (raw_center - raw_min)) * RULE_MAX_ROTATION;
            }

            //client_receive();

            char message[100];
            if (buf[1] & 0x40) {
                message[0] = 'C';
                client_send(message);
                printf("\n");
                break;
            }
            snprintf(message, sizeof(message), "%lf", angle_deg);
            client_send(message);

            printf("\rSteering angle: %7.2f°  ", angle_deg);
            fflush(stdout);
        } else if (res < 0) {
            fprintf(stderr, "\nError reading from wheel\n");
            //break;
        }
    }
}