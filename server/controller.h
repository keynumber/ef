/*
 * Author: number
 * Date  : Nov 8, 2015
 */

#ifndef __SERVER_CONTROLLER_H_H___
#define __SERVER_CONTROLLER_H_H___

#include <cstdint>
#include <string>

#include "default_configure.h"

namespace ef {

class  TaskPool;
class  IoHandler;
class  Worker;
class Poller;

class Controller {
public:
    Controller();
    virtual ~Controller();

    Controller(const Controller &) = delete;
    Controller & operator= (const Controller&) = delete;

    bool Initialize(const char *server_name, const char *configure_file);
    void Run();

    const std::string & GetErrMsg() const;

private:
    int ListenTcpV4(unsigned short port);
    void FillErrmsg(const char * extra, int errorno);

    // 每个IoHandler处理自己的文件描述符,通过对IoHandler的个数取模得到IoHandler的ID
    // 让(fd % io_handler_num)作为处理该fd的IoHandler
    // 让(fd / io_handler_num)作为fd在IoHandler中的下下标,
    // 从而得以充分利用每个空间
    void SelectIoHandler(int fd, uint32_t *io_handler_id, uint32_t *fd_id);

private:
    struct ListenFdInfo {
        int fd = -1;
        unsigned short port = 0;
        ListenFdInfo() = default;
        virtual ~ListenFdInfo();
    };
private:
    std::string _server_name;
    std::string _configure_file;

    ListenFdInfo *_listen_sockets = nullptr;
    uint32_t _listen_size = 0;

    Poller *_poller = nullptr;

    // 这块没有区分handler和worker TODO
    TaskPool *_task_pool = nullptr;
    uint32_t _io_handler_num = 0;
    IoHandler *_io_handers = nullptr;
    uint32_t _worker_num = 0;
    Worker *_workers = nullptr;

    std::string _errmsg;
};

} /* namespace ef */

#endif /* __SERVER_CONTROLLER_H__ */
