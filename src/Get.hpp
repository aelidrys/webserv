#include "Method.hpp"

using namespace std;


#ifndef GET_HPP
#define GET_HPP

class Get : public Method
{
string body;
int body_size;
public:
    Get();
    Get(const Get& oth);
    Get& operator=(const Get& oth);
    void process(std::string body, size_t body_size);
    ~Get();
};

#endif