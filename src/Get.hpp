#include "Method.hpp"
#include <cstring>

using namespace std;


#ifndef GET_HPP
#define GET_HPP


class Get : public Method
{
int body_size;
int head_size;
int opened;
int cgi_execueted;
size_t pos;
string body;
size_t file_len;
fstream src_file;
map<string,string> types;
string content_type;


string extension;
char **cmds;
char **env;

void set_content_type(const string& file_name);
void set_extentions();
void open_file(const string& file_name);
void get(const string& file_name);
void get_bycgi();
void exec_cgi();
void set_cmd();
void set_env();
void set_headers(const string& file_name);
int extension_search(const string& f_name);
public:
    Get();
    Get(const Get& oth);
    Get& operator=(const Get& oth);
    int process(std::string body, size_t body_size);
    ~Get();
};

#endif