#include "multiblex.hpp"

multiblex::multiblex(){

    addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    hello = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 12\r\n\r\nHello world!\n";
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
}


void setnonblocking(int con_sockit){
    fcntl(con_sockit, F_SETFL , fcntl(con_sockit, F_GETFL, 0) | O_NONBLOCK);
}

void multiblex::do_use_fd(int con_sockit, request& req){
    size_t buff_size = 1024;
    char buff[buff_size];
    ssize_t read_size = read(con_sockit, buff, buff_size-1);
    if (read_size == 0){
        close(con_sockit);
        cout << "con_sockit closed" << endl;
        return ;
    }
    if (read_size == -1){
        close(con_sockit);
        cerr << "Chehaja Mahyach Had Le3jeb Fila" << endl;
        return ;
    }
    buff[read_size] = '\0';
    printf("LENTH = %zu \n>>------ Hadchi Lireadina ------<<\n", read_size);
    printf("%s\n<<---------- Safi Rah Sala -------->>\n\n", buff);
    req.parce_req(string("").append(buff, read_size));
}

void multiblex::m_server(){

    int conn_sock, epollfd;
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

    request req;
    int nfds;
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
                ev.events = EPOLLIN | EPOLLOUT;
                ev.data.fd = conn_sock;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,&ev) == -1) {
                    perror("epoll_ctl: conn_sock");
                    exit(EXIT_FAILURE);
                }
            } 
            else {
                if (events[n].events & EPOLLIN){
                    cout << "-------- do_use_fd Bdat --------"<<endl;
                    do_use_fd(events[n].data.fd, req);
                    cout << "------------ do_use_fd Salat -----------\n"<<endl;
                }    
                else if (events[n].events & EPOLLOUT && req.req_done()){
                    cout << "Nchofo Had req_done Chehal :"<<req.req_done()<<endl;
                    cout << "Nchofo Had fd Chehal :"<<events[n].data.fd<<endl;
                    write(events[n].data.fd, hello.c_str(), hello.size());
                    close(events[n].data.fd);
                    req.show_inf();
                }
            }
        }
    }
}


multiblex::~multiblex(){
    ;
}

