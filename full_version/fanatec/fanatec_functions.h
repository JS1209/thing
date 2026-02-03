// fanatec_functions.h
#ifndef FANATEC_FUNCTIONS_H
#define FANATEC_FUNCTIONS_H

#include "global.h"
// Function declarations (prototypes)
int openWheel();
int calibrateSteering();
int calibratePedals();
int setupWheelBase();
void sendWheelData();
void calibrateWheelBaseValue(int par_wheelbase_entry, char *par_message);

#endif // FANATEC_FUNCTIONS_H