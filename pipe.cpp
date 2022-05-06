#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include "common.h"

using namespace std;

int main(int argc, char const *argv[])
{
    // Check arguments
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <numbers>" << endl;
        exit(EXIT_FAILURE);
    }

    // Convert input to int
    int numbers = atoi(argv[1]);

    // Based on "man pipe" https://man7.org/linux/man-pages/man2/pipe.2.html
    int pipeFileDescriptor[2];
    pid_t childId;
    const int bufferSize = 20;
    char buffer[bufferSize];

    // Create pipe
    if (pipe(pipeFileDescriptor) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Create child process
    childId = fork();
    if (childId == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (childId == 0) { // Child
        close(pipeFileDescriptor[1]);

        cout << "Number\tResult" << endl;
        while (read(pipeFileDescriptor[0], &buffer, bufferSize) > 0) {
            int childInput = atoi(buffer);

            if (childInput == 0) {
                close(pipeFileDescriptor[0]);
                exit(EXIT_SUCCESS);
            }

            bool prime = isPrime(childInput);
            if (prime) {
                // \033[32m = green text, \033[0m back to normal
                cout << childInput << "\t\033[32mprime\033[0m" << endl;
            } else {
                // \033[31m = red text
                cout << childInput << "\t\033[31mnot prime\033[0m" << endl;
            }
        }

        close(pipeFileDescriptor[0]);
        exit(EXIT_SUCCESS);
    } else { // Parent
        close(pipeFileDescriptor[0]);

        int previousNumber = 1;
        for (int i = 0; i < numbers; i++) {
            int currentNumber = generateNumber(previousNumber);
            previousNumber = currentNumber;

            // Convert number to string with <bufferSize> bytes
            char parentOutput[bufferSize];
            sprintf(parentOutput, "%d", currentNumber);

            write(pipeFileDescriptor[1], parentOutput, bufferSize);
        }

        write(pipeFileDescriptor[1], "0", bufferSize);

        close(pipeFileDescriptor[1]);
        wait(NULL);
        exit(EXIT_SUCCESS);
    }
}
