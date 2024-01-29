#include "Request.hpp"

#ifndef multiblex_hpp
#define multiblex_hpp

#ifndef PORT
#define PORT 8000
#endif

#ifndef MAX_EVENTS
#define MAX_EVENTS 50
#endif


class multiblex
{
int addrlen;
int epollfd;
string buffer;
string respons;
struct sockaddr_in address;
struct epoll_event ev, events[MAX_EVENTS];
map<int, Request> client;
ParceConf pconf;
public:

    multiblex();
    void m_server();
    void in_event(int con_sockit, int n);
    void use_clinet_fd(int con_sockit, int n);
    void add_client(int listen_sock);
    int read_from_sockit(int sockit);
    void stop_conection(int sockit);
    ~multiblex();
};




#endif