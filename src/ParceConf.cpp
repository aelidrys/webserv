/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParceConf.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouassi <abouassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 17:54:31 by abouassi          #+#    #+#             */
/*   Updated: 2024/01/19 17:22:26 by abouassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParceConf.hpp"

std::vector<std::string> ParceConf::Split_line(std::string line)
{
    std::stringstream ss;
    std::vector<std::string> vline;
    index = 0;
    ss << line;
    std::string chunks;
    while (std::getline(ss,chunks,' '))
    {
        if(!chunks.empty())
            vline.push_back(chunks);
    }
    return vline;
}

ParceConf::ParceConf(std::string confgfile)
{
    std::ifstream configfile;
    std::string line;
    std::vector<std::string> _split;
    configfile.open(confgfile.c_str(), std::ios::in);
    if (configfile.is_open()) 
    {
        while (getline(configfile, line)) {
            
            if (!line.empty() )
            {
                _split = Split_line(line);
                if (!_split.empty())
                {
                    Vconf.push_back(_split);
                }
            }
        }
        configfile.close();
    }
    fd = -2;
}

Servers ParceConf::FirstFill()
{
    Servers server;
    // int bracket = 0;
    if (Vconf[index][0] != "server" || Vconf[index].size() > 1)
    {
        throw "Error : no server derectires";
    }
    else
        server.servconf.push_back(Vconf[index]);
    index++;
    
    // if (index < Vconf.size()  && Vconf[index][0] != "{")
    // {
    //     throw "Error : no open brackets \n";
    // }
    // index++;
    // bracket++;
    
    while (index < Vconf.size() && Vconf[index][0] != "server")
    {
        // if (Vconf[index][0] == "{")
        //     bracket++;

        server.servconf.push_back(Vconf[index]);
        index++;
    }
    // std::cout<<"_______br______"<<bracket<<std::endl;
    // if (Vconf[index - 1][0] != "}")
    // {
    //     throw "Error : no open brackets \n";
    // }
    
    return server;
}
void ParceConf::FillServers()
{
  
    
    std::vector<std::string> Vcol;
    std::vector<std::string>::iterator iter;

    while (index < Vconf.size())
    {
        Vservers.push_back(FirstFill());
    }
    
}

void ParceConf::desplay()
{
    FillServers();
    size_t i = 0;
    while (i < Vservers.size())
    {
        // std::cout<<"###############################"<<std::endl;
        
        Vservers[i].SetAllDir();
        Vservers[i].CreatSocketServer();
        msockets[Vservers[i].server_fd] = Vservers[i];
        // Vservers[i].desplay();
        // std::cout<<"###############################"<<std::endl;
        i++;
    }
    // for (size_t  i = 0; i < vsockets.size();i++)
    // {
    //     std::cout<<"sockets for server :"<<vsockets[i]<<std::endl;
    // }
    
    // std::vector<std::vector<std::string>> matrix =  Vservers[0].vconf;
    // for (const auto& row : matrix) {
    //     for (std::string value : row) {
    //         std::cout << value << "|";
    //     }
    //     std::cout << std::endl;
    // }
}
// void ParceConf::CreatSocketServer()
// {
    
// }
ParceConf::~ParceConf()
{
}

//  Vconf.push_back(std::pair<int, std::vector<std::string>>(0, Split_line(line)));
//      std::vector<std::pair<int,std::vector<std::string>>>  matrix =  Vconf;
//     std::cout<<"jdjdfjfj\n";
//     for (const auto& row : matrix) {
//         std::cout<<"key  :"<<row.first<<"  ";
//         for (std::string value : row.second) {
//             std::cout << value << "|";
//         }
//         std::cout << std::endl;
//     }


// void do_use_fd(int sockfd) {
//     char buffer[1024];
//     ssize_t bytesRead;

//     for (;;) {
//         // Read data from the socket
//         bytesRead = read(sockfd, buffer, sizeof(buffer));

//         if (bytesRead == -1) {
//             if (errno == EAGAIN || errno == EWOULDBLOCK) {
//                 // No more data available for now, exit the loop
//                 break;
//             } else {
//                 perror("read");
//                 // Handle other read errors
//                 break;
//             }
//         } else if (bytesRead == 0) {
//             // Connection closed by the client
//             // Perform cleanup or additional actions if needed
//             close(sockfd);
//             printf("Connection closed by client\n");
//             break;
//         } else {
//             // Process the received data
//             // Example: Print the received data
//             printf("Received data from socket %d: %.*s\n", sockfd, (int)bytesRead, buffer);
//         }
//     }

//     // Continue processing or return to the event loop
// }
std::string chekIf(bool check)
{
    if (check)
        return "";
    else
        return  "Content-type: text/html\r\n\r\n"; 
}
bool ParceConf::checkvalidHeader(std::string str)
{
    size_t pos;
    pos = str.find("Content-type:");
    if (pos == string::npos)
    {
        return 0;
    }
    if (pos != 0)
    {
        return 0;
    }
    pos = str.find("\r\n");
    if (pos == string::npos)
    {
        return 0;
    }
    return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// void ParceConf::Out_Events(int n)
// {
//     char cgiBuffer[1024];
//     std::string cgires("");
//     string globRusl;

//     if (mClients[events[n].data.fd].second.req_done())
//     {
        
//         if (fd != -2)
//         {
//             int readb = 1;
//             bool check_cont;
//             while (readb != 0)
//             {
//                 readb = read(fd,cgiBuffer,1024);
//                 cout<<"read byts :"<<readb<<endl;
//                 cout<<"fd  :"<<fd<<endl;
//                 if (readb == 0)
//                     break;
//                 std::cout<<"fd != -2   fd :"<<fd<<"\n";
//                 if (readb == -1)
//                 {
//                     perror("Erorr read in cgi\n");
//                     exit(1);
//                 }
//                 cgires.append(cgiBuffer,readb);
//             }
//             std::cout<<"enter here :"<<cgires<<endl;
//             check_cont = checkvalidHeader(cgires);
//             std::cout<<check_cont<<endl;
//             globRusl = "HTTP/1.1 200 OK\r\n" + chekIf(check_cont) + cgires;
//             write(events[n].data.fd,globRusl.c_str(),globRusl.length());
//             epoll_ctl(epollfd,EPOLL_CTL_DEL,events[n].data.fd,&ev);
//             close(fd);
//             close(events[n].data.fd);
//             mClients.erase(events[n].data.fd);
//             fd = -2;
//         }
//         else
//         {
//             mClients[events[n].data.fd].second.process_req(string(""),0,EPOLLOUT);

//             string res = mClients[events[n].data.fd].second.get_respons();

//             write(events[n].data.fd , res.c_str(), res.size());
//             if (mClients[events[n].data.fd].second.method && mClients[events[n].data.fd].second.method->end)
//             {
//                 epoll_ctl(epollfd,EPOLL_CTL_DEL,events[n].data.fd,&ev);
//                 close(events[n].data.fd);
//                 mClients.erase(events[n].data.fd);
//                 cout<<"close connections :"<<events[n].data.fd<<endl;
//             }
//         }
//     }
// }

// void ParceConf::In_Events(int n)
// {
//     char buffer[1024];
//     ssize_t bytesRead = 0;

//     std::cout<<"Enter clinet "<<events[n].data.fd<<" \n";
//     bytesRead = read(events[n].data.fd,buffer,1024);
    
//     std::cout<<"size :"<<bytesRead<<std::endl;
//     if (bytesRead == -1)
//     {
//         perror("Error read\n");
//         return ;
//     }
//     if (bytesRead == 0) 
//     {
//         close(events[n].data.fd);
//         std::cout<<"Connection closed by client\n"; 
//     }
//     else 
//     {
//         std::map<int ,std::pair<Servers,Request> >::iterator iter2 = mClients.find(events[n].data.fd);
//         if (iter2 != mClients.end())
//         {
            
//             mClients[events[n].data.fd].second.process_req(string("").append(buffer, bytesRead),bytesRead,EPOLLIN);
//             if (mClients[events[n].data.fd].second.r_path == "/favicon.ico")
//             {
//                 epoll_ctl(epollfd,EPOLL_CTL_DEL,events[n].data.fd,&ev);
//                 close(events[n].data.fd);
//                 mClients.erase(events[n].data.fd);
//                 cout<<"close connections :"<<events[n].data.fd<<endl;
//             }
            
//             // if (mClients[events[n].data.fd].second.r_path.find("/cgi") != std::string::npos)
//             // {
//             //     Cgi cgi(mClients[events[n].data.fd].second.r_path,mClients[events[n].data.fd].first.getLocation("/cgi"));
//             //     std::cout<<"enter here\n";
//             //     cgi.ExecCgi();
//             //     fd  = open("out.txt",O_RDONLY);
//             // }
//         }
//     }
// }

// void ParceConf::Connect_And_Add(int n)
// {
//     int adrlen;
//     std::map<int, Servers>::iterator iter = msockets.find(events[n].data.fd );
//     if (iter != msockets.end()) 
//     {   
//         adrlen = sizeof(iter->second.address);
//         conn_sock = accept(iter->first, (struct sockaddr *)&iter->second.address, (socklen_t*)&adrlen);
//         if (conn_sock == -1) 
//         {
//             perror("accept");
//             exit(EXIT_FAILURE);
//         }

//         mClients[conn_sock].first = iter->second;
//         Request req(mClients[conn_sock].first.root[0]);
//         mClients[conn_sock].second = req;
//         std::cout<<"Fd Server :"<<iter->first<<std::endl;
//         ev.events = EPOLLIN | EPOLLOUT;
//         ev.data.fd = conn_sock;
//         if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,&ev) == -1) 
//         {
//             perror("epoll_ctl: conn_sock");
//             exit(EXIT_FAILURE);
//         }
//     } 
//     else 
//     {
//         if (events[n].events & EPOLLIN) 
//         {
//             In_Events(n);
//         }
//         else if (events[n].events & EPOLLOUT && mClients.find(events[n].data.fd) !=  mClients.end()) 
//         {
//             Out_Events(n);
//         }
//     }
// }


// void ParceConf::CreatMUltiplex()
// {
//     std::ifstream inputFile;
//     inputFile.open("index.html");
//     std::string line;
//     std::string data= "";
//     while (std::getline(inputFile, line)) {
//         data += line;
//     }
//     // std::string headers = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 1000000\r\n\r\n";
//     // std::string httprespose = headers + data;
//     // const char *hello = httprespose.c_str();

//     epollfd = epoll_create(255);
//     if (epollfd == -1) {
//         perror("epoll_create1");
//         exit(EXIT_FAILURE);
//     }
//     std::map<int,Servers>::iterator iter;

//     for (iter = msockets.begin(); iter != msockets.end(); iter++)
//     {
//         ev.events = EPOLLIN;
//         ev.data.fd = iter->first;
//         if (epoll_ctl(epollfd, EPOLL_CTL_ADD, iter->first, &ev) == -1) {
//             perror("epoll_ctl: listen_sock");
//             exit(EXIT_FAILURE);
//         }
//     }
//     while (true)
//     {
//         nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
//         // std::cout<<"nfds   :"<<nfds<<std::endl;
//         if (nfds == -1) {
//             perror("epoll_wait");
//             exit(EXIT_FAILURE);
//         }
        
//         for (int n = 0; n < nfds ; ++n) 
//         {
//             Connect_And_Add(n);
//         }
//     }

// }
//////////////////////////////////////////////////////////////////////////////////////////////////////