#include "include.h"
int generateUniqueId(int n1, int n2) {
    if (n1 < n2) {
        const int buf = n2;
        n2 = n1;
        n1 = buf;
    }
    return (n1+n2) * (n1+n2+1) /2 + n2;
}