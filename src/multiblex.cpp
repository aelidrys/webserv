#include "multiblex.hpp"

multiblex::multiblex(){
    addrlen = sizeof(address);
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
}

void multiblex::add_client(){
    int conn_sock;
    conn_sock = accept(listen_sock,(struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (conn_sock == -1) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    cout<<"conn_sck = "<<conn_sock<<endl;
    client[conn_sock] = Request();
    ev.events = EPOLLIN | EPOLLOUT;
    ev.data.fd = conn_sock;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,&ev) == -1) {
        perror("epoll_ctl: conn_sock");
        exit(EXIT_FAILURE);
    }
}


void multiblex::do_use_fd(int con_sockit){
    size_t buff_size = 1000;
    char buff[buff_size];
    ssize_t read_size = read(con_sockit, buff, buff_size);
    if (read_size == 0){
        close(con_sockit);
        cout << "con_sockit closed" << endl;
        return ;
    }
    if (read_size == -1){
        cerr << "Chehaja Mahyach Had Le3jeb Fila" << endl;
        close(con_sockit);
        return ;
    }
    client[con_sockit].process_req(string("").append(buff, read_size), read_size);
}

void multiblex::use_clinet_fd(int con_sockit, int n){

    if (events[n].events & EPOLLIN){
        cout << "-------- do_use_fd Bdat -------- fd = "<<con_sockit<<endl;
        do_use_fd(con_sockit);
        cout << "------------ do_use_fd Salat -----------\n"<<endl;
    }    
    else if (client[con_sockit].body_state){
        client[con_sockit].process_req("",0);
        cout << "Nchofo Had fd Chehal :"<<con_sockit<<endl;
        if (!client[con_sockit].method || client[con_sockit].method->end){
            respons = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: ";
            stringstream ss;
            ss << client[con_sockit].get_respons().length();
            respons += ss.str();
            respons += string("\r\n\r\n");
            respons += client[con_sockit].get_respons();
            write(con_sockit, respons.c_str(), respons.size());
            close(con_sockit);
            epoll_ctl(epollfd, EPOLL_CTL_DEL, con_sockit, &ev);
            client.erase(con_sockit);

        }
    }
}

void multiblex::m_server(){

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

    int nfds;
    while (1) {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }
        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == listen_sock) 
                add_client();
            else
                use_clinet_fd(events[n].data.fd, n);
        }
    }
}


multiblex::~multiblex(){
    ;
}


// void setnonblocking(int con_sockit){
//     fcntl(con_sockit, F_SETFL , fcntl(con_sockit, F_GETFL, 0) | O_NONBLOCK);
// }