/*
 * Author: number
 * Date  : Nov 1, 2015
 */

#include <stdio.h>

#include "server/controller.h"

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        printf("usage: %s configure_file\n", argv[0]);
        return -1;
    }

    ef::Controller controller;
    controller.Initialize("echosvr", argv[1]);

    return 0;
}
