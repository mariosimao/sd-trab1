#include <iostream>
#include <signal.h>

using namespace std;

int main(int argc, char const *argv[])
{
    // Check number of arguments
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <pid> <signal>" << endl;
        exit(EXIT_FAILURE);
    }

    // Convert inputs to int
    int pid = atoi(argv[1]);
    int signal = atoi(argv[2]);

    // Check if process exist
    // When signal = 0, error checking is done but no signal is actually sent
    // https://linux.die.net/man/2/kill
    bool processExist = (kill(pid, 0) != -1);
    if (!processExist) {
        cerr << "process with PID " << pid << " does not exist." << endl;
        exit(EXIT_FAILURE);
    }

    // Send signal to process with given PID (Process ID)
    kill(pid, signal);
    return 0;
}