#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "global.h"

#define RULE_BUFFER_SIZE 1024
#define RULE_PORT 5000
#define RULE_IP "127.0.0.1"
#define RULE_INTERFACE INADDR_ANY
#define RULE_IPVX AF_INET
#define RULE_ALL_INTERFACES 1

//extern struct WheelBase {
//    uint16_t steering_center;
//    uint16_t steering_min;
//    uint16_t steering_max;
//    uint16_t throttle_min;
//    uint16_t throttle_max;
//    uint16_t brake_min;
//    uint16_t brake_max;
//    uint16_t clutch_min;
//    uint16_t clutch_max;
//} wheel_base;
//extern hid_device *handle;

#endif // GLOBAL_H