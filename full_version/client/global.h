#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <hidapi/hidapi.h>
#include <unistd.h>
#include "./fanatec/fanatec_functions.h"

#define RULE_MAX_ROTATION 360.0
#define RULE_BUFFER_SIZE 1024
#define RULE_IP "127.0.0.1"
#define RULE_PORT 5000
#define RULE_LOG_FILE "error_log.txt"
#define RULE_VEN_ID 0x0eb7
#define RULE_PROD_ID 0x0004

extern struct WheelBase {
    uint16_t steering_center;
    uint16_t steering_min;
    uint16_t steering_max;
    uint16_t throttle_min;
    uint16_t throttle_max;
    uint16_t brake_min;
    uint16_t brake_max;
    uint16_t clutch_min;
    uint16_t clutch_max;
} wheel_base;

extern hid_device *handle;

#endif // GLOBAL_H