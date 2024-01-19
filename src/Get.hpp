#include "Method.hpp"

using namespace std;


#ifndef GET_HPP
#define GET_HPP


class Get : public Method
{
string body;
int body_size;
int opened;
size_t file_len;
fstream src_file;
map<string,string> types;
string content_type;
public:    
    Get();
    Get(const Get& oth);
    Get& operator=(const Get& oth);
    void set_content_type();
    void process(std::string body, size_t body_size);
    void open_file();
    ~Get();
};

#endif