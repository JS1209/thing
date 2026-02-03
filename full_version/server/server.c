#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "global.h"

// Global variables to store server information
int server_socket;
struct sockaddr_in server_addr;
struct sockaddr_in client_addr;

void server_start() {
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
        const char *ip_address = RULE_IP;
        if (inet_pton(AF_INET, ip_address, &server_addr.sin_addr) <= 0) {
            perror("Invalid IP address");
            close(server_socket);
            exit(EXIT_FAILURE);
        }
    }
    server_addr.sin_port = htons(RULE_PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d (UDP)...\n", RULE_PORT);
}

void server_stop() {
    if (server_socket > 0) {
        close(server_socket);
        printf("Server stopped.\n");
    }
}

void server_recieve() {
    socklen_t client_len = sizeof(client_addr);
    char buffer[RULE_BUFFER_SIZE];
    ssize_t n;
    while (1) {
        n = recvfrom(server_socket, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&client_addr, &client_len);
        if (n < 0) {
            perror("Receive failed");
            continue;
        }

        buffer[n] = '\0';  // Null terminate the received string
        if (buffer[0] == 'C') break;    //rondje ingedrukt
        printf("\rSteering angle: %7.2f°  ", strtod(buffer, NULL));
        fflush(stdout);

        // Send a response to the client
        const char *response = "Command received successfully\n";
        sendto(server_socket, response, strlen(response), 0, (struct sockaddr *)&client_addr, client_len);
    }
}

int main() {
    server_start();
    server_recieve();
    server_stop();

    return 0;
}
