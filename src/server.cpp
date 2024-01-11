#include "request.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>



#ifndef PORT
#define PORT 8000
#endif

void server(){
    request req;
    int server_fd, new_socket; long valread;
    
    std::string hello = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 12\r\n\r\nHello world!";
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    

    struct sockaddr_in address;
    int addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof(address.sin_zero));
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        // printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);
        // printf("%s\n",buffer );
        req.parce_req(buffer);
        req.show_inf();
        write(new_socket , hello.c_str() , hello.length());
        // printf("------------------Hello message sent-------------------\n");
        close(new_socket);
    }
}

// int main(int ac, char **av){
//     // if (ac <= 1){
//     //     std::cout << "no input file\n";
//     //     return 0;
//     // }
//     // (void)av;
//     test(av[1]);
//     m_server();
// }