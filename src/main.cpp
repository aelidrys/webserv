#include "request.hpp"

void test(const std::string& file_name)
{
    request req;
    std::ifstream in_file;
    std::string str;
    std::string str_req;
    in_file.open(file_name, std::ios::in);
    if (in_file.fail()){
        std::cout << "ERROR: in input file\n";
        return ;
    }

    while (std::getline(in_file, str)){
        str_req += str;
        str_req += "\n";
    }
    std::cout<< "befor secfault" << std::endl;
    std::string s = "aa aa aa\nbbb4: bb\nccc-c: cc\nddd_d: dd\r\n\r\neeeeee\nmmmmmm\nl\0l\0ss\n";
    req.parce_req(str_req);
    req.show_inf();
    in_file.close();

}

int main(){
    m_server();
    // std::string s1;
    // std::string s2;
    // std::stringstream ss;
    // ss<<s;
    // while (ss.peek() != -1){
    //     getline(ss,s1);
    //     s2 += s1;
    // }
    // s1 = ss.str();
    // test("request.txt");
    

    // std::cout <<"|"<< ss.str() <<"|"<< std::endl;
    // std::cout <<"<"<< s2 <<">"<< std::endl;
}