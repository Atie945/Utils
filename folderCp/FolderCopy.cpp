#include <stdio.h>
#include <iostream>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "FolderCopy.h"

FolderCopy::FolderCopy(const std::string& source, const std::string& dest):m_source(source),m_dest(dest){

}

void FolderCopy::startCopy()
{
    if(!isDir(m_source))
    {
        std::cout << "error: invalid path!" <<std::endl;
        return ;
    }
    createDestDir();
    copyDir(m_source, m_dest);
}

void FolderCopy::createDestDir(){
    mkdir(m_dest.c_str(), 0755);
}

void FolderCopy::copyDir(const std::string& source,const std::string& dest)
{
    DIR *pDir;
    struct dirent *entry;
    std::string nodeName, preDir, outDir, inputDir = source;

    if (isDir(inputDir.c_str()) == false) return;

    if(pDir = opendir(inputDir.c_str()))
    {
        while(entry = readdir(pDir))
        {
            nodeName = entry->d_name;
            if(strcmp(entry->d_name, ".")  != 0 && strcmp(entry->d_name, "..") != 0 )
            {
                preDir = inputDir;
                preDir.append("/");
                preDir.append(nodeName);

                //std::cout << "currnt node is : " << entry->d_name<<std::endl;
                if (isDir(preDir.c_str()))
                {
                    outDir = dest;
                    outDir.append("/");
                    outDir.append(nodeName);
                    mkdir(outDir.c_str(), 0755);

                    copyDir(preDir.c_str(), outDir);
                }
                else
                {
                    outDir = dest;
                    outDir.append("/");
                    outDir.append(nodeName);
                    copyFile(preDir.c_str(), outDir.c_str());
                    //std::cout << "copy from "<< preDir.c_str() << " to " << outDir.c_str() << std::endl;
                }
            }
        }
        closedir(pDir);
    }
}

bool FolderCopy::isDir(const std::string& path)
{
    struct stat buf;
    stat(path.c_str(), &buf);
    return S_ISDIR(buf.st_mode);
}

void FolderCopy::copyFile(const std::string& source,const std::string& dest)
{
    char buffer[128];
    FILE *in,*out;

    if((in=fopen(source.c_str(),"r"))==NULL){
      //  std::cout <<"open source file failed\n"<<std::endl;
        return;
    }

    if((out=fopen(dest.c_str(),"w"))==NULL){
       // std::cout <<"create file failed\n"<<std::endl;
        return;
    }

    int len;
    while((len=fread(buffer,1,128,in))>0){
        fwrite(buffer,1,len,out);
    }

    fclose(out);
    fclose(in);
}


int main(int argc, char **argv)
{
    FolderCopy *p = new FolderCopy(argv[1], argv[2]);
    p->startCopy();
    delete p;
    return 0;
}
