/*
 * Author: number
 * Date  : Nov 8, 2015
 */

#include "io_handler.h"

namespace ef {

IoHandler::IoHandler()
{
}

IoHandler::~IoHandler()
{
}

bool IoHandler::Initialize()
{
    // 创建监听文件描述符
    // 初始化日志
    // 初始化统计
    // 初始化epoll
    // 添加监听文件描述符到epoll
    // epoll_wait

    return true;
}

} /* namespace ef */
