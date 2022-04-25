#include <iostream>
#include <signal.h>

using namespace std;

int main(int argc, char const *argv[])
{
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <pid> <signal>" << endl;
        return 1;
    }

    int pid = atoi(argv[1]);
    int signal = atoi(argv[2]);

    bool proccessExist = (kill(pid, 0) != -1);
    if (!proccessExist) {
        cerr << "Proccess with PID " << pid << " does not exist." << endl;
        return 1;
    }

    kill(pid, signal);
    return 0;
}