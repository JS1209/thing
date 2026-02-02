#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"  // Change this to the server's IP if needed
#define SERVER_PORT 5000
#define COMMAND "turn_on_led"

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    // Convert IP address to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Send the command to the server
    send(sock, COMMAND, strlen(COMMAND), 0);
    printf("Command sent: %s\n", COMMAND);

    // Receive the server's response
    ssize_t n = read(sock, buffer, sizeof(buffer) - 1);
    if (n < 0) {
        perror("Read error");
        close(sock);
        exit(EXIT_FAILURE);
    }
    buffer[n] = '\0';  // Null terminate the received string

    // Print the server's response
    printf("Server response: %s\n", buffer);

    // Close the socket
    close(sock);
    return 0;
}
