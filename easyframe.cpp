/*
 * Author: number
 * Date  : Nov 1, 2015
 */

#include <stdio.h>
#include <stdlib.h>

#include "common/mem_pool.h"
#include "common/poller.h"
#include "common/logger.h"

int main(void) {
    ef::Poller poller;
    ef::MemPool mem_pool(128, 1280, 128);

    ef::Logger::Initialize("/home/number/test_logger", 1000, 5, ef::kLevelInfo);
    for (int i = 0; i < 100; ++i) {
        LOG.Info("this is a warn message: %d\n", i);
        LOG.Debug("this is a debug message: %d\n", i);
    }
    LOG.Err("this is a debug message: %13241235d\n");
    LOG.Err("this is a debug message: %d\n");
    // printf core 问题
    return 0;
}
