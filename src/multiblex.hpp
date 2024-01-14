#include "Request.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>

#ifndef multiblex_hpp
#define multiblex_hpp

#ifndef PORT
#define PORT 8000
#endif

#ifndef MAX_EVENTS
#define MAX_EVENTS 10
#endif


class multiblex
{
string respons;
public:
    struct sockaddr_in address;
    struct epoll_event ev, events[MAX_EVENTS];

    int addrlen;
    int listen_sock;
    multiblex();
    void m_server();
    void do_use_fd(int con_sockit, Request& req);
    ~multiblex();
};




#endif