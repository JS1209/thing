#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define BUFFER_SIZE 1024

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
    char buffer[BUFFER_SIZE];
    ssize_t n;

    // Create the server socket (SOCK_DGRAM for UDP)
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Listen on all interfaces
    server_addr.sin_port = htons(PORT);        // Use defined port

    // Bind the socket to the address and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d (UDP)...\n", PORT);

    // Continuously receive messages from clients
    while (1) {
        // Receive a message from a client
        n = recvfrom(server_socket, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&client_addr, &client_len);
        if (n < 0) {
            perror("Receive failed");
            continue;
        }

        buffer[n] = '\0';  // Null terminate the received string
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

    // Uncomment the line below if you want to stop the server programmatically at some point
    // stop_server();

    return 0;
}
