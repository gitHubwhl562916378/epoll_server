#pragma once
#include <stdint.h>
#include <unistd.h>

class Socket
{
public:
    Socket():_nativeSocket(0){}
    Socket(const int32_t nativeSocket):_nativeSocket(nativeSocket){}
    virtual ~Socket(){close(_nativeSocket);}

    int32_t GetNativeSocket() const {return _nativeSocket;}

protected:
    void SetNativeSocket(int32_t fd){_nativeSocket = fd;}
    
private:
    int32_t _nativeSocket;
};