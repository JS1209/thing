#include "./client.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <sys/select.h>
#include "../errorhandling/error_handling.h"

int client_socket = -1;

void set_non_blocking(int sockfd) {
    fcntl(sockfd, F_SETFL, O_NONBLOCK); // Set socket to non-blocking
}

// Function to initialize and connect the client to the server
int client_init(const char *server_ip, int port) {
    struct sockaddr_in server_addr;
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0) {
        log_perror_to_file("/fanatec/client.client_init", "Socket creation failed");
        return -1;
    }
    set_non_blocking(client_socket);
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        log_perror_to_file("/fanatec/client.client_init", "Invalid address or address not supported");
        close(client_socket);
        return -1;
    }
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        if (errno != EINPROGRESS) { // Connection attempt failed, but that's okay for non-blocking sockets
        log_perror_to_file("/fanatec/client.client_init", "Connection failed");
            close(client_socket);
            return -1;
        }
    }

    printf("Connecting to server at %s:%d...\n\n\n", server_ip, port);
    return 0;
}

void client_send(const char *message) {
    if (client_socket < 0) {
        printf("Client socket not initialized!\n");
        return;
    }

    // Use select to check if socket is ready for writing
    fd_set write_fds;
    FD_ZERO(&write_fds);
    FD_SET(client_socket, &write_fds);

    struct timeval timeout = { 1, 0 };  // 1 second timeout
    int ready = select(client_socket + 1, NULL, &write_fds, NULL, &timeout);
    if (ready < 0) {
        log_perror_to_file("/client/client.client_send", "Select failed");
        return;
    }

    if (FD_ISSET(client_socket, &write_fds)) {
        if (send(client_socket, message, strlen(message), 0) < 0) {
            log_perror_to_file("/client/client.client_send", "Send failed");
        } else {
            //printf("\rMessage sent: %s", message);
        }
    } else {
        printf("Socket not ready for sending. Retrying...\n");
    }
}

void client_receive() {
    if (client_socket < 0) {
        printf("Client socket not initialized!\n");
        return;
    }

    // Use select to check if socket is ready for reading
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(client_socket, &read_fds);

    struct timeval timeout = { 0, 0 };  // 1 second timeout
    int ready = select(client_socket + 1, &read_fds, NULL, NULL, &timeout);
    if (ready < 0) {
        perror("select failed");
        log_perror_to_file("/client/client.client_recieve", "Select failed");
        return;
    }

    if (FD_ISSET(client_socket, &read_fds)) {
        char buffer[1024];
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received < 0) {
            log_perror_to_file("/client/client.client_recieve", "Recieve failed");
        } else if (bytes_received == 0) {
            printf("Server closed the connection.\n");
        } else {
            buffer[bytes_received] = '\0';
            printf("Received message: %s\n", buffer);
        }
    } else {
        printf("No data received within timeout.\n");
    }
}

void client_close() {
    if (client_socket >= 0) {
        close(client_socket);
        client_socket = -1;
        printf("Client connection closed.\n");
    }
}
