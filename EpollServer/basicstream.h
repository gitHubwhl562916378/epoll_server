#pragma once
#include "istream.h"
#include "socket.h"

class BasicStream : public IStream, public Socket
{
public:
    BasicStream() = default;
    BasicStream(const int32_t nativeSocket) : Socket(nativeSocket){}

    void SetDataSink(DataSink* sink) override{
        _dataSink = sink;
    }

    DataSink* GetDataSink() override{
        return _dataSink;
    }

private:
    DataSink *_dataSink;
};