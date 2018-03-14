/*  
 * File:   Server.c 
 * Author: root 
 * 
 * Created on 2012年6月20日, 下午1:29 
 */
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>  
#include <stdlib.h>

#include <sys/socket.h>  
#include <string.h>
#include <time.h>
#include <errno.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>   
#include <iostream>
#include <string>  
#include <fstream>
#include<sys/types.h>   
#include <sys/fcntl.h>
#include <stdbool.h>
#include <time.h>
#include <map>
#include"FileUtil.h"


#include <dirent.h>




#define MAXBUFFER 256
using namespace std;



string getTime()
{
    time_t timep;
    time (&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&timep) );
    return tmp;
}
map<int, string> mp;

//异步接受数据
void ayncReceive(int port, int backlog) {
    int rcd;
    int new_cli_fd;
    int maxfd;
    socklen_t socklen;
    int  server_len;
    int ci;
    int watch_fd_list[backlog + 1];
    for (ci = 0; ci <= backlog; ci++)
        watch_fd_list[ci] = -1;

    int server_sockfd;
    //建立socket，类型为TCP流  
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sockfd == -1) {
        printf("create server_socket error!\n");
        exit(1);
    }

    //设为非阻塞  
    if (fcntl(server_sockfd, F_SETFL, O_NONBLOCK) == -1) {
        printf("Set server socket nonblock failed\n");
        exit(1);
    }




    struct sockaddr_in server_sockaddr;
    memset(&server_sockaddr, 0, sizeof(server_sockaddr));
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //设置监听端口  
    server_sockaddr.sin_port = htons(port);
    server_len = sizeof(server_sockaddr);
    //绑定  
    rcd = bind(server_sockfd, (struct sockaddr *) &server_sockaddr, server_len);
    if (rcd == -1) {
        printf("bind port %d error!\n", ntohs(server_sockaddr.sin_port));
        exit(1);
    }
    //监听  
    rcd = listen(server_sockfd, backlog);
    if (rcd == -1) {
        printf("listen error!\n");
        exit(1);
    }
    printf("Server is  waiting on socket=%d \n", server_sockfd);

    watch_fd_list[0] = server_sockfd;
    maxfd = server_sockfd;

    //初始化监听集合  
    fd_set watchset;
    FD_ZERO(&watchset);
    FD_SET(server_sockfd, &watchset);

    struct timeval tv; /* 声明一个时间变量来保存时间 */
    struct sockaddr_in cli_sockaddr;
    while (1) {
        tv.tv_sec = 20;
        tv.tv_usec = 0; /* 设置select等待的最大时间为20秒*/
        //每次都要重新设置集合才能激发事件  
        FD_ZERO(&watchset);
        FD_SET(server_sockfd, &watchset);
        //对已存在到socket重新设置  
        for (ci = 0; ci <= backlog; ci++)
            if (watch_fd_list[ci] != -1) {
                FD_SET(watch_fd_list[ci], &watchset);
            }

        rcd = select(maxfd + 1, &watchset, NULL, NULL, NULL);
        switch (rcd) {
            case -1:
                printf("Select error\n");
                exit(1);
            case 0:
                printf("Select time_out\n");
                //超时则清理掉所有集合元素并关闭所有与客户端的socket
                FD_ZERO(&watchset);
                for (ci = 1; ci <= backlog; ci++){
                    shutdown(watch_fd_list[ci],2);
                    watch_fd_list[ci] = -1;
                }
                //重新设置监听socket，等待链接
                FD_CLR(server_sockfd, &watchset);
                FD_SET(server_sockfd, &watchset);
                continue;
            default:
                //检测是否有新连接建立
                if (FD_ISSET(server_sockfd, &watchset)) { //new connection
                    socklen = sizeof(cli_sockaddr);
                    new_cli_fd = accept(server_sockfd,
                                        (struct sockaddr *) &cli_sockaddr, &socklen);

                    //获取连接时间，客户端端口号和IP
                    string   timeLjsj1 =getTime();
                    char *addr=   inet_ntoa(cli_sockaddr.sin_addr);
                    int port=ntohs(cli_sockaddr.sin_port);
                    string   wLog =timeLjsj1+addr+to_string (port);

                    mp.insert(pair<int, string>(server_sockfd, wLog));
                    if (new_cli_fd < 0) {
                        printf("Accept error\n");
                        exit(1);
                    }
                    printf("\nopen communication with  Client %s on socket %d\n",
                           inet_ntoa(cli_sockaddr.sin_addr), new_cli_fd);



                    for (ci = 1; ci <= backlog; ci++) {
                        if (watch_fd_list[ci] == -1) {
                            watch_fd_list[ci] = new_cli_fd;
                            break;
                        }
                    }

                    FD_SET(new_cli_fd, &watchset);
                    if (maxfd < new_cli_fd) {
                        maxfd = new_cli_fd;
                    }

                    continue;
                } else {//已有连接的数据通信
                    //遍历每个设置过的集合元素

                    for (ci = 1; ci <= backlog; ci++) { //data
                        if (watch_fd_list[ci] == -1)
                            continue;
                        if (!FD_ISSET(watch_fd_list[ci], &watchset)) {
                            continue;
                        }
                        char buffer[128];
                        //接收
                        int len = recv(watch_fd_list[ci], buffer, 128, 0);
                        if (len < 0) {
                            printf("Recv error\n");
                            exit(1);
                        }else if(len==0){
                            printf("断开连接");
                            //记录日志
                            string wrLog= mp[server_sockfd];
                            char sWl[40];
                            sprintf(sWl,"%s  %s",wrLog.c_str(),getTime().c_str());
                            FileUtil fu;
                            fu.writeLog(sWl);


                        }
                        buffer[len] = 0;

                        //获得客户端的IP地址
                        struct sockaddr_in sockaddr;
                        getpeername(watch_fd_list[ci], (struct sockaddr*) &sockaddr,
                                    (socklen_t*)sizeof(sockaddr));


                        string wrLog= mp[server_sockfd];
                        mp[server_sockfd]=wrLog+buffer;
                        printf("read data [%s] from Client %s on socket %d\n",
                               buffer,inet_ntoa(sockaddr.sin_addr),watch_fd_list[ci]);
                        //发送接收到到数据
                        len = send(watch_fd_list[ci], buffer, strlen(buffer), 0);
                        if (len < 0) {
                            printf("Send error\n");
                            exit(1);
                        }
                        printf("write data [%s] to Client %s on socket %d\n",
                               buffer, inet_ntoa(sockaddr.sin_addr),
                               watch_fd_list[ci]);

                        shutdown(watch_fd_list[ci],2);
                        watch_fd_list[ci] = -1;
                        FD_CLR(watch_fd_list[ci], &watchset);

                        //接收到的是关闭命令
                        if (strcmp(buffer, "quit") == 0) {
                            for (ci = 0; ci <= backlog; ci++)
                                if (watch_fd_list[ci] != -1) {
                                    shutdown(watch_fd_list[ci],2);
                                }
                            printf("\nWeb Server Quit!\n");
                            exit(0);
                        }
                    }
                }
                break;
        }
    }
}




//同步接受数据
void syncReceive(int port,string ipServer){
    int serverFd, connfd,ret;
    socklen_t len;
    struct sockaddr_in serveraddr,clientaddr;
    char readBuf[MAXBUFFER]={0};
    char ip[40]={0};
    serverFd=socket(AF_INET,SOCK_STREAM,0);//创建socket
    if(serverFd < 0)
    {
        printf("socket error:%s\n",strerror(errno));
        exit(-1);
    }
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(port);
    inet_pton(AF_INET,ipServer.c_str(),&serveraddr.sin_addr);
    ret=bind(serverFd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
    if(ret!=0)
    {
        close(serverFd);
        printf("bind error:%s\n",strerror(errno));
        exit(-1);
    }
    ret=listen(serverFd,5);//监听
    if(ret!=0)
    {
        close(serverFd);
        printf("listen error:%s\n",strerror(errno));
        exit(-1);
    }


    len=sizeof(clientaddr);
    bzero(&clientaddr,sizeof(clientaddr));
    while (1)
    {
        connfd = accept(serverFd, (struct sockaddr *) &clientaddr, &len);
        char *addr=   inet_ntoa(clientaddr.sin_addr);
        string   timeLjsj = getTime();
        printf("%s 连接到服务器 \n",inet_ntop(AF_INET,&clientaddr.sin_addr,ip,sizeof(ip)));
        if (serverFd < 0)
        {

            printf("accept error : %s\n", strerror(errno));
            continue;
        }
        string receiveData="";
        while((ret=read(connfd,readBuf,MAXBUFFER)))//读客户端发送的数据
        {
            write(connfd,readBuf,MAXBUFFER);//写回客户端

            receiveData=receiveData+readBuf;

            bzero(readBuf,MAXBUFFER);
        }
        string   timeDksj;
        if(ret==0)
        {
            timeDksj = getTime();
            printf("客户端关闭连接\n");
        }else
        {
            printf("read error:%s\n",strerror(errno));
        }


        close(connfd);
    }

    close(serverFd);



}



int main(int argc, char** argv)
{

    const char* cfgfilepath="/home/hzl/project/github/echoServer/demo/echo.cfg";
    const string listen_ip="listen_ip";
    const string listen_port="listen_port";
    const string net_mode="net_mode";
    const string log_directory="log_directory";
    const string log_file_name="log_file_name";
    const string log_file_size="log_file_size";
    const string log_file_number="log_file_number";
    string value_listen_ip="";
    string value_listen_port="";
    string value_net_mode="";
    string value_log_directory="";
    string value_log_file_name="";
    string value_log_file_size="";
    string value_log_file_number="";
    FileUtil  fu;
    fu.readConfigFile(cfgfilepath,listen_ip,value_listen_ip);
    fu.readConfigFile(cfgfilepath,listen_port,value_listen_port);
    fu.readConfigFile(cfgfilepath,net_mode,value_net_mode);
    fu.readConfigFile(cfgfilepath,net_mode,value_log_directory);
    fu.readConfigFile(cfgfilepath,net_mode,value_log_file_name);
    fu.readConfigFile(cfgfilepath,net_mode,value_log_file_size);
    fu.readConfigFile(cfgfilepath,net_mode,value_log_file_number);
    if(value_net_mode.compare("asyn")==0){
        ayncReceive(atoi(value_listen_port.c_str()),5);
    }else if(value_net_mode.compare("sync")==0){
        syncReceive(atoi(value_listen_port.c_str()),value_listen_ip);
    }


    return 0;
}  
