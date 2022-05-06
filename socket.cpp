#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
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

    const int bufferSize = 20;

    if (mode == "server") {
        int socketFileDescriptor, newSocketFileDescriptor;
        struct sockaddr_in serverAddress, clientAddress;
        char buffer[bufferSize];

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

            while (read(newSocketFileDescriptor, &buffer, bufferSize) > 0) {
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
                char serverOutput[bufferSize];
                if (prime) {
                    sprintf(serverOutput, "Y");
                } else {
                    sprintf(serverOutput, "N");
                }

                write(newSocketFileDescriptor, serverOutput, bufferSize);
            }

            close(newSocketFileDescriptor);
        }

        close(socketFileDescriptor);
        exit(EXIT_SUCCESS);
    } else if (mode == "client") {
        int socketFileDescriptor;
        struct sockaddr_in serverAddress;
        char buffer[bufferSize];
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

            char clientOutput[bufferSize];
            sprintf(clientOutput, "%d", currentNumber);
            cout << currentNumber << "\t";

            write(socketFileDescriptor, clientOutput, bufferSize);
            read(socketFileDescriptor, &buffer, bufferSize);

            if (buffer[0] == 'Y') {
                // \033[32m = green text, \033[0m back to normal
                cout << "\033[32mprime\033[0m" << endl;
            } else if (buffer[0] == 'N') {
                // \033[31m = red text
                cout << "\033[31mnot prime\033[0m" << endl;
            } else {
                cerr << "Invalid server response." << endl;
                exit(EXIT_FAILURE);
            }
        }

        send(socketFileDescriptor, "0", bufferSize, 0);

        close(socketFileDescriptor);
        exit(EXIT_SUCCESS);
    } else {
        usageErrorMessage(argv);
    }
}