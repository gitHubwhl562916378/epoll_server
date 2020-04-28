#pragma once
#include <memory>
#include "epollstream.h"
#include "basicserver.h"

class EpollServer : public BasicServer<std::shared_ptr<EpollStream>>
{
public:
    EpollServer() = default;
    int32_t Listen(const std::string &host, const int32_t port, const int backlog) override;
    std::shared_ptr<EpollStream> Accept(const int32_t listerfd) override;
    void Send(const std::string &bytes, const std::vector<std::shared_ptr<BasicStream>> &streams = std::vector<std::shared_ptr<BasicStream>>());

protected:
    int32_t _Bind(const std::string &host, const int port);
};