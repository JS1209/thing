#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <hidapi/hidapi.h>
#include <unistd.h>
#include "../global.h"
#include "../fanatec/fanatec_functions.h"
#include "../network/client.h"
#include "../errorhandling/error_handling.h"
#include "../additional/functions.h"

int check_device_available(unsigned short vendor_id, unsigned short product_id) {
    struct hid_device_info *devs, *cur_dev;
    int found = 0;
    devs = hid_enumerate(0, 0);
    cur_dev = devs;

    while (cur_dev) {
        if (cur_dev->vendor_id == vendor_id && cur_dev->product_id == product_id) {
            found = 1;
            break;
        }

        if (!found && cur_dev->next == NULL) log_perror_to_file("/additional/functions.check_device_available", "Error: No device available.");
        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);

    return found;
}