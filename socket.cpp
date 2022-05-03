#include <iostream>
#include <sys/socket.h>
// #include <stdio.h>
#include <stdlib.h>
// #include <string.h>
#include <unistd.h>
// #include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "common.h"

using namespace std;

void usageErrorMessage(char *argv[])
{
    cerr << "Usage: " << argv[0] << " server <port> or "
         << argv[0] << " client <port> <numbers>" << endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    // Check arguments
    if (argc < 3) {
        usageErrorMessage(argv);
    }

    // Convert input to proper types
    string mode = string(argv[1]);
    int port = atoi(argv[2]);

    if (mode == "server") {
        int socketFileDescriptor, newSocketFileDescriptor;
        struct sockaddr_in serverAddress, clientAddress;
        char buffer[20];

        /**
         * Create socket
         *
         * AF_INET = IPV4
         * SOCK_STREAM = TCP
         * 0 = automatic protocol
         */
        socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
        if (socketFileDescriptor == 0) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        // Setup server address
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_port = htons(port);

        // Give socket the defined server address
        int bindResult = bind(
            socketFileDescriptor,
            (struct sockaddr *) &serverAddress,
            sizeof(serverAddress)
        );
        if (bindResult == -1) {
            perror("bind");
            exit(EXIT_FAILURE);
        }

        /**
         * Prepare to accept connections
         *
         * 5 = pending connections queue maximum size
         */
        listen(socketFileDescriptor, 5);

        cout << "Listening on "
             << inet_ntoa(serverAddress.sin_addr) << ":" << ntohs(serverAddress.sin_port) << endl;

        socklen_t clientAddressLength = sizeof(clientAddress);

        while (1) {
            // Wait for socket connection
            newSocketFileDescriptor = accept(
                socketFileDescriptor,
                (struct sockaddr *) &clientAddress,
                &clientAddressLength
            );

            while (recv(newSocketFileDescriptor, &buffer, 20, 0) > 0) {
                // Convert socket input to int
                int serverInput = atoi(buffer);

                // Close when input is zero
                if (serverInput == 0) {
                    close(newSocketFileDescriptor);
                    close(socketFileDescriptor);
                    exit(EXIT_SUCCESS);
                }

                bool prime = isPrime(serverInput);

                // Convert
                char serverOutput[20];
                if (prime) {
                    sprintf(serverOutput, "Y");
                } else {
                    sprintf(serverOutput, "N");
                }
                send(newSocketFileDescriptor, serverOutput, 20, 0);
            }
        }
    } else if (mode == "client") {
        int socketFileDescriptor;
        struct sockaddr_in serverAddress;
        char buffer[20];
        int numbers;

        if (argc != 4) {
            usageErrorMessage(argv);
        }
        numbers = atoi(argv[3]);

        /**
         * Create socket
         *
         * AF_INET = IPV4
         * SOCK_STREAM = TCP
         * 0 = automatic protocol
         */
        socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
        if (socketFileDescriptor == 0) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(port);

        if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) == -1) {
            perror("inet_pton");
            exit(EXIT_FAILURE);
        }

        int connectResult = connect(
            socketFileDescriptor,
            (struct sockaddr*) &serverAddress,
            sizeof(serverAddress)
        );
        if (connectResult == -1) {
            perror("connect");
            exit(EXIT_FAILURE);
        }

        cout << "Connected" << endl;

        int previousNumber = 1;
        for (int i = 0; i < numbers; i++) {
            int currentNumber = generateNumber(previousNumber);
            previousNumber = currentNumber;

            char clientOutput[20];
            sprintf(clientOutput, "%d", currentNumber);
            cout << currentNumber << "\t";

            send(socketFileDescriptor, clientOutput, 20, 0);
            recv(socketFileDescriptor, &buffer, 20, 0);

            if (buffer[0] == 'Y') {
                cout << "\033[32mprime\033[0m" << endl;
            } else if (buffer[0] == 'N') {
                cout << "\033[31mnot prime\033[0m" << endl;
            } else {
                cerr << "Invalid server response." << endl;
                exit(EXIT_FAILURE);
            }
        }

        send(socketFileDescriptor, "0", 20, 0);
    } else {
        usageErrorMessage(argv);
    }
}