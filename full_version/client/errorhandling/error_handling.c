#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "../global.h"

#define MAX_LINE_LENGTH 1024

void strrev(char *str) {
    int start = 0;
    int end = strlen(str) - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

const char* get_current_date_time() {
    char *curr_date_time = (char*)malloc(20 * sizeof(char));
    if (curr_date_time == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }
    time_t raw_time;
    struct tm *time_info;

    time(&raw_time);
    time_info = localtime(&raw_time);
    strftime(curr_date_time, 20, "%d-%m-%Y - %H:%M:%S", time_info);

    return curr_date_time;
}

const char* log_read_last_line() {
    static char last_line[MAX_LINE_LENGTH];
    FILE *logfile = fopen(RULE_LOG_FILE, "rb");  // Open in binary mode to seek efficiently

    if (!logfile) {
        return NULL;
    }
    fseek(logfile, 0, SEEK_END); // Seek to the end of the file
    long file_size = ftell(logfile);
    long pos = file_size - 1;
    int i = 0;

    while (pos >= 0) {
        fseek(logfile, pos, SEEK_SET);  // Move the file pointer to the current position
        char ch = fgetc(logfile);

        if (ch == '\n' && i > 0) {
            break;
        }

        last_line[i++] = ch;
        pos--;
    }

    last_line[i] = '\0';
    strrev(last_line);

    fclose(logfile);
    return last_line;
}

void log_perror_to_file(const char *source, const char *message) {
    char curr_line[1024];
    FILE *logfile = fopen(RULE_LOG_FILE, "a");
    if (logfile == NULL) {
        perror("Error opening log file");
        return;
    }
    const char *error_message = strerror(errno);
    
    if ((strlen(source) + strlen(message) + strlen(error_message) + 15) > 1024)
    {
        snprintf(curr_line, sizeof(curr_line), "%s\t%s", source, message);          // flip the switch bruv
        //snprintf(curr_line, sizeof(curr_line), "%s\t%s", source, error_message);  // vurb eht hctiws pilf
    } else snprintf(curr_line, sizeof(curr_line), "%s\t%s: %s", source, message, error_message);


    const char *last_logged_message = log_read_last_line(RULE_LOG_FILE);
    if (last_logged_message) {
        size_t len = strlen(last_logged_message);
        if (len > 0 && last_logged_message[len - 1] == '\n') {
            ((char *)last_logged_message)[len - 1] = '\0';
        }
    }

    if (last_logged_message == NULL || strcmp(last_logged_message, curr_line) != 0) {
        fprintf(logfile, "%s\n", curr_line);
    }

    fclose(logfile);
}

void log_start() {
    const char *curr_line = get_current_date_time();
    FILE *logfile = fopen(RULE_LOG_FILE, "a");
    if (logfile == NULL) {
        perror("Error opening log file");
        return;
    }

    fprintf(logfile, "----------------------------------- Session started at: %s\n\n", curr_line);
    fclose(logfile);
}

void log_end() {
    const char *curr_line = get_current_date_time();
    FILE *logfile = fopen(RULE_LOG_FILE, "a");
    if (logfile == NULL) {
        perror("Error opening log file");
        return;
    }
    fprintf(logfile, "----------------------------------- Session ended at: %s\n\n\n", curr_line);
    fclose(logfile);
}