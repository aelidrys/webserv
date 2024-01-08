#include "request.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>

#ifndef PORT
#define PORT 8080
#endif

#define MAX_EVENTS 10
struct epoll_event ev, events[MAX_EVENTS];
int listen_sock, conn_sock, nfds, epollfd;

/* Code to set up listening socket, 'listen_sock',
    (socket(), bind(), listen()) omitted. */

void setnonblocking(int con_sockit){
    // fcntl(con_sockit, F_SETFL , O_NONBLOCK);
    fcntl(con_sockit, F_SETFL , fcntl(con_sockit, F_GETFL, 0) | O_NONBLOCK);
}

void do_use_fd(int con_sockit){
    request req;
    char buff[1024];
    ssize_t read_size = read(con_sockit, buff, 1024);
    if (read_size == 0){
        close(con_sockit);
        std::cout << "con_sockit closed" << std::endl;
    }
    if (read_size == -1){
        close(con_sockit);
        return ;
    }
    buff[read_size] = '\0';
    std::string hello = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 12\r\n\r\nHello world!\n";
    write(con_sockit, hello.c_str(), hello.size());
    printf("----------->   :    %s\n", buff);
    printf("lenth   :    %zu\n\n\n", strlen(buff));
    req.parce_req(buff);
    req.show_inf();

}

void m_server(){


    struct sockaddr_in address;
    int addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );


    if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    int reuseaddr = 1;
    if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1) {
        perror("setsockopt");
        close(listen_sock);
        exit(EXIT_FAILURE);
    }
    memset(address.sin_zero, '\0', sizeof(address.sin_zero));
    if (bind(listen_sock, (struct sockaddr *)&address, sizeof(address))<0){
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(listen_sock, 10) < 0){
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    epollfd = epoll_create1(0);
    if (epollfd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = listen_sock;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
        perror("epoll_ctl: listen_sock");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == listen_sock) {
                conn_sock = accept(listen_sock,(struct sockaddr *)&address, (socklen_t *)&addrlen);
                if (conn_sock == -1) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
                // setnonblocking(conn_sock);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_sock;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,
                            &ev) == -1) {
                    perror("epoll_ctl: conn_sock");
                    exit(EXIT_FAILURE);
                }
            } else {
                do_use_fd(events[n].data.fd);
            }
        }
    }

}


