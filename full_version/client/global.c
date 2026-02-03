#include "global.h"
#include <hidapi/hidapi.h>

struct WheelBase wheel_base = {
    .steering_center = 0,
    .steering_min = 0,
    .steering_max = 0,
    .throttle_min = 0,
    .throttle_max = 0,
    .brake_min = 0,
    .brake_max = 0,
    .clutch_min = 0,
    .clutch_max = 0
};
hid_device *handle = NULL;
