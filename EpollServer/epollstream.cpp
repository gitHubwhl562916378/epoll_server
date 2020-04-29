#include <string.h>
#include "epollstream.h"
#include "datasink.h"

EpollStream::EpollStream(const int32_t nativeSocket) : BasicStream(nativeSocket)
{
    _readBuffer = new char[RMAXLENGTH];
}

int32_t EpollStream::Receive(int32_t &readedSize)
{
    readedSize = 0;
    int32_t nread = 0;

    while ((nread = ::read(GetNativeSocket(), _readBuffer, RMAXLENGTH)) > 0)
    {
        if (GetDataSink())
        {
            GetDataSink()->onDataIndication(_readBuffer, nread, this);
        }
        readedSize += nread;
    }

    return nread;
}

void EpollStream::Send(const char *data, const int32_t length)
{
    std::lock_guard<std::mutex> lock(_dataMtx);
    _data.append(data, length);
}

int32_t EpollStream::SendSome(bool &finished)
{
    int32_t writtenSize = 0;
    int32_t nwrite = 0;

    std::lock_guard<std::mutex> lock(_dataMtx);
    while ((nwrite = ::write(GetNativeSocket(), _data.data() + _writePos, _data.size() - _writePos)) > 0)
    {
        writtenSize += nwrite;
        _writePos += nwrite;
    }

    finished = (_writePos == _data.size());
    if (finished)
    {
        _writePos = 0;
        _data.clear();
    }

    return nwrite;
}