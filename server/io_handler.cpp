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

bool IoHandler::Initialize()
{
    _read_buf_size = gConfigureMap["iohandler"]["read_buf_size"].as<uint32_t>(DefaultConf::kReadBufferSize);
    _read_buf = new char[_read_buf_size];
    assert(_read_buf);

    // poller的参数错了........poller的参数应该时一次最大返回的事件数量
    int max_client_num = gConfigureMap["iohandler"]["max_client_num"].as<int>(DefaultConf::kMaxClientNum);
    assert(max_client_num > 0);
    _poller = new Poller(max_client_num);
    assert(_poller);

    _fds_size = max_client_num + 64;
    _fds = new FdInfo[_fds_size];
    assert(_fds);

    // TODO task pool, iohandler and worker


    return true;
}

void IoHandler::Run()
{
    while (true) {
    }
}

} /* namespace ef */
