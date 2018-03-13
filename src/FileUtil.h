//
// Created by root on 18-3-13.
//

#ifndef ECHOSERVER_FILEUTIL_H
#define ECHOSERVER_FILEUTIL_H

#include <string>
#include <vector>

using namespace std;



class FileUtil {
public:

    vector<string> getFiles(char *path);

    void  writeLog(char wValue[]);

    //键值对方式读取参数配置文件
    bool readConfigFile(const char* cfgfilepath,const string & key, string & value1);


};


#endif //ECHOSERVER_FILEUTIL_H
