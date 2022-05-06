#include <stdlib.h>
#include <time.h>
#include "common.h"

int generateNumber(int previousNumber)
{
    srand(time(NULL));
    int delta = rand() % 100 + 1;

    return previousNumber + delta;
}

bool isPrime(int n)
{
    if (n <= 1) {
        return false;
    }

    for (int i = 2; i < n; i++) {
        if (n % i == 0) {
            return false;
        }
    }

    return true;
}
