/*
 * Author: number
 * Date  : Nov 10, 2015
 */

#ifndef __SERVER_COMMON_DATA_H_H___
#define __SERVER_COMMON_DATA_H_H___

#include <stdint.h>
#include <time.h>

namespace ef {

enum PollerFdType{
    kTcpListenFd    = 0,      // accept连接,并添加到poller继续监听
    kTcpClientFd    = 1,      // read数据,并路由到worker

    kUdpListenFd    = 2,      // accept连接,并添加到poller继续监听
    kUdpClientFd    = 3,      // read数据,并路由到worker

    kUnixListenFd   = 4,      // accept连接,并添加到poller继续监听
    kUnixCLientFd   = 5,      // read数据,并路由数据路由到worker

    kTaskPoolFd     = 6,      // woker处理回包,将接收到的数据发送到对应的client,client对应的fd由TaskData保存
    kTimerFd        = 7,      // TimerFd,定时任务,执行对应的定时任务
    kExternServerFd = 8,      // woker访问外部server,iohandler需要将回包送到指定的woker
};


/**
 * @desc IOHander通过poller进行监听的文件描述符及相关数据
 *
 * @ext 当描述符为socket时,标记
 *       fd_type为kExternSeverFd时使用,用于标记使用的worker的ID
 *       fd_type为kTimerFd时,记录对应的timer执行的回调函数
 *       否则,保存上次路由的worker的ID,用于数据均衡分发
 */
struct FdInfo {
    int        fd               = -1;
    uint32_t   fd_type          = -1;           //   PollerFdType
    uint32_t   extern_ip        = -1;
    uint16_t   extern_port      = -1;
    uint16_t   local_port       = -1;
    void       *ext             = nullptr;
    time_t     last_active_time = 0;
};


enum DataType {
    kTcpData     = 0,
    kUdpData     = 1,
    kUnixData    = 2,
};


enum Cmd {
    // io handler to worker
    kIoToWorkerReq          = 0,
    kIoToWorkerRsp          = 1,
    kIoToWorkerNotify       = 2,

    // worker to io handler
    kWorkerToIoRsp          = 3,
    kWorkerToIoVisitExt     = 4,

    // control to me
    kControlStop            = 5,
};


/**
 * @desc woker和iohand进行交互的数据,需要确定到唯一的handler及数据相关的文件描述符
 *       具体包括如下的功能:
 *          1. iohander通知woker有外部(1. client请求 2. 外部server回包)数据
 *          2. iohandler通知worker连接变动相关信息
 *          3. worker通知iohandler对client请求回包
 *          4. worker通知iohandler访问外部server
 */
struct TaskData {
    int      fd_id;         // io handler发送给worker的数据,worker回包时确定发送到哪个client

    uint16_t headler_id;    // 1. 当io handler给worker发数据时,io handler id,用于worker回包
                            // 2. 当worker请求外部server时,worker id,io handler返回外部回包到指定worker
    uint16_t data_type;
    uint16_t cmd;

    uint16_t  connect_id;    // worker连接外部server使用,不同的connect_id决定了不同的连接
    uint32_t extern_ip;
    uint16_t extern_port;
    uint16_t local_port;

    uint32_t msg_len;
    char msg[0];
};

}  /* namespace ef */

#endif /* __SERVER_COMMON_DATA_H__ */
