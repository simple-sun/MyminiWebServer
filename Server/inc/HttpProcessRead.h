#ifndef HTTPPROCESSREAD_H
#define HTTPPROCESSREAD_H

#include<fcntl.h>
#include<sys/mman.h>
#include"sys/socket.h"
#include<string>
#include<map>

#include"MysqlConn.h"

static const int READBUFFERSIZE = 2048;



class HttpProcessRead 
{
public:
    //状态
enum METHOD {  GET,POST };
enum CHECKSTATE { CHECK_REQUESTLINE,CHECK_HEAD, CHECK_CONTENT }; 
enum HTTPCODE {  NO_REQUEST, GET_REQUEST, BAD_REQUEST,
                 NO_RESOURCE,FILE_REQUEST};
enum LINESTATUS { LINE_OK, LINE_WRONG, LINE_HALFBAKED };

public:
    HttpProcessRead(char readbuffer[],int readIndex);
    ~HttpProcessRead();
 
    HTTPCODE processRead();

    void reset();    

    void initMysqlData(SqlPool* sqlPool);

private:
    char* getLine() { return readBuffer+processPosition;}
    HTTPCODE parseContent(char* text);
    HTTPCODE parseRequest(char* text);
    HTTPCODE parseHead(char* text);
    HTTPCODE doRequset();
    LINESTATUS parseLine();

private:
    //缓冲区
    char readBuffer[READBUFFERSIZE];
    int processPosition;
    int checkIndex_;
    int readIndex_;

    CHECKSTATE checkstate_;

    //HTTP请求
public:
    char* url_;
    METHOD method_;
    char* version_;    

    int contentLength_;
    bool linger_;
    char* host_;

    //http连接
    char filePath_[1024];

    struct stat filestat_;

    char* fileAddr;
private:
    //存储post传递的信息
    char* postdata;
    std::map<std::string,std::string> userData;

    //MySQL链接
    MYSQL* connPool_;
    MYSQL* mysql_;
    Locker lock;
    //std::shared_ptr<SqlPool> sqlPool_;
};



#endif