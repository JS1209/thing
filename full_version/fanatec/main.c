#include "./global.h"
#include "./fanatec_functions.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../client/client.h"


int main () {
    if (!setupWheelBase()) {
        if (client_init("127.0.0.1", 5000) != 0) {
            printf("Failed to initialize client.\n");
            return -1;
        }
        sendWheelData();
        // Example loop to continuously send and receive messages asynchronously
        //int counter = 0;
        //while (1) {
            // Continuously check for incoming messages
        //    client_receive();

            // Send a message asynchronously
//            char message[100];
       //     snprintf(message, sizeof(message), "Message #%d", counter++);
  //          client_send(message);
            // Sleep for a short time to prevent busy-waiting
    //        usleep(1);  // Sleep for 100 ms
        //}

        // Close the client connection when done
        client_close();
    }

    hid_close(handle);
    hid_exit();
    return 0;
}