#ifndef __Resource_H__
#define __Resource_H__

#include "ResourceUtils.h"
#include <stdio.h>
#include <string>
#include <cstring>

namespace ResourceUtils
{
    const char* path;
    
    const char* getPathForSpecifiedResource(const char* filename, const char* aFileExtension)
    {

        char buf[100];
        memset(buf, 0, sizeof(buf));
       // char* buf =new char[100];


        strcat(buf,path);
        strcat(buf,"/");
        strcat(buf,filename);
        strcat(buf,".");
        strcat(buf,aFileExtension);
       // std::string buff;
      //  buff =  buf;
        char* Items[100] ;

        Items[0] = new char[100] ; // We have to allocate for each Item memory.

        std::strcpy(*Items,buf);
        return *Items;//buff.c_str();
    }
    
    void setPathForSpecifiedResource(const char* appPath)
    {
        path = appPath;
    }
    
   
}


#endif
