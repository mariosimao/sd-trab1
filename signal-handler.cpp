#include <iostream>
#include <csignal>
#include <unistd.h>

using namespace std;

void signalHandler(int signal)
{
    cout << "Signal received: " << signal << endl;

    switch (signal) {
    case SIGINT:
        cout << "Fake interrupt..." << endl << endl;
        break;
    case SIGQUIT:
        cout << "Fake quit..." << endl << endl;
        break;
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
    // Register signal handler for signals 2, 3 and 15
    signal(SIGINT, signalHandler);
    signal(SIGQUIT, signalHandler);
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