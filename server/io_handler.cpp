/*
 * Author: number
 * Date  : Nov 8, 2015
 */

#include "io_handler.h"

namespace ef {


IoHandler::IoHandler()
    : _poller(nullptr)
    , _fds_size(0)
    , _fds(nullptr)
    , _read_buf_size(0)
    , _read_buf(nullptr)
{
}

IoHandler::~IoHandler()
{
    if (_fds) {
        delete [] _fds;
        _fds = nullptr;
    }

    if (_read_buf) {
        delete [] _read_buf;
        _read_buf = nullptr;
    }

    if (_poller) {
        delete _poller;
        _poller = nullptr;
    }
}

bool IoHandler::Initialize(const ListenInfo * listen_fds, uint32_t size)
{
    _read_buf_size = gConfigureMap["iohandler"]["read_buf_size"].as<uint32_t>(DefaultConf::kReadBufferSize);
    _read_buf = new char[_read_buf_size];
    assert(_read_buf);

    int max_client_num = gConfigureMap["iohandler"]["max_client_num"].as<uint32_t>(DefaultConf::kMaxClientNum);
    _poller = new Poller(max_client_num);
    assert(_poller);

    _fds_size = max_client_num + 64;
    _fds = new FdInfo[_fds_size];
    assert(!_fds);

    time_t now = time(nullptr);

    // TODO different port bind different protocol(unpack function)
    // 每个监听端口最好能独立的包完整性检查,每个访问外部server的端口,最好也能绑定独立协议


    // TODO worker fd

    for (int i = 0; i < size; ++i) {
        int fd = listen_fds[i].fd;
        _fds[fd].fd = fd;
        _fds[fd].local_port = listen_fds[i].port;
        _fds[fd].fd_type = kTcpListenFd;
        _fds[fd].last_active_time = now;

        _poller->Add(fd, static_cast<uint64_t>(fd), EPOLLIN);
    }

    return true;
}

void IoHandler::Run()
{
    while (true) {
    }
}

} /* namespace ef */
