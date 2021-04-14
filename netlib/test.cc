#include <stdio.h>
#include <stdlib.h>
#include <iostream>
struct dp {
    int k;
    int l;
};

int main()
{
    dp p;
    std::cout << sizeof(p) << "\n";
    std::cout << sizeof p << "\n";
}