#include "Get.hpp"
#include <sys/stat.h>


Get::Get(){
    set_extentions();
    end = 0;
    opened = 0;
}

Get::Get(const Get& oth){
    set_extentions();
    *this = oth;
}

Get& Get::operator=(const Get& oth){
    if (this != &oth){
        respons = oth.respons;
        end = oth.end;
        content_type = oth.content_type;
        headers = oth.headers;
        http_v = oth.http_v;
        req_path = oth.req_path;
        r_path = oth.r_path;
    }
    return *this;
}

void Get::set_content_type(){
    size_t pos = r_path.find(".");
    if (pos != string::npos && pos+1 < r_path.size()){
        if (types.find(r_path.substr(pos+1)) != types.end())
            content_type = types.find(r_path.substr(pos+1))->second;
        else
            content_type = "application/octet-stream";
    }
    else
        content_type = "application/octet-stream";
}

void Get::set_extentions(){
    types["html"] = "text/html";
    types["htm"] = "text/html";
    types["css"] = "text/css";
    types["jpeg"] = "image/jpeg";
    types["jpg"] = "image/jpeg";
    types["png"] = "image/png";
    types["gif"] = "image/gif";
    types["json"] = "application/json";
    types["mp4"] = "video/mp4";
    types["mp3"] = "audio/mpeg";
    types["js"] = "application/javascript";
    types["bmp"] = "image/bmp";
    types["ico"] = "image/x-icon";
    types["pdf"] = "application/pdf";
    types["txt"] = "text/plain";
    types["xml"] = "application/xml";
    types["zip"] = "application/zip";
    types["tar"] = "application/x-tar";
    types["gz"] = "application/gzip";
    
}

void Get::open_file(){
    string tmp = req_path;
    string index = "/default.html";
    cout<<"open file"<<endl;
    if (opendir(req_path.c_str())){
        tmp += index;
        content_type = "text/html";
    }
    else
        set_content_type();
    src_file.open(tmp.c_str(), ios::in);
    opened = 1;
    if (!src_file.is_open()){
        opened = -1;
        cout<<"open file "<<tmp<<" faild"<<endl;
        return;
    }

    src_file.seekg(0, std::ios::end);
    file_len = src_file.tellg();
    src_file.seekg(0, std::ios::beg);
    respons = "HTTP/1.1 200 OK\r\nContent-Type: ";
    respons +=  content_type+string("\r\nContent-Length: ");
    stringstream ss;
    ss<<file_len;
    respons += ss.str();
    respons += string("\r\n\r\n");
    cout<<"content_len: "<<file_len<<endl;
    cout<<"content_type: "<<content_type<<endl;
}

int Get::process(string _body, size_t _body_size, int event){
    body = _body;
    body_size = _body_size;

    if (event == EPOLLIN){
        cout << "GET->in" << endl;
        return(0);
    }
    cout << "GET->out" << endl;
    respons = "";
    if (!opened)
        open_file();
    ssize_t r_len, max_r = 1000;

    if (opened == 1){
        string res;
        res.resize(max_r);
        src_file.read(&res[0], max_r);
        r_len = src_file.gcount();
        if (src_file.gcount() < max_r)
            end = 1;
        res.resize(r_len);
        respons += res;
    }
    if (opened == -1){
        end = 1;
        respons = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 13\r\n\r\nURI Not Found";
    }
    return(0);
}

Get::~Get(){
    ;
}