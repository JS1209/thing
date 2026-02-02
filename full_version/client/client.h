#ifndef CLIENT_H
#define CLIENT_H

#include <stdint.h>

// Function declarations
int client_init(const char *server_ip, int port);  // Initialize and connect the client
void client_send(const char *message);              // Send a message to the server
void client_receive();                              // Receive a message from the server
void client_close();                                // Close the client connection

extern int client_socket;  // The global client socket

#endif // CLIENT_H
