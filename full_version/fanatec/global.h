#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <hidapi/hidapi.h>
#include <unistd.h>
#include "global.h"
#include "fanatec_functions.h"

#define MAX_ROTATION 360.0

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