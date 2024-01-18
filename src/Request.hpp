#include "Get.hpp"
using namespace std;


#ifndef REQUEST_HPP
#define REQUEST_HPP

class Request
{
std::stringstream sstr;
std::string req_h;
std::string root_path;
public:
    Method* method;
    size_t body_size;
    int body_state;
    std::string type;
    std::string r_path;
    std::string req_path;
    std::string http_v;
    std::string body;
    std::map<std::string, std::string> headers;

    Request();
    Request(std::string& root_path);
    Request(const Request& req);
    Request& operator=(const Request& oth);
    void parce_req(const std::string& req1);
    void process_req(const string &req, size_t read_len);
    void show_inf() const;
    int parce_line(const std::string& line);
    int parce_rline(const std::string& line);
    int parce_key(const std::string& key);
    int spl_reqh_body(std::string s1);
    int req_done();
    Method* create_method(const string &type);
    string get_respons() const;
    ~Request();
};

void m_server();

#endif