#pragma once
#include <sys/epoll.h>
#include <memory>
#include <map>
#include <vector>
#include <atomic>
#include "loop.h"
#include "basicstream.h"

class EpollServer;
class EpollLoop : public Loop
{
public:
    static EpollLoop *Get();
    ~EpollLoop() override;

    void AddServer(int32_t fd, EpollServer *server);
    void AddStream( std::shared_ptr<BasicStream> stream);
    void Send(const std::string &bytes, const std::vector<std::shared_ptr<BasicStream>> &streams);

    int32_t AddEpollEvents(int32_t events, Socket *ptr);    //添加监听事件
    int32_t ModifyEpollEvents(int32_t events, Socket *ptr); // 更改监听事件

protected:
    EpollLoop();
    void _Run() override;

private:
    std::atomic_bool _shutdown;
    int32_t _eventfd;
    std::map<int32_t, EpollServer*> _servers;
    std::map<int32_t, std::shared_ptr<BasicStream>> _streams;

    void _Initalize();
    void _EpollThread();
    int32_t _Accept(const int32_t evfd, const int32_t listenfd);
};