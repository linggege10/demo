/*  
 * File:   Server.c 
 * Author: root 
 * 
 * Created on 2012年6月20日, 下午1:29 
 */  
#include <unistd.h>
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
#include <vector>
#include <algorithm>




#define MAXBUFFER 256
using namespace std;

int write_log(const char *msg, ...){
    cout<<msg;
    cout<<*msg;

char content[100]={0};
    cout<<content;
    cout<<"99999";


}


void yy(int *a,int *b){
    char c[100];


    *a+=2;
    *b+=2;
   // a+=2;
   // b+=2;
    vector<int> vec(5);
    vec[0]=2;
    vec[1]=5;
    vec[2]=2;
    vec[3]=3;
    vec[4]=9;
    reverse(vec.begin(),vec.end());
    cout<<"111";
    cout<<vec[1];





}

int main(int argc, char* argv[])
{
    int a=5;
    int *c=&a;
    cout<<&a;
    cout<<&*c;
 //   int *d=c;
   // cout<<d;
 //   int b=6;
   // cout<<&a;
  //  cout<<&b;
  //  yy(&a,&b);
   // cout<<a;
  //  cout<<b;
   // cout<<&a;
   // cout<<&b;




//int *pi=new int( 10 );
//    cout<<*pi;
//    while ( *pi < 10 ) {
//        cout<<150;
//        *pi = *pi + 1;
//    }
//
//
//
//
//// cout<<*pi;
//    char h='k';
//    char *s=&h;
//    cout<<"ppppppp";
//    cout<<s;
//    cout<<*s;
//    char ss='2';
//
//    write_log("9999");
    //write_log(ss);
}  
