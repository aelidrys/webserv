#include "Get.hpp"

Get::Get(){
    end = 0;
    opened = 0;
}

Get::Get(const Get& oth){
    *this = oth;
}


Get& Get::operator=(const Get& oth){
    if (this != &oth){
        // respons = oth.respons;
        // end = oth.end;
        headers = oth.headers;
        http_v = oth.http_v;
        req_path = oth.req_path;
        r_path = oth.r_path;
    }
    return *this;
}

void Get::open_file(){

    fd = open(req_path.c_str(), O_RDWR);
    if (fd == -1){
        string index = "/default.html";
        string tmp = req_path+index;
        fd = open(tmp.c_str(), O_RDWR);
    }
    opened = 1;
    if (fd == -1)
        opened = -1;
}

void Get::process(std::string _body, size_t _body_size){
    body = _body;
    body_size = _body_size;

    cout << "GET Request Enter" << endl;
    if (!opened)
        open_file();
    ssize_t r_len, max_r = 2000;
    if (fd >= 0){
        string res;
        res.resize(max_r);
        r_len = read(fd, &res[0], max_r);
        if (r_len < max_r)
            end = 1;
        res.resize(r_len);
        respons += res;
        return ;
    }
    end = 1;
    respons = "URI Not Found";
    cout << "GET Request End" << endl;
}

Get::~Get(){
    ;
}