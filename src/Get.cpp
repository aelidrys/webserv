#include "Get.hpp"
#include <sys/stat.h>


Get::Get(){
    set_extentions();
    cmds = new char *[3];
    cmds[0] = NULL;
    cmds[1] = NULL;
    cmds[2] = NULL;
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

void Get::set_content_type(const string& file_name){
    size_t pos = file_name.find(".");
    if (serv.Is_cgi){
        string s1;
        content_type = "text/html";
        getline(src_file,s1);
        getline(src_file,s1);
    }
    if (pos != string::npos && pos+1 < file_name.size()){
        if (types.find(file_name.substr(pos+1)) != types.end())
            content_type = types.find(file_name.substr(pos+1))->second;
        else
            content_type = "application/octet-stream";
    }
    else
        content_type = "application/octet-stream";
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

void Get::open_file(const string& file_name){
    src_file.open(file_name.c_str(), ios::in);
    opened = 1;
    if (!src_file.is_open()){
        opened = -1;
        cout<<"can't open file: "<<file_name<<endl;
        return;
    }
    src_file.seekg(0, std::ios::end);
    file_len = src_file.tellg();
    src_file.seekg(0, std::ios::beg);
    set_content_type(file_name);
    respons = "HTTP/1.1 200 OK\r\nContent-Type: ";
    respons +=  content_type+string("\r\nContent-Length: ");
    stringstream ss;
    ss<<file_len;
    respons += ss.str();
    respons += string("\r\n\r\n");
    cout<<"full_path: "<<fullUri_path<<endl;
    // cout<<"uri: "<<uri<<endl;
    cout<<"content_len: "<<file_len<<endl;
    cout<<"content_type: "<<content_type<<endl;
}

void Get::get(const string& file_name){
    respons = "";
    if (!opened)
        open_file(file_name);
    ssize_t r_len, max_r = 1000;

    if (opened == 1){
        string res;
        res.resize(max_r);
        src_file.read(&res[0], max_r);
        r_len = src_file.gcount();
        if (r_len < max_r)
            end = 1;
        res.resize(r_len);
        respons += res;
    }
    if (opened == -1){
        opened = 0;
        get("/nfs/homes/aelidrys/Desktop/webserv/abouassi/error_pages/404.html");
    }
}

int Get::process(string _body, size_t _body_size, int event){
    body = _body;
    body_size = _body_size;

    if (event == EPOLLIN)
        return(0);
    if (serv.Is_cgi){
        // cout<<"Cgi"<<endl;
        get_bycgi();
    }
    else
        get(fullUri_path);
    return(0);
}

void Get::get_bycgi(){
    cout<<"cgi_statue: ";
    // if (cgi_execueted)
    //     cout<<"cgi executed"<<endl;
    if (!cgi_execueted){
        // cout<<"cgi not executed"<<endl;
        exec_cgi();
    }
    else
        get("out.html");
}

void Get::exec_cgi(){
    set_cmd();
    fd = open("out.html",O_CREAT | O_RDWR | O_TRUNC,0644);
    std::cout<<"cgi_fd = "<<fd<<endl;
    char * cmd;
    cmd = cmds[0];
    int pid = fork();
    if(pid < 0){
      perror("fork fail");
      exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        dup2(fd, STDOUT_FILENO);
        if(execve(cmd,cmds,NULL) == -1)
        {
            std::cout<<"No exec\n";
            perror("execve");
            exit(1);
            
        }
    }
    waitpid(pid,NULL,0);
    close(fd);
    cgi_execueted = 1;
}

void Get::set_cmd(){
    extension = "";
    size_t pos = fullUri_path.find(".");
    if (pos == string::npos){
        cerr<<"Error No Extension detected"<<endl;
        // an Error should be returned
    }
    else
        extension.append(&fullUri_path[pos]);
    string cmdCgi = serv.UriLocation.getCmdCgi(extension);
    cmds[0] =  new char[cmdCgi.length() + 1];
    strcpy(cmds[0],cmdCgi.c_str());
    cmds[1] = new char[ fullUri_path.length() + 1];
    strcpy(cmds[1],fullUri_path.c_str());
    
    // cout<<"cmd :"<<cmds[0]<<endl;
    // cout<<"arg :"<<cmds[1]<<endl;
    cmds[2] = NULL;
}

Get::~Get(){
    cout<<"DESTRUCTOR IN"<<endl;
    for (int i = 0; cmds[i]; i++)
    {
	    delete [] cmds[i];
    }
    delete [] cmds;
    cout<<"DESTRUCTOR OUT"<<endl;
}