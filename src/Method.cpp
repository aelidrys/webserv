#include "Method.hpp"

Method::Method(){
    ;
}

Method::Method(const Method& oth){
    *this = oth;
}

Method& Method::operator=(const Method& oth){
    if (this != &oth){
        headers = oth.headers;
        http_v = oth.http_v;
        req_path = oth.req_path;
        r_path = oth.r_path;
    }
    return *this;
}


Method::~Method(){
    ;
}