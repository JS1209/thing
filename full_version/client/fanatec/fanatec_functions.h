// fanatec_functions.h
#ifndef FANATEC_FUNCTIONS_H
#define FANATEC_FUNCTIONS_H

#include "../global.h"
// Function declarations (prototypes)
int open_wheel();
int calibrate_steering();
int calibrate_pedals();
int setup_wheelbase();
void send_wheel_data();
void calibrate_wheelbase_value(int wheelbase_entry, char *par_message);

#endif // FANATEC_FUNCTIONS_H