/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouassi <abouassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 18:23:05 by abouassi          #+#    #+#             */
/*   Updated: 2024/01/18 15:33:15 by abouassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP


#include <algorithm> 
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include <map>
#include <sys/stat.h>
class Location
{
private:


        void parce_location();  
        void CheckMethods(std::string methd);
        std::vector<std::string>  AddcgiPaths(std::string status,std::string path);
        size_t GetIndex(std::string dir);
        int  checkDup(std::string der,int & index);

        void FillValid();
        void checkValidation();
        std::vector<std::string> Vstrvalid;

        void SetPath();
        void SetAllowMethods();
        void SetAutoindex();
        void SetUpload();
        void SetCgiPath();
        void SetRoot();
        void Printtwodom(const std::vector<std::vector<std::string> > & matrix,std::string data);
        int pathExists(std::string path);
public:
        std::vector<std::string> path;
        std::vector<std::string> root;
        std::vector<std::string >  allow_methods;
        bool autoindex ;
        bool upload;
        std::vector<std::vector<std::string > >  cgi_path;
        bool get;
        bool post;
        bool dele;

        /*________________Cgi___________________*/
        std::string getCmdCgi(std::string & exta);


        
        void desplayLocation();
        void SetAllDir();
        const std::vector<std::string> & GetPath();
        const std::vector<std::string> & GetAllowMethods();
        const bool & GetAutoindex();
        const bool & GetUpload();
        const std::vector<std::vector<std::string> > & GetCgiPath();
        const std::vector<std::string> & GetRoot();
        
        std::vector<std::vector<std::string> > vlocation;


        // void CheckisDup(std::vector<std::string> vec , std::string elemnt);
        template<typename T,typename V>
        void Print_dirs(T begin,T end,V data)
        {
            while(begin != end)
            {
                std::cout<<data<<"  :"<<*begin<<std::endl;
                begin++;
            }
        }


        Location();
        ~Location();
};





#endif