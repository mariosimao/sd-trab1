#include <iostream>
#include <csignal>
#include <unistd.h>

using namespace std;

void signalHandler(int signal)
{
    switch (signal) {
    case SIGINT:
        cout << "Interrupting..." << endl;
        sleep(1);
        exit(0);
        break;
    case SIGQUIT:
        cout << "Quitting..." << endl;
        sleep(1);
        exit(0);
        break;
    case SIGKILL:
        cout << "Killing..." << endl;
        sleep(1);
        exit(0);
        break;
    case SIGTERM:
        cout << "Terminating..." << endl;
        exit(0);
        break;
    default:
        break;
    }

    return;
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, signalHandler);
    signal(SIGQUIT, signalHandler);
    signal(SIGKILL, signalHandler);
    signal(SIGTERM, signalHandler);

    int pid = getpid();
    cout << "Starting process..." << endl;
    cout << "PID: " << pid << endl << endl;

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
        return 1;
    }

    return 0;
}