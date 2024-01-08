#include "request.hpp"

request::request()
{
    body_state = 0;
}

int request::spl_req_body(std::string s1)
{
    if (body_state){
        body += s1;
        return 1;
    }
    if (s1.find("\r\n\r\n", 0) != s1.npos)
    {
        body = s1.substr(s1.find("\r\n\r\n", 0) + 4);
        req_h = s1.substr(0, s1.find("\r\n\r\n", 0));
        body_state = 1;
        return 1;
    }
    // std::cout << "$$$$$$" << std::endl;
    req_h = s1;
    // std::cout << req_h << std::endl;
    return 0;
}

int request::parce_key(const std::string &key)
{
    if (key.size() > 0 && !std::isalpha(key[0]))
    {
        std::cout << "ERROR: " << key << " invalid key" << std::endl;
        return 0;
    }
    for (size_t i = 0; i < key.size(); i++)
    {
        if (!std::isalnum(key[i]) && key[i] != '_' && key[i] != '-')
        {
            std::cout << "ERROR: " << key << " invalid key" << std::endl;
            return 0;
        }
    }
    return 1;
}

int request::check_path(const std::string& rpath){
    struct stat fileStat;
    // rpath += path;
    return stat(rpath.c_str(), &fileStat) == 0;
}

int request::parce_rline(const std::string &rline){
    std::stringstream ss;
    std::string tmp;
    ss<<rline;
    std::getline(ss, tmp, ' ');
    if (tmp != "GET" && tmp != "POST" && tmp != "DELETE"){
        std::cout << "ERROE: unkounu method " << tmp << std::endl;
        return 0;
    }
    type = tmp;
    std::getline(ss, tmp, ' ');
    // if (check_path(tmp)){
    //     std::cout << "ERROE: unkounu path " << tmp << std::endl;
    //     return 0;
    // }
    r_path = tmp;
    std::getline(ss, tmp);
     if (tmp != "HTTP/1.1\r" || tmp != "HTTP/1.1\r"){
        std::cout << "ERROE: unkounu http version " << tmp << std::endl;
        return 0;
    }
    http_v = tmp;
    return 1;
}

int request::parce_line(const std::string &line)
{
    std::stringstream ss;
    std::string key;
    std::string value;

    ss << line;
    std::getline(ss, key, ':');
    std::getline(ss, value, ' ');
    std::getline(ss, value, '\r');
    if (!parce_key(key) && value.size())
        return 0;
    if (value.size() == 0){
        std::cout << "ERROR: no value for key " << key << std::endl;
        return 0;
    }
    headers[key] = value;
    return 1;
}

void request::parce_req(const std::string &req)
{
    std::string line;
    int c_f_body = 0;

    spl_req_body(req);
    sstr << req_h;
    std::getline(sstr,line);
    if (!parce_rline(line))
        return ;
    while (sstr.peek() != -1)
    {
        std::getline(sstr, line);
        if (!parce_line(line))
            break;
    }
}

void request::show_inf() const
{
    std::map<std::string, std::string>::const_iterator it;
    for (it = headers.begin(); it != headers.end(); it++)
        std::cout << "|" << it->first << " => " << it->second << "|" << std::endl;
    std::cout << "\r\n\r\n ----<body>----" << body << std::endl;
}

request::~request()
{
    ;
}
