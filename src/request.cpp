#include "request.hpp"

request::request()
{
    body_state = 0;
    root_path = "/nfs/homes/aelidrys/Desktop/webserv";
}

request::request(std::string& root_path1){
    root_path = root_path1;
}

int request::spl_reqh_body(std::string s1)
{
    if (body_state){
        body = s1;
        return 0;
    }
    if (s1.find("\r\n\r\n", 0) != s1.npos)
    {
        body = s1.substr(s1.find("\r\n\r\n", 0) + 4);
        std::cout << "--_______Lheaders Te9raw Kolhom________--\n" << std::endl;
        req_h = s1.substr(0, s1.find("\r\n\r\n", 0));
        std::cout <<"#################\n"<< req_h <<"\n##############"<< std::endl;
        body_state = 1;
        return 1;
    }
    // req_h += s1;
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

int request::check_path(){
    struct stat fileStat;
    req_path = root_path + r_path;
    return stat(req_path.c_str(), &fileStat) == 0;
}

int request::parce_rline(const std::string &rline){
    std::stringstream ss;
    std::string tmp;
    ss<<rline;
    std::getline(ss, tmp, ' ');
    if (tmp != "GET" && tmp != "POST" && tmp != "DELETE"){
        std::cerr << "ERROE: unkounu method " << tmp << std::endl;
        return 0;
    }
    type = tmp;
    std::getline(ss, tmp, ' ');
    r_path = tmp;
    if (!check_path()){
        std::cout << "ERROE: unkounu path " << tmp << std::endl;
        return 0;
    }
    std::getline(ss, tmp);
     if (tmp != "HTTP/1.1\r" && tmp != "HTTP/1.1"){
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


int request::req_done(){
    if (!body_state)
        return 0;
    if (headers.find("Content-Length") != headers.end()){
        if (body.size() != atoi(&headers.find("Content-Length")->second[0]))
        return 0;
    }
    return 1;
}

void request::parce_req(const std::string &req)
{
    std::cout << "----------- parse_req Bdat ------------\n";
    std::string line;
    int c_f_body = 0;

    if (!spl_reqh_body(req))
        return ;
    sstr << req_h;
    std::getline(sstr,line);
    if (!parce_rline(line))
        return ;
    while (sstr.peek() != -1)
    {
        std::getline(sstr, line);
        if (line.size() && !parce_line(line))
            break;
    }
    std::cout << "------------ parse_req Salat ------------\n";
}

void request::show_inf() const
{
    std::map<std::string, std::string>::const_iterator it;
    std::cout<<"request line -> : "<<type<<" "<< r_path<<" "<< http_v<< std::endl;
    for (it = headers.begin(); it != headers.end(); it++)
        std::cout << "||" << it->first << " => " << it->second << "||" << std::endl;
    std::cout << "\n\n ----<body>---- \n" << body << std::endl;
}

request& request::operator=(const request& oth){
    if (this != &oth){
        body_state = oth.body_state;
        type = oth.type;
        r_path = oth.r_path;
        http_v = oth.http_v;
        body = oth.body;
        headers = oth.headers;
    }
    return *this;
}

request::request(const request& req1){
    *this = req1;
}

request::~request(){
    ;
}
