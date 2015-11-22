/*
 * Author: number
 * Date  : Nov 8, 2015
 */

#include "controller.h"

#include <cstdio>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "common/util.h"
#include "common/c_map.h"
#include "common/io_wrapper.h"
#include "common/logger.h"
#include "common/mem_pool.h"

namespace ef {

CMap gConfigureMap;
MemPool gMemPool;

Controller::Controller() : _listen_sockets(nullptr), _listen_size(0)
{
}

Controller::~Controller()
{
    if (_listen_sockets) {
        for (uint32_t i = 0; i < _listen_size; ++i) {
            safe_close(_listen_sockets[i].fd);
        }

        delete [] _listen_sockets;
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

    // 初始化监听端口
    const Value & val = gConfigureMap["listen"];
    _listen_size = val.size();
    _listen_sockets = new ListenInfo[_listen_size];
    for (uint32_t i=0; i<_listen_size; ++i)
    {
        _listen_sockets[i].port = val[i]["port"].as<unsigned>(0);
        _listen_sockets[i].fd = ListenTcpV4(_listen_sockets[i].port);
        if (_listen_sockets[i].fd < 0) {
            return false;
        }
    }

    // initialize io handler and worker

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
    sleep(10000000);
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

} /* namespace ef */
