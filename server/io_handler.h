/*
 * Author: number
 * Date  : Nov 8, 2015
 */

#ifndef __SERVER_IO_HANDLER_H_H___
#define __SERVER_IO_HANDLER_H_H___

#include "common/poller.h"

namespace ef {

class IoHandler {
public:
    IoHandler();
    virtual ~IoHandler();

    IoHandler(const IoHandler &) = delete;
    IoHandler(const IoHandler &&) = delete;
    IoHandler operator = (const IoHandler &) = delete;
    IoHandler operator = (const IoHandler &&) = delete;

private:
    ef::Poller _poller;
};

} /* namespace ef */

#endif /* __SERVER_IO_HANDLER_H__ */
