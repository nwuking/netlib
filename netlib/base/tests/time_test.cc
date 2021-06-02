#include "netlib/base/Time.h"

#include <iostream>
//#include <stdio.h>

void passByValue(netlib::Time t) {
    std::cout << "passByValue " << t.toString().c_str() << "\n";
    //printf("passByValue %f\n", t.toString().c_str());
}

void passByReference(netlib::Time &t) {
    std::cout << "passByReference " << t.toString().c_str() << "\n";
}

void benchmark() {
}

int main() {
    netlib::Time now(netlib::Time::now());
    std::cout << now.toString().c_str() << "\n";
    passByValue(now);
    passByReference(now);
    return 0;
}