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

void Get::set_content_type(const string& file_name){
    size_t pos, tmp = file_name.find(".");
    pos = tmp;
    while (tmp != string::npos){
        pos = tmp;
        tmp = file_name.find(".",pos+1);
    } 
    if (serv.Is_cgi){
        string s1;
        content_type = "text/html";
        getline(src_file,s1);
        getline(src_file,s1);
    }
    if (pos != string::npos && pos+1 < file_name.size()){
        cout<<"extension: "<<file_name.substr(pos+1)<<endl;
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
        // cout<<"can't open file: "<<file_name<<endl;
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
    // cout<<"full_path: "<<fullUri_path<<endl;
    // cout<<"uri: "<<uri<<endl;
    cout<<"content_len: "<<file_len<<endl;
    cout<<"content_type: "<<content_type<<endl;
}

void Get::get(const string& file_name){
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
        respons = res;
    }
    if (opened == -1){
        opened = 0;
        get(serv.error_page["404"]);
    }
    if (end)
        src_file.close();
}

int Get::process(string _body, size_t _body_size, int event){
    body = _body;
    body_size = _body_size;
    respons.clear();
    (void)event;
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
        get("out.html");
}

void Get::exec_cgi(){
    FILE *file;
    int pid;
    char * cmd;

    set_cmd();
    cmd = cmds[0];
    pid = fork();
    if(pid < 0){
      perror("fork fail");
      exit(EXIT_FAILURE);
    }
    if (pid == 0){
        cout<<"file_name: "<<cmds[1]<<endl;
        file = fopen("out.html", "w");
        dup2(file->_fileno,STDOUT_FILENO);
        cout<<"execve\n";
        if(execve(cmd,cmds,NULL) == -1){
            std::cout<<"No exec\n";
            perror("execve");
            cgi_execueted = 1;
        }
    }
    waitpid(pid,NULL,0);
    cgi_execueted = 1;
}

void Get::set_env(){
    string senv("QUERY_STRING=");

    senv += serv.querys;
    env[0] = (char *)senv.data();
    senv = "PATH_INFO";
    senv += "";
    env[1] = (char *)senv.data();
    senv = "";
    env[1] = (char *)senv.data();
}

void Get::set_cmd(){
    extension = "";
    size_t pos, tmp = fullUri_path.find(".");
    pos = tmp;
    while (tmp != string::npos){
        pos = tmp;
        tmp = fullUri_path.find(".",pos+1);
    }
    if (pos == string::npos){
        // an Error_page should be returned
        cerr<<"Error No Extension detected"<<endl;
        return ;
    }
    extension.append(&fullUri_path[pos]);
    map<string,string>::iterator it;
    it = serv.UriLocation.cgi_path.find(extension);
    if (it == serv.UriLocation.cgi_path.end()){
        cerr<<"No Cgi Extension"<<endl;
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
    {
	    delete [] cmds[i];
    }
    delete [] cmds;
    delete [] env;
}