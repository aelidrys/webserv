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

#ifndef METHOD_HPP
#define METHOD_HPP

class Method
{

public:
    std::string r_path;
    std::string req_path;
    std::string http_v;
    std::map<std::string, std::string> headers;

    Method();
    Method(const Method& oth);
    Method& operator=(const Method& oth);
    virtual void process(std::string body, size_t body_size) = 0;
    virtual ~Method();
};


#endif