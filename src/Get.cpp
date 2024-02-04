#include "Get.hpp"
#include <sys/stat.h>


Get::Get(){
    set_extentions();
    cmds = new char *[3];
    cmds[0] = NULL;
    cmds[1] = NULL;
    cmds[2] = NULL;
    env = new char *[4];
    env[0] = NULL;
    env[1] = NULL;
    env[2] = NULL;
    env[3] = NULL;
    end = 0;
    opened = 0;
    cgi_execueted = 0;
}

Get::Get(const Get& oth){
    set_extentions();
    *this = oth;
}

Get& Get::operator=(const Get& oth){
    if (this != &oth){
        serv = oth.serv;
        http_v = oth.http_v;
        uri = oth.uri;
        headers = oth.headers;
        req_path = oth.req_path;
        fullUri_path = oth.fullUri_path;
    }
    return *this;
}

void Get::set_extentions(){
    types["html"] = "text/html";
    types["htm"] = "text/html";
    types["css"] = "text/css";
    types["jpeg"] = "image/jpeg";
    types["jpg"] = "image/jpeg";
    types["png"] = "image/png";
    types["gif"] = "image/gif";
    types["json"] = "application/json";
    types["mp4"] = "video/mp4";
    types["mp3"] = "audio/mpeg";
    types["js"] = "application/javascript";
    types["bmp"] = "image/bmp";
    types["ico"] = "image/x-icon";
    types["pdf"] = "application/pdf";
    types["txt"] = "text/plain";
    types["xml"] = "application/xml";
    types["zip"] = "application/zip";
    types["tar"] = "application/x-tar";
    types["gz"] = "application/gzip";
}

int Get::extension_search(const string& f_name){
    extension = "";
    size_t tmp = f_name.find(".");
    pos = tmp;
    while (tmp != string::npos){
        pos = tmp;
        tmp = f_name.find(".",pos+1);
    }
    if (pos != string::npos && pos+1 < f_name.size()){
        extension = f_name.substr(pos+1);
        return 1;
    }
    return 0;
}

void Get::set_content_type(const string& file_name){
    extension_search(file_name);
    if (serv.Is_cgi){
        content_type = "text/html";
    }
    cout<<"extension1: "<<extension<<endl;
    if (types.find(extension) != types.end())
        content_type = types.find(extension)->second;
    else
        content_type = "application/octet-stream";
}

void Get::set_headers(const string& file_name){
    int hed = 0;
    set_content_type(file_name);
    respons = "HTTP/1.1 200 OK\r\nContent-Type: ";
    respons +=  content_type+string("\r\n");
    respons +=  string("Content-Length: ");
    string line;
    getline(src_file,line);
    head_size = line.size()+1;
    while (line.size() && line[line.size()-1] == '\r' && !hed){
        cout<<"|"<<(int)line[line.size()-1]<<"|"<<endl;
        getline(src_file,line);
        head_size += line.size()+1;
        if (line.size() && line == "\r")
            hed = 1;
    }
    if (hed)
        file_len -= head_size;
    stringstream ss;
    ss<<file_len;
    respons += ss.str()+string("\r\n");
    if (!hed)
        respons += "\r\n";
    src_file.seekg(0, std::ios::beg);
}

void Get::open_file(const string& file_name){
    src_file.open(file_name.c_str(), ios::in);
    opened = 1;
    if (!src_file.is_open()){
        opened = -1;
        // cout<<"can't open file: "<<file_name<<endl;
        return;
    }
    src_file.seekg(0, std::ios::end);
    file_len = src_file.tellg();
    src_file.seekg(0, std::ios::beg);
    set_headers(file_name);
    // cout<<"full_path: "<<fullUri_path<<endl;
    // cout<<"uri: "<<uri<<endl;
    cout<<"content_len: "<<file_len<<endl;
    cout<<"content_type: "<<content_type<<endl;
}

void Get::get(const string& file_name){
    ssize_t r_len, max_r = 1000;
    
    if (!opened)
        open_file(file_name);
    else 
    if (opened == 1){
        string res;
        res.resize(max_r);
        src_file.read(&res[0], max_r);
        r_len = src_file.gcount();
        res.resize(r_len);
        respons += res;
        if (src_file.eof())
            end = 1;
        cout<<"res_size = "<<respons.size()<<"\nres: <<"<<respons<<">>"<<endl;
    }
    if (opened == -1){
        opened = 0;
        end = 1;
        get(serv.error_page["404"]);
    }
    if (end)
        src_file.close();
}

int Get::process(string _body, size_t _body_size){
    body = _body;
    body_size = _body_size;
    respons.clear();
    if (serv.Is_cgi)
        get_bycgi();
    else
        get(fullUri_path);
    return(0);
}

void Get::get_bycgi(){
    if (!cgi_execueted)
        exec_cgi();
    else
        get(string("out.html"));
}

void Get::exec_cgi(){
    FILE *file;
    int pid;
    char * cmd;
    int exit_stat;

    set_cmd();
    set_env();
    cmd = cmds[0];
    pid = fork();
    if(pid < 0){
      perror("fork fail");
      exit(EXIT_FAILURE);
    }
    if (pid == 0){
        // cout<<"file_name: "<<cmds[1]<<endl;
        file = fopen("out.html", "w");
        dup2(file->_fileno,STDOUT_FILENO);
        if(execve(cmd,cmds,env) == -1){
            std::cout<<"No exec\n";
            perror("execve");
            exit(1);
        }
    }
    waitpid(pid,&exit_stat,0);
    cout<<WEXITSTATUS(exit_stat)<<endl;
    cgi_execueted = 1;
}

void Get::set_env(){
    string senv("QUERY_STRING=");

    senv += serv.querys;
    env[0] = new char[senv.length()+1];
    strcpy(env[0],senv.c_str());
    senv = "PATH_INFO=";
    senv += "None";
    env[1] = new char[senv.length()+1];
    strcpy(env[1],senv.c_str());
    // senv = "REQUEST_METHOD=";
    // senv += "GET";
    // env[2] = new char[senv.length()+1];
    // strcpy(env[2],senv.c_str());
}

void Get::set_cmd(){

    extension_search(fullUri_path);
    cout<<"cgi_exten: "<<extension<<endl;
    map<string,string>::iterator it;
    it = serv.UriLocation.cgi_path.find(extension);
    if (it == serv.UriLocation.cgi_path.end()){
        cerr<<"No Cgi Extension"<<endl;
        end = 1;
        get(serv.error_page["404"]);
        return ;
    }
    string cmdCgi = it->second;
    cmds[0] =  new char[cmdCgi.length() + 1];
    strcpy(cmds[0],cmdCgi.c_str());
    cmds[1] = new char[ fullUri_path.length() + 1];
    strcpy(cmds[1],fullUri_path.c_str());
}

Get::~Get(){
    for (int i = 0; cmds[i]; i++)
	    delete [] cmds[i];
    delete [] cmds;

    for (int i = 0; env[i]; i++)
	    delete [] env[i];
    delete [] env;
}