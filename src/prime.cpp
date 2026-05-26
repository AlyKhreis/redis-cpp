#include "prime.h"
bool is_prime(int n) {
    if (n<=1) return false;
    if (n == 2) return true;
    for (int i = 2; i <= std::sqrt(n); i++) {
        if (n%i==0) return false;
    }
    return true;
}

int next_prime(int n) {
    while (!is_prime(n)) {
        n++;
    }
    return n;
}