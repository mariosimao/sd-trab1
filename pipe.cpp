#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include "common.h"

using namespace std;

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <numbers>" << endl;
        exit(EXIT_FAILURE);
    }

    int numbers = atoi(argv[1]);

    // Based on "man pipe" https://man7.org/linux/man-pages/man2/pipe.2.html
    int pipeFileDescriptor[2];
    pid_t childId;
    char buf[20];

    if (pipe(pipeFileDescriptor) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    childId = fork();
    if (childId == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (childId == 0) { // Child
        close(pipeFileDescriptor[1]);

        cout << "Number\tResult" << endl;
        while (read(pipeFileDescriptor[0], &buf, 20) > 0) {
            int childInput = atoi(buf);

            if (childInput == 0) {
                close(pipeFileDescriptor[0]);
                _exit(EXIT_SUCCESS);
            }

            bool prime = isPrime(childInput);
            if (prime) {
                cout << childInput << "\t\033[32mprime\033[0m" << endl;
            } else {
                cout << childInput << "\t\033[31mnot prime\033[0m" << endl;
            }
        }
    } else { // Parent
        close(pipeFileDescriptor[0]);

        int previousNumber = 1;
        for (int i = 0; i < numbers; i++) {
            int currentNumber = generateNumber(previousNumber);
            previousNumber = currentNumber;

            char parentOutput[20];
            sprintf(parentOutput, "%d", currentNumber);
            write(pipeFileDescriptor[1], parentOutput, 20);
        }
        write(pipeFileDescriptor[1], "0", 20);

        close(pipeFileDescriptor[1]);
        wait(NULL);
        exit(EXIT_SUCCESS);
    }
}
