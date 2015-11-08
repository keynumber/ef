/*
 * Author: number
 * Date  : Nov 7, 2015
 */

#include "event_notifier.h"

#include <unistd.h>
#include <sys/eventfd.h>

#include "io_wrapper.h"
#include "macro.h"

namespace ef {

EventNotifier::EventNotifier() : _fd(-1)
{
}

EventNotifier::~EventNotifier()
{
    if (_fd >= 0) {
        safe_close(_fd);
    }
}

bool EventNotifier::Initialize(bool blocked, bool edge_trigger)
{
    int flags = EFD_CLOEXEC;
    if (!blocked) {
        flags = flags | EFD_NONBLOCK;
    }

    if (!edge_trigger) {
        flags = flags | EFD_SEMAPHORE;
    }

    _fd = eventfd(0, flags);
    if (_fd < 0) {
        return false;
    }
    return true;
}

bool EventNotifier::Notify(uint64_t num)
{
    ssize_t ret = safe_write(_fd, &num, sizeof(uint64_t));
    return ret == sizeof(uint64_t);
}

int64_t EventNotifier::GetEvent()
{
    uint64_t num;
    ssize_t ret = safe_read(_fd, &num, sizeof(uint64_t));
    if (likely(ret == sizeof(uint64_t)))
        return static_cast<int64_t>(num);
    return -1;
}

int EventNotifier::GetEventFd() const
{
    return _fd;
}

} /* namespace ef */



#if 0

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    ef::EventNotifier ev_nbl;
    ev_nbl.Initialize(false, false);
    cout << "ev with nonblock and level trigger, nodify 12 event" << endl;
    ev_nbl.Notify(12);

    for (int i = 0; i < 15; ++i) {
        cout << "loop " << i << " get events num " <<ev_nbl.GetEvent() << endl;
    }

    ef::EventNotifier ev_nbe;
    ev_nbe.Initialize(false, true);
    cout << "ev with block and edge trigger, nodify 12 event" << endl;
    ev_nbe.Notify(12);

    for (int i = 0; i < 15; ++i) {
        cout << "loop " << i << " get events num " <<ev_nbe.GetEvent() << endl;
    }

    ef::EventNotifier ev;
    ev.Initialize(true, false);
    cout << "ev with block and level trigger, nodify 12 event" << endl;
    ev.Notify(12);

    for (int i = 0; i < 15; ++i) {
        cout << "loop " << i << " get events num " <<ev.GetEvent() << endl;
    }
    return 0;
}

#endif
