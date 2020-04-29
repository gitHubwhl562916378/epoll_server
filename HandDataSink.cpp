#include <string>
#include "EpollServer/socket.h"
#include "HandDataSink.h"

void HandDataSink::SetDataCallBack(std::function<void(const std::string &, const Socket*)> cb)
{
    data_cb_ = cb;
}

void HandDataSink::onDataIndication(const char *buf, const int nbytes, const Socket *s)
{
    if(data_cb_)
    {
        std::string msg(buf, nbytes);
        data_cb_(msg, s);
    }
}