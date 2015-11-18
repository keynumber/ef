/*
 *
 * Author: number
 * Date  : Nov 19, 2015
 */

#ifndef __COMMON_FD_HANDLER_H_H___
#define __COMMON_FD_HANDLER_H_H___

namespace ef {

enum FdType{
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


class FdHandler {
public:
    FdHandler();
    virtual ~FdHandler();

    int read(char *buf, int len);
    int write(const char *buf, int len);
    int listen(int ip, uint16_t port);
    int accept();
    int connect(int ip, uint16_t port);

private:
    FdType     _type;
    int        _fd;
    int        extern_ip        = -1;
    uint16_t   extern_port      = -1;
    uint16_t   local_port       = -1;
    time_t     last_active_time = 0;
    void       *ext;
};

} /* namespace ef */

#endif /* __COMMON_FD_HANDLER_H__ */
