#ifndef FANATEC_FUNCTIONS_H
#define FANATEC_FUNCTIONS_H

void send_wheel_data();
double calculate_wheel_angle(uint16_t raw_angle);

#endif