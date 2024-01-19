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
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
using namespace std;

#ifndef METHOD_HPP
#define METHOD_HPP

class Method
{

public:
    string respons;
    int end;

    string r_path;
    string req_path;
    string http_v;
    map<string, string> headers;

    Method();
    Method(const Method& oth);
    Method& operator=(const Method& oth);
    virtual void process(string body, size_t body_size) = 0;
    virtual ~Method();
};


#endif