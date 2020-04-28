#pragma once
#include <mutex>
#include "basicstream.h"

class EpollStream : public BasicStream
{
public:
    EpollStream(const int32_t nativeSocket);
    EpollStream(const EpollStream&) = delete;
    virtual ~EpollStream(){}

    int32_t Receive(int32_t &readedSize) override;
    void Send(const char* data, const int32_t length) override;
    virtual int32_t SendSome(bool &finished);

private:
    std::mutex _dataMtx;
    char *_readBuffer;
    const int RMAXLENGTH = 1024;
    std::string _data;
    uint64_t _writePos;
};