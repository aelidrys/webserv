/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouassi <abouassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 17:43:44 by abouassi          #+#    #+#             */
/*   Updated: 2024/01/08 09:25:46 by abouassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Servers.hpp"

Location & Servers::getLocation(std::string path)
{
    size_t i;
    for (i = 0; i < loactions.size(); i++)
    {
        if(loactions[i].path[0] == path )
        {
            return loactions[i];
        }
        
    }
    return loactions[i];
}


void Servers::ParceServers()
{
    std::vector<std::string> vec;
    for (size_t i = 0; i < servconf.size(); i++)
    {
        if (servconf[i][0] == "server" || servconf[i][0] == "location" ||  servconf[i][0] == "}" ||  servconf[i][0] == "{" )
        {
            vec.push_back(servconf[i][0]);
        }
        
    }
    std::vector<std::string>::iterator iter;
    std::vector<std::string>::iterator loc;
    iter = vec.begin();
    iter++;
    if(iter != vec.end() && *iter != "{")
    {
         throw "error no open { for server \n";
    }
    iter++;
    while (iter != vec.end())
    {
        if (*iter == "{")
        {
            throw "error no open\n";
        }
        
        if (*iter == "}" && iter + 1 != vec.end())
        {
            throw "error no close here\n";
        }
        loc = std::find(iter,vec.end(),"location");
        
        if (loc != vec.end())
        {
            loc++;
            if (loc != vec.end() && *loc !=  "{")
            {
                throw "error no open { for location\n";
            }
            loc++;
            if (loc != vec.end() && *loc !=  "}")
            {
                throw "error no open } for location\n";
            }
            loc++;
            if (loc == vec.end() || (*loc == "}" && loc + 1 != vec.end()))
            {
                throw "error for closeb\n";
            }
        }
        iter = loc;
    }
}


int Servers::pathExists(std::string path) {
    struct stat fileStat;
    return stat(path.c_str(), &fileStat) == 0;
}

bool Servers::check_isdigit(std::string str)
{
    int i = 0;
    while (str[i])
    {
        if (!isdigit(str[i]))
        {
            return (0);
        }
        i++;
    }
    return (1);
}

void Servers::Printtwodom(const std::vector<std::vector<std::string> > & matrix,std::string data)
{
    for (std::vector<std::vector<std::string> >::const_iterator row = matrix.begin(); row != matrix.end(); ++row) {
        std::cout<<data<<"   :";
    for (std::vector<std::string>::const_iterator value = row->begin(); value != row->end(); ++value) {
        std::cout << *value << "|";
    }
    std::cout << std::endl;
}
    
}

/*_____________________________________________________________*/
/*_________________________SET_________________________________*/
/*_____________________________________________________________*/

void Servers::parceIp(std::string ip)
{
    std::stringstream ss;
    ss << ip;
    std::string line;
    int net = 0;
    int len = 0;
    while (getline(ss,line,'.'))
    {
        net = std::atoi(line.c_str()) ;
        if (line.size() > 3 || !check_isdigit(line) || net > 255 || net < 0)
        {
            throw "error invalid ip host "+ip+" \n";
        }
        len++;
    }
    if (len != 4)
        throw "error invalid ip host "+ip+" \n";

}
int Servers::checkDup(std::string der,int & index)
{
    int dup = 0;
    size_t i = 0;
    while ( i < GetIndex("location"))
    {
        if (servconf[i][0] == der)
        {
            index = i;
            dup++;
        }
        if (dup > 1)
        {
           throw "Error duplicate derective \n";
        }
        i++;
    }
    return (dup);
}
void Servers::SetPorts ()
{
    int i;
    int num = checkDup("listen",i);
    std::string arg;
    if (num == 0)
    {
        return ;
    }
    if (servconf[i].size() != 2 )
    {
         throw "invalid port in of the listen directive \n";
    }
    arg = servconf[i][1];
    int myport = std::atoi(arg.c_str());
    if (!check_isdigit(arg)  || myport > 65535)
    {
        throw  ("invalid port in '" + arg + "' of the  directive \n");
    }
    port.push_back(myport);
}

void Servers::SetServerName()
{
    int i;
    int num = checkDup("server_name",i);
    std::string arg;
    if (num == 0)
    {
        return ;
    }
    if (servconf[i].size() != 2 )
    {
         throw "invalid port  directive \n";
    }
    arg = servconf[i][1];

    //check hna laykon chi check
    server_name.push_back(arg);
}


void Servers::SetHost()
{
    int i;
    int num = checkDup("host",i);
    std::string arg;
    if (num == 0)
    {
        return ;
    }
    if (servconf[i].size() != 2 )
    {
         throw "invalid host in  directive \n";
    }
    arg = servconf[i][1];
    parceIp(arg);
    host.push_back(arg);
}
void Servers::SetRoot()
{
    int i;
    int num = checkDup("root",i);
    std::string arg;
    if (num == 0)
    {
        return ;
    }
    if (servconf[i].size() != 2 )
    {
         throw "invalid root directive \n";
    }
    arg = servconf[i][1];
    if (!pathExists(arg)) 
    {
        throw ("Path '"+arg+"' does not exist.\n");
    }
    root.push_back(arg);
}
void Servers::SetIndex()
{
    int i;
    int num = checkDup("index",i);
    std::string arg;
    if (num == 0)
    {
        return ;
    }
    if (servconf[i].size() != 2 )
    {
         throw "invalid port in of the server_name directive \n";
    }
    arg = servconf[i][1];
    if (!pathExists(arg)) 
    {
       throw ("Path '"+arg+"' does not exist.\n");
    }
    index.push_back(arg);
}
void Servers::SetClient_max_body_size()
{
    int i;
    int num = checkDup("client_max_body_size",i);
    std::string arg;
    if (num == 0)
    {
        return ;
    }
    if (servconf[i].size() != 2 )
    {
         throw "invalid client_max_body_size  directive \n";
    }
    arg = servconf[i][1];
    long long int  body_size = std::strtod(arg.c_str(),NULL);
    if (!check_isdigit(arg))
    {
        throw  "invalid port in '"+ arg +"' of the  directive \n";
    }   
    client_max_body_size.push_back(body_size);
}

void Servers::check_Status(std::string status)
{
    std::vector<std::string>::iterator iter;

    iter = std::find(s_erorr.begin(),s_erorr.end(),status);
    if (iter == s_erorr.end())
    {
        throw "Error : Invalid status '"+status +"' code  Derecties\n";
    }
}
std::vector<std::string>  Servers::AddErrorPage(std::string status,std::string path)
{
    std::vector<std::string> vErrorPage;
    vErrorPage.push_back(status);
    vErrorPage.push_back(path);

    return vErrorPage;
}
void Servers::ReplacePath(std::string status,std::string path)
{
    for (size_t i = 0; i < error_page.size(); i++)
    {
        if (error_page[i][0] == status)
        {
            error_page[i][1] = path;
            return ;
        }
        
    }
    
}
void Servers::SetError_page()
{
    std::string status;
    std::string path;
    for (size_t i = 0; i < servconf.size(); i++)
    {
        if (servconf[i][0] == "error_page")
        {
            if (servconf[i].size() != 3)
            {
                throw "Error in error_page\n";
            }
            status = servconf[i][1];
            path = servconf[i][2];
            check_Status(status);
            if (!pathExists(path)) 
            {
                throw ("Path '"+path+"' does not exist.\n");
            }
            ReplacePath(status,path);
        }
    }
}

void Servers::SetAllDir()
{
    ParceServers();
    FillValid();
    FillStatus();
    FillLocation();
    checkValidation();
    
     
    SetHost();
    SetRoot();
    SetPorts();
    SetIndex();
    SetServerName();
    SetError_page();
    SetClient_max_body_size();
    
    size_t i = 0;
    while (i < loactions.size())
    {
        loactions[i].SetAllDir();
        i++;
    }
    
}
/*_____________________________________________________________*/
/*_________________________GET_________________________________*/
/*_____________________________________________________________*/

const std::vector<int> & Servers::GetPorts ()
{
    return port;
}
const std::vector<std::string> & Servers::GetServerName()
{
    return server_name ;
} 
const std::vector<std::string> & Servers::GetHost()
{
    return host;
}
const std::vector<std::string> & Servers::GetRoot()
{
    return root;
}
const std::vector<std::vector<std::string> >&  Servers::GetError_page()
{
    return error_page;
}
const std::vector<long long int > &  Servers::GetClient_max_body_size()
{
    return client_max_body_size;
}
const std::vector<std::string> & Servers::GetIndex()
{
    return index;
}



/*=======================================================================*/
/*=======================================================================*/
size_t Servers::GetIndex(std::string dir)
{
    size_t i ;
    for (i = 0; i < servconf.size(); i++)
    {
        if (servconf[i][0] == dir)
        {
            return i;
        }
    }
    return (i);
}
Location Servers::FirstFill(size_t & index)
{
    Location loaction;
    loaction.vlocation.push_back(servconf[index]);
    index++;
    while (index < servconf.size() && servconf[index][0] != "location")
    {
        // std::cout<<index<<"  :"<<servconf[index][0]<<std::endl;
        loaction.vlocation.push_back(servconf[index]);
        index++;
    }
    return loaction;
}


void Servers::FillLocation()
{
    size_t index = GetIndex("location");

    if (index == servconf.size())
    {
        return ;
    }
    while (index < servconf.size())
    {
        loactions.push_back(FirstFill(index));
    }
}
/*=======================================================================*/
/*=======================================================================*/

void Servers::FillStatus()
{
    s_erorr.push_back("400");
    s_erorr.push_back("401");
    s_erorr.push_back("402");
    s_erorr.push_back("403");
    s_erorr.push_back("404");
    s_erorr.push_back("405");
    s_erorr.push_back("406");
    s_erorr.push_back("407");
    s_erorr.push_back("408");
    s_erorr.push_back("409");
    s_erorr.push_back("410");
    s_erorr.push_back("411");
    s_erorr.push_back("412");
    s_erorr.push_back("413");
    s_erorr.push_back("414");
    s_erorr.push_back("415");
    s_erorr.push_back("416");
    s_erorr.push_back("417");
    s_erorr.push_back("418");
    s_erorr.push_back("421");
    s_erorr.push_back("422");
    s_erorr.push_back("423");
    s_erorr.push_back("424");
    s_erorr.push_back("425");
    s_erorr.push_back("426");
    s_erorr.push_back("428");
    s_erorr.push_back("429");
    s_erorr.push_back("431");
    s_erorr.push_back("451");
    s_erorr.push_back("500");
    s_erorr.push_back("501");
    s_erorr.push_back("502");
    s_erorr.push_back("503");
    s_erorr.push_back("504");
    s_erorr.push_back("505");
    s_erorr.push_back("506");
    s_erorr.push_back("507");
    s_erorr.push_back("508");
    s_erorr.push_back("510");
    s_erorr.push_back("511");

}
void Servers::FillValid()
{
     Vstrvalid.push_back("listen");
     Vstrvalid.push_back("server");
     Vstrvalid.push_back("server_name");
     Vstrvalid.push_back("host");
     Vstrvalid.push_back("root");
     Vstrvalid.push_back("error_page");
     Vstrvalid.push_back("client_max_body_size");
     Vstrvalid.push_back("index");
     Vstrvalid.push_back("location");
     Vstrvalid.push_back("{");
     Vstrvalid.push_back("}");
     Vstrvalid.push_back("#");
     Vstrvalid.push_back("allow_methods");
     Vstrvalid.push_back("autoindex");
     Vstrvalid.push_back("upload");
     Vstrvalid.push_back("cgi_path");
}
void Servers::checkValidation()
{
    std::vector<std::string>::iterator iter;
    for (size_t i = 0; i < GetIndex("location"); i++)
    {
        iter = std::find(Vstrvalid.begin(),Vstrvalid.end(),servconf[i][0]);
        if (iter == Vstrvalid.end())
        {
            throw "Error : Invalid Derecties\n";
        }
    }
    
}

void Servers::desplay()
{
    // std::vector<std::vector<std::string> > matrix = servconf;
    
Print_dirs(GetPorts().begin(),GetPorts().end(),"Ports");
Print_dirs(GetServerName().begin(),GetServerName().end(),"Server_name");
Print_dirs(GetHost().begin(),GetHost().end(),"Host");
Print_dirs(GetRoot().begin(),GetRoot().end(),"Root");
Printtwodom(GetError_page(),"Error_page");
Print_dirs(GetClient_max_body_size().begin(),GetClient_max_body_size().end(),"Client_max_body_size");
Print_dirs(GetIndex().begin(),GetIndex().end(),"Index");
size_t i = 0;
while (i < loactions.size())
{
    std::cout<<"________________________\n";
    loactions[i].desplayLocation();
    i++;
}
   
}

/*#############################################################*/
                     /*CREATE SOKCET*/
/*#############################################################*/

void Servers::CreatSocketServer()
{
    
    if ((server_fd = socket(AF_INET , SOCK_STREAM,0) )< 0)
    {
        perror("connot create socket");
        return ;
    }

    int reuseaddr = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( port[0] );
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    if (bind(server_fd,(struct sockaddr *)&address,sizeof(address)) < 0) 
    { 
        perror("bind failed"); 
        return ; 
    }
    if (listen(server_fd, 3) < 0)
    { 
        perror("“In listen”");
        exit(EXIT_FAILURE);
    }

}
/*#############################################################*/
void Servers::SetDefaultError()
{
    error_page.push_back(AddErrorPage("400","400.html"));
    error_page.push_back(AddErrorPage("401","401.html"));
    error_page.push_back(AddErrorPage("402","402.html"));
    error_page.push_back(AddErrorPage("403","403.html"));
    error_page.push_back(AddErrorPage("404","404.html"));
    error_page.push_back(AddErrorPage("405","405.html"));
    error_page.push_back(AddErrorPage("406","406.html"));
    // error_page.push_back(AddErrorPage("407","407.html"));
    // error_page.push_back(AddErrorPage("408","408.html"));
    // error_page.push_back(AddErrorPage("409","409.html"));
    // error_page.push_back(AddErrorPage("410","410.html"));
    // error_page.push_back(AddErrorPage("411","411.html"));
    // error_page.push_back(AddErrorPage("412","412.html"));
    // error_page.push_back(AddErrorPage("413","413.html"));
    // error_page.push_back(AddErrorPage("414","414.html"));
    // error_page.push_back(AddErrorPage("415","415.html"));
    // error_page.push_back(AddErrorPage("416","416.html"));
    // error_page.push_back(AddErrorPage("417","417.html"));
    // error_page.push_back(AddErrorPage("418","418.html"));
    // error_page.push_back(AddErrorPage("421","421.html"));
    // error_page.push_back(AddErrorPage("422","422.html"));
    // error_page.push_back(AddErrorPage("423","423.html"));
    // error_page.push_back(AddErrorPage("424","424.html"));
    // error_page.push_back(AddErrorPage("425","425.html"));
    // error_page.push_back(AddErrorPage("426","426.html"));
    // error_page.push_back(AddErrorPage("428","428.html"));
    // error_page.push_back(AddErrorPage("429","429.html"));
    // error_page.push_back(AddErrorPage("431","431.html"));
    // error_page.push_back(AddErrorPage("451","451.html"));
    // error_page.push_back(AddErrorPage("500","500.html"));
    // error_page.push_back(AddErrorPage("501","501.html"));
    // error_page.push_back(AddErrorPage("502","502.html"));
    // error_page.push_back(AddErrorPage("503","503.html"));
    // error_page.push_back(AddErrorPage("504","504.html"));
    // error_page.push_back(AddErrorPage("505","505.html"));
    // error_page.push_back(AddErrorPage("506","506.html"));
    // error_page.push_back(AddErrorPage("507","507.html"));
    // error_page.push_back(AddErrorPage("508","508.html"));
    // error_page.push_back(AddErrorPage("510","510.html"));
    // error_page.push_back(AddErrorPage("511","511.html"));
}
Servers::Servers()
{
    SetDefaultError();
    
}

Servers::~Servers()
{
    
}