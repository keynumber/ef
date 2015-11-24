/*
 * Author: number
 * Date  : Nov 8, 2015
 */

#ifndef __SERVER_WORKER_H_H___
#define __SERVER_WORKER_H_H___

namespace ef {

class Worker {
public:
    Worker();
    virtual ~Worker();

    void HandlerRequest(void * task);

    bool Initialize();
};

} /* namespace ef */

#endif /* __SERVER_WORKER_H__ */
