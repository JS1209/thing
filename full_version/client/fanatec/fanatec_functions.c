#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <hidapi/hidapi.h>
#include <unistd.h>
#include "../global.h"
#include "fanatec_functions.h"
#include "../network/client.h"
#include "../errorhandling/error_handling.h"

int check_device_available(unsigned short vendor_id, unsigned short product_id) {
    struct hid_device_info *devs, *cur_dev;
    int found = 0;
    devs = hid_enumerate(0, 0);
    cur_dev = devs;

    while (cur_dev) {
        if (cur_dev->vendor_id == vendor_id && cur_dev->product_id == product_id) {
            found = 1;
            break;
        }

        if (!found && cur_dev->next == NULL) log_perror_to_file("/fanatec/fanatec_functions.check_device_available", "Error: No device available.");
        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);

    return found;
}

int open_wheel() {
    if (hid_init()) {
        fprintf(stderr, "ERROR: Failed to initialize HIDAPI");
        log_perror_to_file("/fanatec/fanatec_functions.open_wheel", "Error: Wheel could not be opened, hid_init failed");
        return 1;
    }
    if (check_device_available(0x0eb7, 0x0004)) {
        handle = hid_open(0x0eb7, 0x0004, NULL);
        if (!handle) {
            fprintf(stderr, "ERROR: Unable to open Fanatec wheel");
            log_perror_to_file("/fanatec/fanatec_functions.open_wheel", "Error: Wheel could not be opened, handle is NULL");
            return 1;
        }
    }
    return 0;
}

void calibrate_wheelbase_value(int wheelbase_entry, char *message) {
    unsigned char buf[64];
    int pressable = 0;
    uint16_t raw_inp = 0;
    int var_buf_num[2];

    if (wheelbase_entry == 0 || wheelbase_entry == 1 || wheelbase_entry == 2) {
        var_buf_num[0] = 17;
        var_buf_num[1] = 18;
    }
    else if (wheelbase_entry == 3 || wheelbase_entry == 4) {
        var_buf_num[0] = 19;
        var_buf_num[1] = 20;
    }
    else if (wheelbase_entry == 5 || wheelbase_entry == 6) {
        var_buf_num[0] = 21;
        var_buf_num[1] = 22;
    }
    else if (wheelbase_entry == 7 || wheelbase_entry == 8) {
        var_buf_num[0] = 23;
        var_buf_num[1] = 24;
    }
    printf(message, sizeof(message));
    while (1) {
        if (hid_read(handle, buf, sizeof(buf)) > 0) {
            raw_inp = buf[var_buf_num[0]] | (buf[var_buf_num[1]] << 8);
            if (buf[1] == 0x08) pressable = 1;
            if (buf[1] & 0x20) {
                if (pressable == 1) {
                    pressable = 0;
                    break;
                }
            }
        }
    }

    if (wheelbase_entry == 0) wheel_base.steering_center = raw_inp;
    else if (wheelbase_entry == 1) wheel_base.steering_min = raw_inp;
    else if (wheelbase_entry == 2) wheel_base.steering_max = raw_inp;
    else if (wheelbase_entry == 3) wheel_base.throttle_min = raw_inp;
    else if (wheelbase_entry == 4) wheel_base.throttle_max = raw_inp;
    else if (wheelbase_entry == 5) wheel_base.brake_min = raw_inp;
    else if (wheelbase_entry == 6) wheel_base.brake_max = raw_inp;
    else if (wheelbase_entry == 7) wheel_base.clutch_min = raw_inp;
    else if (wheelbase_entry == 8) wheel_base.clutch_max = raw_inp;
}

int calibrate_steering() {
    calibrate_wheelbase_value(0, "--- Centre wheel and press X.\n");
    calibrate_wheelbase_value(1, "--- Turn wheel fully left and press X.\n");
    calibrate_wheelbase_value(2, "--- Turn wheel fully right and press X.\n");
    printf("\n\n----------------------------------- Steering calibration done.\n\n\n");
    return 0;
}

int calibrate_pedals() {
    calibrate_wheelbase_value(3, "--- Fully press the throttle, and press X.\n");
    calibrate_wheelbase_value(4, "--- Fully relieve the throttle, and press X.\n");
    calibrate_wheelbase_value(5, "--- Fully press the brake, and press X.\n");
    calibrate_wheelbase_value(6, "--- Fully relieve the brake, and press X.\n");
    calibrate_wheelbase_value(7, "--- Fully press the clutch, and press X.\n");
    calibrate_wheelbase_value(8, "--- Fully relieve the clutch, and press X.\n");
    
    printf("\n\n------------------------------------ Pedal calibration done.\n\n");
    return 0;
}

int setup_wheelbase() {
    if (open_wheel()) {
        log_perror_to_file("/fanatec/fanatec_functions.setup_wheelbase", "Error: Setup failed miserably. Close program.");
        return 1;
    }
    if (!handle) return 1;

    calibrate_steering();
    calibrate_pedals();
    
    printf("\n---------- FULL REPORT: ----------\n");
    printf("/*\tValue steering_center\t: %u\t*/\n", wheel_base.steering_center);
    printf("/*\tValue steering_min\t: %u\t*/\n", wheel_base.steering_min);
    printf("/*\tValue steering_max\t: %u\t*/\n", wheel_base.steering_max);
    printf("/*\tValue throttle_min\t: %u\t*/\n", wheel_base.throttle_min);
    printf("/*\tValue throttle_max\t: %u\t*/\n", wheel_base.throttle_max);
    printf("/*\tValue brake_min\t\t: %u\t*/\n", wheel_base.brake_min);
    printf("/*\tValue brake_max\t\t: %u\t*/\n", wheel_base.brake_max);
    printf("/*\tValue clutch_min\t: %u\t*/\n", wheel_base.clutch_min);
    printf("/*\tValue clutch_max\t: %u\t*/\n", wheel_base.clutch_max);
    printf("-------------------------------------\n\n\n");

    return 0;
}

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