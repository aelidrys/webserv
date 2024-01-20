#include "Request.hpp"

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
map<int, Request> client;
int epollfd;
ssize_t gw_len;
public:
    struct sockaddr_in address;
    struct epoll_event ev, events[MAX_EVENTS];

    int addrlen;
    int listen_sock;
    multiblex();
    void m_server();
    void do_use_fd(int con_sockit, int n);
    void use_clinet_fd(int con_sockit, int n);
    void add_client();
    ~multiblex();
};




#endif