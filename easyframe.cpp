/*
 * Author: number
 * Date  : Nov 1, 2015
 */

#include <stdio.h>
#include <stdlib.h>

#include "common/mem_pool.h"
#include "common/poller.h"

int main(void) {
    ef::Poller poller;
    ef::MemPool mem_pool(128, 1280, 128);
    puts("Hello World!");
    return 0;
}
