#include <iostream>
#include <csignal>
#include <unistd.h>

using namespace std;

void signalHandler(int signal)
{
    switch (signal) {
    case SIGINT:
        cout << "Interrupting..." << endl;
        exit(0);
    case SIGQUIT:
        cout << "Quitting..." << endl;
        exit(0);
    case SIGKILL:
        cout << "Killing..." << endl;
        exit(0);
    case SIGTERM:
        cout << "Terminating..." << endl;
        exit(0);
    default:
        break;
    }

    return;
}

int main(int argc, char const *argv[])
{
    // Register signal handler for signals 2, 3, 9 and 15
    signal(SIGINT, signalHandler);
    signal(SIGQUIT, signalHandler);
    signal(SIGKILL, signalHandler);
    signal(SIGTERM, signalHandler);

    int pid = getpid();
    cout << "Starting process..." << endl;
    cout << "PID: " << pid << endl << endl;

    // Default to busy waiting
    string waitType = "busy";
    if (argc > 1) {
        waitType = string(argv[1]);
    }

    if (waitType == "busy") {
        while (true);
    } else if (waitType == "blocking") {
        getchar();
    } else {
        cerr << "Invalid wait type. Use 'busy' or 'blocking'." << endl;
        exit(EXIT_FAILURE);
    }

    return 0;
}