#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <stdint.h>

// Function declarations

void strrev(char *str);
void log_perror_to_file(const char *source, const char *message);
const char* get_current_date_time();
const char* log_read_last_line();
void log_start();
void log_end();

#endif // ERROR_HANDLING_H
