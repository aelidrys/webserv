#include "Get.hpp"

Get::Get(){
    ;
}

Get::Get(const Get& oth){
    *this = oth;
}

Get& Get::operator=(const Get& oth){
    if (this != &oth){
        headers = oth.headers;
        http_v = oth.http_v;
        req_path = oth.req_path;
        r_path = oth.r_path;
    }
    return *this;
}

void Get::process(std::string _body, size_t _body_size){
    body = _body;
    body_size = _body_size;
    cout << "GET Request Enter" << endl;
    cout<<"^^^^^^^^^^^^^^^^^^^^^\n"<< body <<"\n^^^^^^^^^^^^^^^^^^^^^^"<<endl;
}

Get::~Get(){
    ;
}