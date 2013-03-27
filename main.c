#include <stdio.h>
#include <unistd.h>

#include "init.h"

int main (int argc, char* argv[]) {
    if(init(argc, argv)) {
        return 0;
    }
    return 0;
}

