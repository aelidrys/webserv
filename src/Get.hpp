#include "Method.hpp"

using namespace std;


#ifndef GET_HPP
#define GET_HPP


class Get : public Method
{
int fd;
int body_size;
int opened;
int cgi_execueted;
string body;
size_t file_len;
fstream src_file;
map<string,string> types;
string content_type;

string extension;
char **cmds;
// char **env;

void set_content_type(const string& file_name);
void set_extentions();
void open_file(const string& file_name);
void get(const string& file_name);
void get_bycgi();
void exec_cgi();
void set_cmd();
public:
    Get();
    Get(const Get& oth);
    Get& operator=(const Get& oth);
    int process(std::string body, size_t body_size, int event);
    ~Get();
};

#endif