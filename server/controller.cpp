/*
 * Author: number
 * Date  : Nov 8, 2015
 */

#include "controller.h"

#include <cassert>
#include <cstring>
#include <cstdio>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "common/c_map.h"
#include "common/io_wrapper.h"
#include "common/logger.h"
#include "common/mem_pool.h"
#include "common/poller.h"
#include "common/task_pool.h"
#include "common/util.h"
#include "common_data.h"
#include "debug.h"
#include "io_handler.h"
#include "worker.h"

namespace ef {

CMap gConfigureMap;
MemPool gMemPool;

Controller::ListenFdInfo::~ListenFdInfo() {
    if (fd >= 0) {
        safe_close(fd);
    }
}

Controller::Controller()
{
}

Controller::~Controller()
{
    // TODO
    if (_listen_sockets) {
        delete [] _listen_sockets;
        _listen_sockets = nullptr;
    }
}

bool Controller::Initialize(const char* server_name, const char* configure_file)
{
    _server_name = server_name;
    _configure_file = configure_file;

    // 加载配置
    if (!gConfigureMap.Load(configure_file))
    {
        printf("error: %s\n", gConfigureMap.GetErrMsg().c_str());
        return false;
    }
    gConfigureMap.Print();

    // 初始化日志
    std::string log_path = JoinPath(gConfigureMap["log"]["log_path"].str(), server_name);
    if (!Logger::Initialize(log_path.c_str(),
                           gConfigureMap["log"]["max_file_size"].as<uint32_t>(DefaultConf::kMaxLogFileSize),
                           gConfigureMap["log"]["max_file_num"].as<uint32_t>(DefaultConf::kMaxLogFileNum),
                           gConfigureMap["log"]["log_level"].c_str())) {
        printf("logger initialize failed, errmsg: %s\n", Logger::GetErrMsg().c_str());
    }

    LogInfo("Logger initialize succeed\n");

    int poller_max_event = gConfigureMap["poller_max_event"].as<int>(DefaultConf::kPollerMaxEventNum);
    assert(poller_max_event > 0);
    _poller = new Poller(poller_max_event);
    assert(_poller);

    LogInfo("Poller initialize succeed\n");

    // 初始化监听端口

    // TODO different port bind different protocol(unpack function)
    // 每个监听端口最好能独立的包完整性检查,每个访问外部server的端口,最好也能绑定独立协议
    const Value & val = gConfigureMap["listen"];
    _listen_size = val.size() + 128;
    _listen_sockets = new ListenFdInfo[_listen_size];
    for (uint32_t i=0; i<_listen_size; ++i)
    {
        unsigned int port = val[i]["port"].as<unsigned>(0);
        int fd = ListenTcpV4(port);
        if (fd < 0) {
            FillErrmsg("listen failed: ", errno);
            return false;
        }

        _listen_sockets[fd].port = port;
        _listen_sockets[fd].fd = fd;

        if (_poller->Add(fd, (uint64_t)fd, EPOLLIN)) {
            FillErrmsg("listen fd add to poller failed: ", errno);
            return false;
        }
    }

    _io_handler_num = gConfigureMap["server"]["io_handler_num"].as<uint32_t>(DefaultConf::kIoHandlerNum);
    _io_handers = new IoHandler[_io_handler_num];
    assert(_io_handers);
    for (uint32_t i=0; i<_io_handler_num; ++i) {
        if(!_io_handers[i].Initialize()) {
            _errmsg = "io handler initialize failed";
            return false;
        }
    }

    _worker_num = gConfigureMap["server"]["worker_num"].as<uint32_t>(DefaultConf::kWorkerNum);
    _workers = new Worker[_worker_num];
    assert(_workers);
    for (uint32_t i=0; i<_worker_num; ++i) {
        if(!_workers[i].Initialize()) {
            _errmsg = "initialize workers failed";
            return false;
        }
    }

    int pool_size = _io_handler_num + _worker_num;
    int queue_size = gConfigureMap["server"]["queue_size"].as<uint32_t>(DefaultConf::kQueueSize);
    _task_pool = new TaskPool();
    assert(_task_pool);
    if (!_task_pool->Initialize(pool_size, queue_size)) {
        LogErr("task poll initialize failed");
        return false;
    }

    return true;
}

int Controller::ListenTcpV4(unsigned short port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        FillErrmsg("create listen socket failed, errmsg: ", errno);
        return -1;
    }

    if (set_nonblock(sock) < 0) {
        FillErrmsg("set listen socket nonblocked failed, errmsg: ", errno);
        safe_close(sock);
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) == -1)
    {
        FillErrmsg("bind failed, errmsg: ", errno);
        safe_close(sock);
        return -1;
    }

    // TODO socketopt


    int listen_backlog = gConfigureMap["socket_configure"]["listen_back_log"].as<int>(DefaultConf::kListenBackLog);
    if (listen(sock, listen_backlog)) {
        safe_close(sock);
        return -1;
    }
    return sock;
}

void Controller::Run()
{
    while(true) {
        int num = _poller->Wait(-1);
        while (num--) {
            int listen_fd;
            uint64_t u64;
            uint32_t events;
            _poller->GetEvent(&u64, &events);
            if (events != EPOLLIN) {
                continue;
            }

            listen_fd = static_cast<int>(u64);

            // TODO 目前暂时只支持TCP

            struct sockaddr_in  socket_addr;
            socklen_t socket_addr_len = sizeof(sockaddr_in);
            int client = accept(listen_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);

            // TODO 如果是大量短链接的话,比较.......
            TaskData * task = static_cast<TaskData*>(gMemPool.Malloc(sizeof(TaskData)));
            if (!task) {
                LogErr("alloc task data failed, close client connection, client address: %u:%u\n",
                        socket_addr.sin_addr.s_addr, socket_addr.sin_port);
                safe_close(client);
                continue;
            }

            uint32_t io_handler_id;
            uint32_t client_id;
            SelectIoHandler(client, &io_handler_id, &client_id);

            task->cmd = kControlAddClient;
            task->data_type = kTcpData;
            task->extern_ip = socket_addr.sin_addr.s_addr;
            task->extern_port = socket_addr.sin_port;
            task->local_port = _listen_sockets[listen_fd].port;
            task->fd_id = client_id;
            task->headler_id = io_handler_id;

            LOGDEBUG("accept client connection: %u:%u", task->extern_ip, task->extern_port);

            if (!_task_pool->Put(io_handler_id, static_cast<void*>(task))) {
                LogInfo("add task to pool failed");
                safe_close(client);
                continue;
            }
        }
    }
}

bool Controller::LoadNetCompleteFunc()
{
    return true;
}

void Controller::FillErrmsg(const char * extra, int errorno)
{
    char buf[256];
    safe_strerror(errorno, buf, 256);
    _errmsg = extra;
    _errmsg.append(buf);
}

const std::string& Controller::GetErrMsg() const
{
    return _errmsg;
}

void Controller::SelectIoHandler(int fd, uint32_t *io_handler_id, uint32_t *fd_id)
{
    *io_handler_id = (uint32_t)fd % _io_handler_num;
    *fd_id = (uint32_t)fd / _io_handler_num;
}

} /* namespace ef */
