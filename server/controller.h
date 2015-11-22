/*
 * Author: number
 * Date  : Nov 8, 2015
 */

#ifndef __SERVER_CONTROLLER_H_H___
#define __SERVER_CONTROLLER_H_H___

#include <cstdint>
#include <string>

#include "common_data.h"
#include "default_configure.h"

namespace ef {

class Controller {
public:
    Controller();
    virtual ~Controller();

    bool Initialize(const char *server_name, const char *configure_file);
    void Run();

    const std::string & GetErrMsg() const;
private:
    int ListenTcpV4(unsigned short port);
    void FillErrmsg(const char * extra, int errorno);

private:
    std::string _server_name;
    std::string _configure_file;

    ListenInfo *_listen_sockets;
    uint32_t _listen_size;

    std::string _errmsg;
};

} /* namespace ef */

#endif /* __SERVER_CONTROLLER_H__ */
