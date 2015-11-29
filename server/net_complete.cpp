#include <stdio.h>

extern "C" {

int net_complete_func(const char * buf) {
    puts("net_complete_func in net_complete.so");
    return 0;
}

}
