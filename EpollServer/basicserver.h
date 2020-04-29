#pragma once
#include "socket.h"
#include <string>

template<class ConnectionType>
class BasicServer : public Socket
{
public:
    BasicServer() = default;
    virtual int32_t Listen(const std::string &host, const int32_t port, const int backlog) = 0;
    virtual ConnectionType Accept(const int32_t listerfd) = 0;
    virtual void SetNewConnectCB(std::function<void(std::shared_ptr<BasicStream>)> cb) = 0;
};