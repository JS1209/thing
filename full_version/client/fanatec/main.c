#include "../global.h"
#include "./fanatec_functions.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../network/client.h"
#include "../errorhandling/error_handling.h"


int main () {
    if (!setup_wheelbase()) {
        if (client_init(RULE_IP, RULE_PORT) != 0) {
            return -1;
        }
        send_wheel_data();
        client_close();
    }

    hid_close(handle);
    hid_exit();
    return 0;
}