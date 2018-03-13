//
// Created by root on 18-3-13.
//

#include "FileUtil.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <fstream>
#include <iostream>

using namespace std;
vector<string>  FileUtil::getFiles(char *path)
{
    vector<string> vs;
    DIR * dir;
    struct dirent * ptr;
    int i=0;
    char srcFile1[1][100];
    string rootdirPath = path;
    string x,dirPath;
    dir = opendir((char *)rootdirPath.c_str()); //打开一个目录
    while((ptr = readdir(dir)) != NULL) //循环读取目录数据
    {
        printf("d_name : %s\n", ptr->d_name); //输出文件名
        x=ptr->d_name;
        dirPath = rootdirPath + x;
        printf("d_name : %s\n", dirPath.c_str()); //输出文件绝对路径



        vs.push_back(x);
        if ( ++i>=100 ) break;
    }
    closedir(dir);//关闭目录指针
    return vs;

}


//键值对方式读取参数配置文件
bool FileUtil::readConfigFile(const char* cfgfilepath,const string & key, string & value1)
{
    fstream cfgFile;
    cfgFile.open(cfgfilepath);//打开文件
    if(!cfgFile.is_open())
    {
        cout<<"can not open cfg file!"<<endl;
        return false;
    }
    char tmp[100];
    while(!cfgFile.eof())//循环读取每一行
    {

        cfgFile.getline(tmp,100);//每行读取前100个字符，100个应该足够了
        string line(tmp);
        size_t pos = line.find('=');//找到每行的“=”号位置，之前是key之后是value
        if(pos==string::npos)
            return false;
        string tmpKey = line.substr(0,pos);//取=号之前
        if(key==tmpKey)
        {
            value1 = line.substr(pos+1);//取=号之后
            return true;
        }
    }
    cfgFile.close();
    return false;
}

void  FileUtil::writeLog(char wValue[]){

    const char* cfgfilepath="/home/hzl/project/github/echoServer/demo/echo.cfg";
    const string log_directory="log_directory";
    const string log_file_name="log_file_name";
    const string log_file_size="log_file_size";
    const string log_file_number="log_file_number";

    string value_log_directory="";
    string value_log_file_name="";
    string value_log_file_size="";
    string value_log_file_number="";
    readConfigFile(cfgfilepath,log_directory,value_log_directory);
    readConfigFile(cfgfilepath,log_file_name,value_log_file_name);
    readConfigFile(cfgfilepath,log_file_size,value_log_file_size);
    readConfigFile(cfgfilepath,log_file_number,value_log_file_number);


    struct stat info;
    stat((value_log_directory+"/"+value_log_file_name).c_str(), &info);
    int size = info.st_size;
    int file_size=atoi(value_log_file_size.c_str());
    int file_number=atoi(value_log_file_number.c_str());
    if(size >= file_size * 1024 ) {
        for(int i=1;i<file_number;i++){
            char  oldFile[100];
            sprintf(oldFile,"%s_%d",(value_log_directory+"/"+value_log_file_name).c_str(),i);
            char  newFile[100];
            sprintf(newFile,"%s_%d",(value_log_directory+"/"+value_log_file_name).c_str(),i+1);
            rename(oldFile,newFile);

        }
        rename((value_log_directory+"/"+value_log_file_name).c_str(),(value_log_directory+"/"+value_log_file_name+"_1").c_str());
    }



    FILE *fp = fopen((value_log_directory+"/"+value_log_file_name).c_str(), "a+");
    char* cw=wValue;
    if(fp) {
   fprintf(fp,"%s",cw);
    }else{

        mkdir((value_log_directory+"/"+value_log_file_name).c_str(),S_IRWXU);
        FILE *fp = fopen((value_log_directory+"/"+value_log_file_name).c_str(), "a+");
        fprintf(fp,"%s",cw);
    }
    fclose(fp);


}

