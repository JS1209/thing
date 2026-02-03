#include "./global.h"
#include "./fanatec/fanatec_functions.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "./network/client.h"
#include "./errorhandling/error_handling.h"


int main () {
    log_start();
    if (!setup_wheelbase()) {
        if (client_init(RULE_IP, RULE_PORT) != 0) {
            log_perror_to_file("/main.main", "but like really main. This is the fourth line...");
            return -1;
        }
        send_wheel_data();
        client_close();
    }

    hid_close(handle);
    hid_exit();
    log_end();
    return 0;
}