#include "request.hpp"

request::request()
{
    body_state = 0;
    root_path = "/nfs/homes/aelidrys/Desktop/webserv";
}

request::request(string& root_path1){
    root_path = root_path1;
}

int request::spl_reqh_body(string s1)
{
    if (body_state){
        body += s1;
        return 0;
    }
    if (s1.find("\r\n\r\n", 0) != s1.npos)
    {
        body = s1.substr(s1.find("\r\n\r\n", 0) + 4);
        cout << "--_______Lheaders Te9raw Kolhom________--\n" << endl;
        req_h = s1.substr(0, s1.find("\r\n\r\n", 0));
        cout <<"#################\n"<< req_h <<"\n##############"<< endl;
        body_state = 1;
        return 1;
    }
    // req_h += s1;
    return 0;
}

int request::parce_key(const string &key)
{
    if (key.size() > 0 && !isalpha(key[0]))
    {
        cout << "ERROR: " << key << " invalid key" << endl;
        return 0;
    }
    for (size_t i = 0; i < key.size(); i++)
    {
        if (!isalnum(key[i]) && key[i] != '_' && key[i] != '-')
        {
            cout << "ERROR: " << key << " invalid key" << endl;
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

int request::parce_rline(const string &rline){
    stringstream ss;
    string tmp;
    ss<<rline;
    getline(ss, tmp, ' ');
    if (tmp != "GET" && tmp != "POST" && tmp != "DELETE"){
        cerr << "ERROE: unkounu method " << tmp << endl;
        return 0;
    }
    type = tmp;
    getline(ss, tmp, ' ');
    r_path = tmp;
    if (!check_path()){
        cout << "ERROE: unkounu path " << tmp << endl;
        return 0;
    }
    getline(ss, tmp);
     if (tmp != "HTTP/1.1\r" && tmp != "HTTP/1.1"){
        cout << "ERROE: unkounu http version " << tmp << endl;
        return 0;
    }
    http_v = tmp;
    return 1;
}

int request::parce_line(const string &line)
{
    stringstream ss;
    string key;
    string value;

    ss << line;
    getline(ss, key, ':');
    getline(ss, value, ' ');
    getline(ss, value, '\r');
    if (!parce_key(key) && value.size())
        return 0;
    if (value.size() == 0){
        cout << "ERROR: no value for key " << key << endl;
        return 0;
    }
    headers[key] = value;
    return 1;
}


int request::req_done(){
    if (!body_state)
        return 0;
    // cout << "body_size = " << body.size() << endl;
    // cout << "body\n **********\n " << body <<"**********"<< endl;
    if (headers.find("Content-Length") != headers.end()){
        if (body.size() != atoi(&headers.find("Content-Length")->second[0]))
        return 0;
    }
    return 1;
}

void request::parce_req(const string &req)
{
    cout << "----------- parse_req Bdat ------------\n";
    string line;

    if (!spl_reqh_body(req))
        return ;
    sstr << req_h;
    getline(sstr,line);
    if (!parce_rline(line))
        return ;
    while (sstr.peek() != -1)
    {
        getline(sstr, line);
        if (line.size() && !parce_line(line))
            break;
    }
    cout << "------------ parse_req Salat ------------\n";
}

void request::show_inf() const
{
    map<string, string>::const_iterator it;
    cout<<"request line -> : "<<type<<" "<< r_path<<" "<< http_v<< endl;
    for (it = headers.begin(); it != headers.end(); it++)
        cout << "||" << it->first << " => " << it->second << "||" << endl;
    cout << "\n\n ----<body>---- \n" << body << endl;
}

request& request::operator=(const request& oth){
    if (this != &oth){
        body_state = oth.body_state;
        type = oth.type;
        r_path = oth.r_path;
        req_path = oth.req_path;
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
