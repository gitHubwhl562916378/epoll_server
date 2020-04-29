#pragma once
#include <functional>
#include "EpollServer/datasink.h"
class HandDataSink : public DataSink
{
public:
    HandDataSink() = default;
    void SetDataCallBack(std::function<void(const std::string &, const Socket*)> cb);
    void onDataIndication(const char *buf, const int nbytes, const Socket *) override;

private:
    std::function<void(const std::string &, const Socket*)> data_cb_;
};