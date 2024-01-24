#include "Request.hpp"

Request::Request()
{
    method = NULL;
    body_state = 0;
    body_size = 0;
    error = 0;
    root_path = "/nfs/homes/aelidrys/Desktop/webserv/root_dir";
}


Request::Request(const Servers &ser){
    method = NULL;
    body_state = 0;
    body_size = 0;
    error = 0;
    serv = ser;
    root_path = ser.root[0];
}

Request::Request(const Request& req1){
    method = NULL;
    *this = req1;
}

Request& Request::operator=(const Request& oth){
    if (this != &oth){
        delete method;
        error = oth.error;
        method = oth.method;
        body_state = oth.body_state;
        body_size = oth.body_size;
        type = oth.type;
        r_path = oth.r_path;
        root_path = oth.root_path;
        req_path = oth.req_path;
        http_v = oth.http_v;
        body = oth.body;
        headers = oth.headers;
        uri = oth.uri;
        serv = oth.serv;
    }
    return *this;
}

int Request::spl_reqh_body(string s1)
{
    if (body_state){
        body = s1;
        body_size += body.size();
        return 0;
    }
    if (s1.find("\r\n\r\n", 0) != s1.npos)
    {
        body = s1.substr(s1.find("\r\n\r\n", 0) + 4);
        cout << "--_______Lheaders Te9raw Kolhom________--\n" << endl;
        req_h += s1.substr(0, s1.find("\r\n\r\n", 0));
        body_state = 1;
        body_size = body.size();
        return 1;
    }
    req_h += s1;
    return 0;
}

int Request::parce_key(const string &key)
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

int Request::parce_rline(const string &rline){
    stringstream ss;
    string tmp;
    ss<<rline;
    getline(ss, tmp, ' ');
    if (tmp != "GET" && tmp != "POST" && tmp != "DELETE"){
        cerr << "ERROE: Unkounu Method " << rline << endl;
        return 0;
    }
    type = tmp;
    getline(ss, tmp, ' ');
    r_path = tmp;
    uri = tmp;
    req_path = root_path + uri;
    getline(ss, tmp);
     if (tmp != "HTTP/1.1\r" && tmp != "HTTP/1.1"){
        cout << "ERROE: Unkounu Http Version " << tmp << endl;
        return 0;
    }
    http_v = tmp;
    return 1;
}

int Request::parce_line(const string &line)
{
    stringstream ss;
    string key;
    string value;

    ss<<line;
    getline(ss, key, ':');
    getline(ss, value, ' ');
    getline(ss, value, '\r');
    if (!parce_key(key) && value.size())
        return 0;
    if (value.size() == 0){
        cout << "ERROR: No Value For Key " << key << endl;
        return 0;
    }
    headers[key] = value;
    return 1;
}


int Request::req_done(){
    if (!body_state)
        return 0;
    if (headers.find("Content-Length") != headers.end()){
        if (body_size != (size_t)atoi(&headers.find("Content-Length")->second[0]))
        return 0;
    }
    return 1;
}

void Request::parce_req(const string &req)
{
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
    cout<<"uri: "<<uri<<endl;
    serv.FillData(uri);
    method = create_method(type);
}

void    Request::process_req(const string &req, size_t read_len, int event){
    parce_req(req);
    if (body_state && method)
        method->process(body, read_len, event);
}



Method* Request::create_method(const string &type){
    Method* m = NULL;
    if (type == "GET")
        m = new Get();
    // if (type == "POST")
        // m = new Post();
    // if (type == "DELETE")
        // m = new Delete();
    else
        cerr << "Unkounu Method" << endl;
    if (m){
        m->headers = headers;
        m->http_v = http_v;
        m->r_path = uri;
        m->req_path = req_path;
        m->fullUri_path = serv.rootUri;
        m->serv = serv;
    }
    return (m);
}

string Request::get_respons() const{
    if (!method)
        return("");
    return (method->respons);
}

Request::~Request(){
    delete method;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

void Request::show_inf() const
{
    map<string, string>::const_iterator it;
    cout<<"Request line -> : "<<type<<" "<< r_path<<" "<< http_v<< endl;
    for (it = headers.begin(); it != headers.end(); it++)
        cout << "||" << it->first << " => " << it->second << "||" << endl;
    cout << "\n$$$$  body_size = "<<body_size<<" b_state: "<<body_state;
    cout <<" $$$$$\n ----<body>---- \n" << body << endl;
}