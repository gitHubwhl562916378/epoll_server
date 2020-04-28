#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
#include "epollloop.h"
#include "epollserver.h"

int32_t EpollServer::Listen(const std::string &host, const int32_t port, const int backlog)
{
    int32_t errorCode = _Bind(host, port);
    if(errorCode < 0)
    {
        return errorCode;
    }

    errorCode = ::listen(GetNativeSocket(), backlog);
    if(-1 == errorCode)
    {
        std::cout << "Listen socket failed code " << errorCode << std::endl;
        return errorCode;
    }

    errorCode = EpollLoop::Get()->AddEpollEvents(EPOLLIN | EPOLLET, this);
    if(-1 == errorCode)
    {
        std::cout << "Add EpollServer to Epoll failed" << std::endl;
        return errorCode;
    }

    EpollLoop::Get()->AddServer(GetNativeSocket(), this);
    return errorCode;
}

std::shared_ptr<EpollStream> EpollServer::Accept(const int32_t listerfd)
{
    int32_t conn_sock;
    int32_t addrlen;
    sockaddr_in remote;

    int32_t listenfd = GetNativeSocket();
    while ((conn_sock = ::accept(listenfd,(sockaddr*)&remote,(socklen_t*)&addrlen)) > 0) {
        ::fcntl(conn_sock,F_SETFL,O_NONBLOCK);
        std::shared_ptr<EpollStream> connection = std::make_shared<EpollStream>(conn_sock);
        EpollLoop::Get()->AddEpollEvents(EPOLLIN | EPOLLET, connection.get());
        std::cout << "create new connection " << conn_sock << std::endl;

        return connection;
    }

    if(-1 == conn_sock){
        if(errno != EAGAIN && errno != ECONNABORTED && errno != EPROTO && errno != EINTR)
            std::cout << "error accept" << std::endl;
    }

    return nullptr;
}

void EpollServer::Send(const std::string &bytes, const std::vector<std::shared_ptr<BasicStream>> &streams)
{
    EpollLoop::Get()->Send(bytes, streams);
}

int32_t EpollServer::_Bind(const std::string &host, const int port)
{
    int32_t listenfd;
    if((listenfd = ::socket(AF_INET,SOCK_STREAM, 0)) < 0)
    {
        std::cout << "Create socket failed" << std::endl;
        ::exit(-1);
    }

    SetNativeSocket(listenfd);
    int32_t option = 1;
    ::setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    ::fcntl(listenfd, F_SETFL, O_NONBLOCK);

    sockaddr_in addr;
    ::bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = ::htons(port);
    addr.sin_addr.s_addr = ::inet_addr(host.data());

    int32_t errorCode = ::bind(listenfd, (sockaddr*)&addr, sizeof(addr));
    if(errorCode < 0)
    {
        std::cout << "Bin Socket failed, code " << errorCode << std::endl;
    }

    return errorCode;
}