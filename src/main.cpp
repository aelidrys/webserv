#include "multiblex.hpp"

void test(const string& file_name)
{
    request req;
    ifstream in_file;
    string str;
    string str_req;
    in_file.open(file_name, ios::in);
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
    // int fd = open("request.txt", O_RDWR);
    // int b_size = 1000, reading_s;
    // string str;
    // str.resize(b_size);
    // for (size_t i = 0; i < 10; i++)
    // {
    //     reading_s = read (fd, &str[0], b_size);
    //     if (!reading_s)
    //         cout<<"readina ZIROOO"<<endl;
    //     else
    //         cout<<"readina : "<<reading_s<<endl;
    // }
    
    // test("request.txt");
}