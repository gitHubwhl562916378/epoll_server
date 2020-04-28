#pragma once
#include <string>
#include <stdint.h>

class DataSink;
class IStream
{
public:
    virtual ~IStream(){}
    virtual int32_t Receive(int32_t &readedSize) = 0;
    virtual void Send(const char* data, const int32_t length) = 0;

protected:
    virtual void SetDataSink(DataSink* sink) = 0;
    virtual DataSink* GetDataSink() = 0;
};