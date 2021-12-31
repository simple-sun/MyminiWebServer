#ifndef HTTPPROCESSREAD_H
#define HTTPPROCESSREAD_H


static const int READBUFFERSIZE = 2048;



class HttpProcessRead 
{
    //状态
enum METHOD {  GET,POST };
enum CHECKSTATE { CHECK_REQUESTLINE,CHECK_HEAD, CHECK_CONNECT }; 
enum HTTPCODE {  NO_REQUEST, GET_REQUEST, BAD_REQUEST, NO_RESOURCE};
enum LINESTAUS { OK, WRONG, HALFBAKED };

public:
    HttpProcessRead();
    ~HttpProcessRead();

    HTTPCODE processRead();

    void setRDBuffer();

private:
    char* getLine() { return readBuffer+processPosition;}
    HTTPCODE parseContent(char* text);
    HTTPCODE parseRequest(char* text);
    HTTPCODE parseHead(char* text);
    HTTPCODE doRequset();

private:
    char readBuffer[READBUFFERSIZE];
    int processPosition;
    int checkIndex_;
    int readIndex_;
    CHECKSTATE checkstate_;
};



#endif