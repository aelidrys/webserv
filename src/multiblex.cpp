#include "multiblex.hpp"

multiblex::multiblex(){
    epollfd = epoll_create(255);
    if (epollfd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
}

void multiblex::add_client(int listen_sock){
    address = pconf.msockets[listen_sock].address;
    addrlen = sizeof(pconf.msockets[listen_sock].address);
    int conn_sock;
    conn_sock = accept(listen_sock,(struct sockaddr *)&(address), (socklen_t *)&addrlen);
    if (conn_sock == -1) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    client[conn_sock] = Request(pconf.msockets[listen_sock]);
    ev.events = EPOLLIN | EPOLLOUT | EPOLLHUP;
    ev.data.fd = conn_sock;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,&ev) == -1) {
        perror("epoll_ctl: conn_sock");
        exit(EXIT_FAILURE);
    }
}

int multiblex::read_from_sockit(int sockit){
    ssize_t read_size;

    buffer.resize(1000);
    read_size = read(sockit, &buffer[0], 1000);
    if (read_size == -1){
        cerr << "Shehaja mahyach read filat!!" << endl;
        close(sockit);
        return -1;
    }
    buffer.resize(read_size);
    if (read_size == 0){
        return 0;
    }
    return read_size;
}

void multiblex::in_event(int con_sockit, int event){
    ssize_t read_size;

    read_size = read_from_sockit(con_sockit);
    if (read_size <= 0)
        return;
    client[con_sockit].process_req(buffer, read_size, event);
    respons = client[con_sockit].get_respons();
    write(con_sockit, respons.c_str(), respons.size());
    if (client[con_sockit].uri == "/favicon.ico"){
        string res = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 11\r\n\r\nFavicon Res";
        cout<<"favicon fd: "<<con_sockit<<" closed!"<<endl;
        write(con_sockit, res.c_str(), res.size());
        stop_conection(con_sockit);
    }
}

void multiblex::use_clinet_fd(int con_sockit, int n){

    if (events[n].events & EPOLLHUP){
        stop_conection(con_sockit);
        return;
    }
    if (events[n].events & EPOLLIN){
        // process Request with in mode
        in_event(con_sockit, EPOLLIN);
    }
    else if (client[con_sockit].body_state && events[n].events & EPOLLOUT){
        client[con_sockit].process_req("",0,EPOLLOUT);
        respons = client[con_sockit].get_respons();
        write(con_sockit, respons.c_str(), respons.size());
        if (client[con_sockit].method && client[con_sockit].method->end)
            stop_conection(con_sockit);
    }
}

void multiblex::m_server(){

    pconf.TakeAndParce("webserv.conf");
    map<int,Servers>::iterator it;
    for (it = pconf.msockets.begin(); it != pconf.msockets.end(); it++)
    {
        ev.events = EPOLLIN;
        ev.data.fd = it->first;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, it->first, &ev) == -1) {
            perror("epoll_ctl: listen_sock");
            exit(EXIT_FAILURE);
        }
    }

    int nfds;
    while (1) {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }
        for (int n = 0; n < nfds; ++n) {
            if (pconf.msockets.find(events[n].data.fd) != pconf.msockets.end()) 
                add_client(events[n].data.fd);
            else
                use_clinet_fd(events[n].data.fd, n);
        }
    }
}

 void multiblex::stop_conection(int sockit){
    epoll_ctl(epollfd, EPOLL_CTL_DEL, sockit, &ev);
    close(sockit);
    client.erase(sockit);
    cout<<"coniction with client: "<<sockit<<" end"<<endl;
 }

multiblex::~multiblex(){
    ;
}
