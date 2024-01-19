#include "multiblex.hpp"

void test(const string& file_name)
{
    Request req;
    ifstream in_file;
    string str;
    string str_req;
    in_file.open(file_name.c_str(), ios::in);
    if (in_file.fail()){
        cout << "ERROR: in input file\n";
        return ;
    }

    while (getline(in_file, str)){
        str_req += str;
        str_req += "\n";
    }
    cout<< "befor secfault" << endl;
    string s = "aa aa aa\nbbb4: bb\nccc-c: cc\nddd_d: dd\r\n\r\neeeeee\nmmmmmm\nl\0l\0ss\n";
    req.parce_req(str_req);
    req.show_inf();
    in_file.close();

}

int main(){
    multiblex mlt;
    mlt.m_server();
}