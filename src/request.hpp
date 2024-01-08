#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>


#ifndef REQUEST_HPP
#define REQUEST_HPP

class request
{
std::stringstream sstr;
std::string req_h;
protected:
    std::string host;
    std::string connection;
    std::string Accept;
    std::string Accept_Encoding;
    std::string User_Agent;
public:
    int body_state;
    std::string type;
    std::string r_path;
    std::string http_v;
    std::string body;
    std::map<std::string, std::string> headers;
    
    request();
    void parce_req(const std::string& req);
    int parce_line(const std::string& line);
    int check_path(const std::string& rpath);
    int parce_rline(const std::string& line);
    int parce_key(const std::string& key);
    int spl_req_body(std::string s1);
    void show_inf() const;
    ~request();
};

void m_server();

#endif