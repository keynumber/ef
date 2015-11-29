/*
 * Author: number
 * Date  : Nov 8, 2015
 */

#ifndef __SERVER_IO_HANDLER_H_H___
#define __SERVER_IO_HANDLER_H_H___

#include <cassert>
#include <unordered_set>

#include "common_data.h"
#include "common/poller.h"
#include "common/c_map.h"
#include "common/mem_pool.h"
#include "common/socket_buffer.h"
#include "common/io_wrapper.h"
#include "default_configure.h"

namespace ef {

extern CMap gConfigureMap;
extern MemPool gMemPool;

class IoHandler {
public:
    IoHandler();
    virtual ~IoHandler();

    bool Initialize(int handler_id);
    void Run();

    bool HanderClient();    // client请求
    bool HanderWorker();    // worker的请求外部server
    bool HanderController();    // controller的接收client连接或者控制
    bool HanderTaskPool();    // controller的请求

    void ClearUnuseConnection();        // 清除长时间没有使用的连接

    IoHandler(const IoHandler &) = delete;
    IoHandler(const IoHandler &&) = delete;
    IoHandler operator = (const IoHandler &) = delete;
    IoHandler operator = (const IoHandler &&) = delete;

private:
    enum PollerFdType{
        kTcpClientFd    = 1,      // read数据,并路由到worker
        kUdpClientFd    = 3,      // read数据,并路由到worker
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
        FdInfo() {
            uint32_t block_size = gConfigureMap["iohandle"]["buffer_block_size"].as<uint32_t>(DefaultConf::kBufferBlockSize);
            buffer = new SocketBuffer(&gMemPool, block_size);
            assert(buffer);
        }

        void Clear()
        {
            if (fd >= 0) {
                safe_close(fd);
            }

            if (buffer) {
                buffer->Clear();
            }
        }

        virtual ~FdInfo() {
            if (buffer) {
                delete buffer;
                buffer = nullptr;
            }

            if (fd >= 0) {
                safe_close(fd);
            }
        }

        int          fd               = -1;
        uint32_t     fd_type          = 0;           //   PollerFdType
        uint32_t     extern_ip        = 0;
        uint16_t     extern_port      = 0;
        uint16_t     local_port       = 0;
        void         *ext             = nullptr;
        time_t       last_active_time = 0;
        SocketBuffer *buffer        = nullptr;
    };

private:
    Poller *_poller;

    uint32_t _fds_size;
    FdInfo *_fds;

    uint32_t _read_buf_size;
    char *_read_buf;

    // 访问外部server,woker需要将包完整性检测函数传过来,放入ext中
    // listen socket接受的连接呢?也穿进来,放入ext


    // std::unordered_map<extern_server_addr, related_info> _extern_server;    // 外部server对应的包长度等

    std::string _errmsg;
};

} /* namespace ef */

#endif /* __SERVER_IO_HANDLER_H__ */
