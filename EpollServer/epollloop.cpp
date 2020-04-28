#include <errno.h>
#include <signal.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <thread>
#include "epollserver.h"
#include "epollloop.h"

EpollLoop *EpollLoop::Get()
{
    static EpollLoop epollLoop;
    return &epollLoop;
}

EpollLoop::~EpollLoop()
{
    _shutdown = true;
}

void EpollLoop::AddServer(int32_t fd, EpollServer *server)
{
    _servers.insert({fd, server});
}

void EpollLoop::AddStream(std::shared_ptr<BasicStream> stream)
{
    _streams.insert({stream->GetNativeSocket(), stream});
}

void EpollLoop::Send(const std::string &bytes, const std::vector<std::shared_ptr<BasicStream>> &streams)
{
    if (streams.empty())
    {
        std::for_each(_streams.begin(), _streams.end(), [&](std::pair<int32_t, std::shared_ptr<BasicStream> > value){
            value.second->Send(bytes.data(), bytes.size());
            ModifyEpollEvents(EPOLLIN | EPOLLOUT | EPOLLET, value.second.get());
        });
    }
    else
    {
        for(auto s : streams)
        {
            auto iter = _streams.find(s->GetNativeSocket());
            if(iter == _streams.end())
            {
                continue;
            }

            iter->second->Send(bytes.data(), bytes.size());
            ModifyEpollEvents(EPOLLIN | EPOLLOUT | EPOLLET, iter->second.get());
        }
    }
}

int32_t EpollLoop::AddEpollEvents(int32_t events, Socket *ptr)
{
    epoll_event ev;
    ev.events = events;
    ev.data.ptr = ptr;

    return ::epoll_ctl(_eventfd, EPOLL_CTL_ADD, ptr->GetNativeSocket(), &ev);
}

int32_t EpollLoop::ModifyEpollEvents(int32_t events, Socket *ptr)
{
    epoll_event ev;
    ev.events = events;
    ev.data.ptr = ptr;

    return ::epoll_ctl(_eventfd, EPOLL_CTL_MOD, ptr->GetNativeSocket(), &ev);
}

EpollLoop::EpollLoop()
{
    _shutdown = false;
    ::sigset_t set;
    ::sigemptyset(&set);
    ::sigaddset(&set, SIGPIPE);
    ::sigprocmask(SIG_BLOCK, &set, nullptr);
    _Initalize();
}

void EpollLoop::_Run()
{
    std::thread listenThread(std::bind(&EpollLoop::_EpollThread, this));
    listenThread.detach();
}

void EpollLoop::_Initalize()
{
    _eventfd = ::epoll_create1(0);
    if (-1 == _eventfd)
    {
        std::cout << "epoll_create failed" << std::endl;
        ::exit(-1);
    }
    _Run();
}

void EpollLoop::_EpollThread()
{
    std::cout << "Epoll thread" << std::endl;
    epoll_event events[1024];
    while (!_shutdown.load())
    {
        int32_t nfds;
        nfds = ::epoll_wait(_eventfd, events, 1024, -1);
        if(-1 == nfds)
        {
            std::cout << "FATAL epoll_wait failed" << std::endl;
            ::exit(EXIT_FAILURE);
        }

        std::cout << "event fd count " << nfds << std::endl;
        for(int i = 0; i < nfds; i++)
        {
            Socket *sptr = (Socket*)(events[i].data.ptr);
            std::cout << "event fd " << sptr->GetNativeSocket() << std::endl;
            if(_servers.find(sptr->GetNativeSocket()) != _servers.end())
            {
                _Accept(_eventfd, sptr->GetNativeSocket());
                continue;
            }

            EpollStream* stream = dynamic_cast<EpollStream*>(sptr);
            if(events[i].events & EPOLLIN)
            {
                std::cout << "stream " << stream->GetNativeSocket() << " Reading" << std::endl;
                int32_t readedSize;
                int32_t nread = stream->Receive(readedSize);

                if((nread == -1 && errno != EAGAIN) || 0 == readedSize)
                {
                    _streams.erase(stream->GetNativeSocket());
                    std::cout << "errno: " << errno << ' ' << strerror(errno) << " nread: " << nread << " bytes: " << readedSize << std::endl;
                }
            }

            if(events[i].events & EPOLLOUT)
            {
                std::cout << "stream " << stream->GetNativeSocket() << " Writting" << std::endl;
                bool isfinished = false;
                int32_t nwrite = stream->SendSome(isfinished);
                if((nwrite <= 0 && errno == EINTR) || !isfinished)
                {
                    ModifyEpollEvents(EPOLLIN | EPOLLOUT | EPOLLET, stream); //继续监听写事件
                }

                if(isfinished)
                {
                    ModifyEpollEvents(EPOLLIN | EPOLLET, stream);
                }
            }
        }
    }
    
}

int32_t EpollLoop::_Accept(const int32_t evfd, const int32_t listenfd)
{
    std::cout << "Accept " << listenfd << std::endl;
    EpollServer *server = _servers.find(listenfd)->second;
    std::shared_ptr<EpollStream> stream = server->Accept(evfd);

    if (!stream)
    {
        return -1;
    }

    _streams.insert({stream->GetNativeSocket(), stream});
    return 0;
}