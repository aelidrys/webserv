/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouassi <abouassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 17:42:27 by abouassi          #+#    #+#             */
/*   Updated: 2024/01/06 16:56:47 by abouassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERS_HPP
#define SERVERS_HPP

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <algorithm> 
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include <map>
#include <sys/stat.h>
#include "Location.hpp"
#include <stack>
class Servers
{
private:
  


    std::vector<std::string>  AddErrorPage(std::string status,std::string path);
    int  checkDup(std::string der,int & index);
    bool check_isdigit(std::string str);
    void FillStatus();
    void FillValid();
    void checkValidation();
    void parceIp(std::string ip);
    int  pathExists(std::string path);
    void check_Status(std::string status);

    
    size_t GetIndex(std::string dir);
    
    Location FirstFill(size_t & index);
    void FillLocation();
    
    void SetPorts (); // done ~
    void SetServerName(); // done ~
    void SetHost();  // done ~
    void SetRoot();  // done ~
    void SetError_page(); // done ~
    void SetClient_max_body_size(); // done ~
    void SetIndex(); // done ~
    
    void ParceServers();    
    
    void Printtwodom(const std::vector<std::vector<std::string> > & matrix,std::string data);



    void ReplacePath(std::string status,std::string path);

public:

    std::vector<int> port;
    std::vector<std::string> server_name; 
    std::vector<std::string> host; 
    std::vector<std::string> root;
    std::vector<std::string> index;
    std::vector<long long int > client_max_body_size;
    std::vector<std::vector<std::string> > error_page;
    std::vector<std::string> s_erorr;


    int server_fd;
    struct sockaddr_in address;
    std::vector<std::string> Vstrvalid;
    std::vector<std::vector<std::string> > servconf;
    //________________________//
    //________loaction________//
    
    std::vector<Location> loactions;

    //________________________//
    void SetAllDir();
    void desplay();
    
    const std::vector<int> & GetPorts (); 
    const std::vector<std::string> & GetServerName(); 
    const std::vector<std::string> & GetHost();  
    const std::vector<std::string> & GetRoot();  
    const std::vector<std::vector<std::string> >&  GetError_page(); 
    const std::vector<long long int > &  GetClient_max_body_size(); 
    const std::vector<std::string> & GetIndex(); 
    template<typename T,typename V>
    void Print_dirs(T begin,T end,V data)
    {
        while(begin != end)
        {
            std::cout<<data<<"  :"<<*begin<<std::endl;
            begin++;
        }
    }
    Location & getLocation(std::string);
    void SetDefaultError();

    void CreatSocketServer();
    Servers();
    ~Servers();
};


#endif