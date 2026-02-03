// fanatec_functions.h
#ifndef FANATEC_INIT_H
#define FANATEC_INIT_H

#include "../global.h"

int open_wheel();
int calibrate_steering();
int calibrate_pedals();
int setup_wheelbase();
void send_wheel_data();

#endif