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
using namespace std;

#ifndef REQUEST_HPP
#define REQUEST_HPP

class request
{
std::stringstream sstr;
std::string req_h;
std::string root_path;
public:
    int body_state;
    std::string type;
    std::string r_path;
    std::string req_path;
    std::string http_v;
    std::string body;
    std::map<std::string, std::string> headers;

    request();
    request(std::string& root_path);
    request(const request& req);
    request& operator=(const request& oth);
    void parce_req(const std::string& req1);
    void show_inf() const;
    int parce_line(const std::string& line);
    int check_path();
    int parce_rline(const std::string& line);
    int parce_key(const std::string& key);
    int spl_reqh_body(std::string s1);
    int req_done();
    ~request();
};

void m_server();

#endif