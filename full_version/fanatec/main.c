#include "./global.h"
#include "./fanatec_functions.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../client/client.h"


int main () {
    if (!setupWheelBase()) {
        if (client_init(RULE_IP, RULE_PORT) != 0) {
            printf("Failed to initialize client.\n");
            return -1;
        }
        sendWheelData();
        client_close();
    }

    hid_close(handle);
    hid_exit();
    return 0;
}