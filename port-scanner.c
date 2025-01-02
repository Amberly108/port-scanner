#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

void print_banner() {
    printf(" _____            _   _____                     \n");
    printf("|  __ \\          | | / ____|                    \n");
    printf("| |__) |__ _ __ | || (___   ___  _ __ ___   ___ \n");
    printf("|  ___/ _ \\ '_ \\| | \\___ \\ / _ \\| '_ ` _ \\ / _ \\\n");
    printf("| |  |  __/ | | | | ____) | (_) | | | | | |  __/\n");
    printf("|_|   \\___|_| |_|_| |_____/ \\___/|_| |_| |_|\\___|\n");
}

int main(int argc, char *argv[]) {
    char *target;
    struct sockaddr_in server;
    int port;
    int sock;
    time_t start_time;

    print_banner();

    // Defining a target
    if (argc == 2) {
        target = argv[1];
    } else {
        printf("Invalid amount of Argument\n");
        return 1;
    }

    // Add Banner 
    printf("--------------------------------------------------\n");
    printf("Scanning Target: %s\n", target);
    time(&start_time);
    printf("Scanning started at: %s", ctime(&start_time));
    printf("--------------------------------------------------\n");

    // will scan ports between 1 to 65,535
    for (port = 1; port <= 65535; port++) {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("Socket creation failed");
            return 1;
        }

        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        inet_pton(AF_INET, target, &server.sin_addr);

        // Set timeout
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));

        // returns an error indicator
        if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == 0) {
            printf("Port %d is open\n", port);
        }
        close(sock);
    }

    return 0;
}

