#ifndef HTTPPROCESSREAD_H
#define HTTPPROCESSREAD_H

#include<fcntl.h>
#include<sys/mman.h>
#include"sys/socket.h"
#include"string.h"

static const int READBUFFERSIZE = 2048;



class HttpProcessRead 
{
    //状态
enum METHOD {  GET,POST };
enum CHECKSTATE { CHECK_REQUESTLINE,CHECK_HEAD, CHECK_CONTENT }; 
enum HTTPCODE {  NO_REQUEST, GET_REQUEST, BAD_REQUEST, NO_RESOURCE};
enum LINESTATUS { LINE_OK, LINE_WRONG, LINE_HALFBAKED };

public:
    HttpProcessRead(char readbuffer[],int readIndex);
    ~HttpProcessRead();

    HTTPCODE processRead();

    void setRDBuffer();

private:
    char* getLine() { return readBuffer+processPosition;}
    HTTPCODE parseContent(char* text);
    HTTPCODE parseRequest(char* text);
    HTTPCODE parseHead(char* text);
    HTTPCODE doRequset();
    LINESTATUS parseLine();

private:
    //缓冲区
    char* readBuffer;
    int processPosition;
    int checkIndex_;
    int readIndex_;

    CHECKSTATE checkstate_;

    //HTTP请求

    char* url_;
    METHOD method_;
    char* version_;    

    int contentLength_;
    bool linger_;
    char* host_;

    //http连接
    char filePath_[200];

    struct stat filestat_;

    char* fileAddr;
};



#endif