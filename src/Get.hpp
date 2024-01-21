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
void set_content_type();
void set_extentions();
void open_file();
public:    
    Get();
    Get(const Get& oth);
    Get& operator=(const Get& oth);
    int process(std::string body, size_t body_size, int event);
    ~Get();
};

#endif