#include <iostream>
#include <string>

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
    if(argc == 1) {
        exit(-1);
    }

    char *p;
    for(int i = 1; i < argc; ++i) {
        p = argv[i];
        std::string newp = "./net/";
        newp += p;
        rename(p, newp.c_str());
    }

    return 0;
}