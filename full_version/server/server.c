#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "global.h"

// Global variables to store server information
int server_socket;
struct sockaddr_in server_addr;

// Function prototypes
void handle_request(struct sockaddr_in *client_addr, socklen_t client_len);
void start_server();
void stop_server();

void start_server() {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[RULE_BUFFER_SIZE];
    ssize_t n;

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = RULE_IPVX;
    if (RULE_ALL_INTERFACES == 1) server_addr.sin_addr.s_addr = RULE_INTERFACE; else {
        const char *ip_address = RULE_IP;  // Replace with the desired IP address
        if (inet_pton(AF_INET, ip_address, &server_addr.sin_addr) <= 0) {
            perror("Invalid IP address");
            close(server_socket);
            exit(EXIT_FAILURE);
        }
    }
    server_addr.sin_port = htons(RULE_PORT);        // Use defined port

    // Bind the socket to the address and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d (UDP)...\n", RULE_PORT);

    // Continuously receive messages from clients
    while (1) {
        // Receive a message from a client
        n = recvfrom(server_socket, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&client_addr, &client_len);
        if (n < 0) {
            perror("Receive failed");
            continue;
        }

        buffer[n] = '\0';  // Null terminate the received string
        if (buffer[0] == 'C') break;
        printf("\rSteering angle: %7.2f°  ", strtod(buffer, NULL));
        fflush(stdout);

        // Send a response to the client
        const char *response = "Command received successfully\n";
        sendto(server_socket, response, strlen(response), 0, (struct sockaddr *)&client_addr, client_len);
    }
}

void stop_server() {
    if (server_socket > 0) {
        close(server_socket);
        printf("Server stopped.\n");
    }
}

int main() {
    // Start the server
    start_server();  // This runs the server in the background
    stop_server();

    return 0;
}
