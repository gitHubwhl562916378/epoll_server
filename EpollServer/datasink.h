#pragma once

class Socket;
class DataSink
{
public:
    virtual ~DataSink(){}
    virtual void onDataIndication(const char *buf, const int nbytes, const Socket *) = 0;
};