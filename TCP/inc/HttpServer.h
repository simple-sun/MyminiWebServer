#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include<stdarg.h>
#include<unistd.h>
#include<signal.h>
#include<netinet/in.h>
#include<sys/uio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<errno.h>

static const int FILENAME = 200;
static const int READBUFFERSIZE = 2048;
static const int WRITEBUFFERSIZE = 2048;

class HttpServer
{
public:
//设置状态
    enum METHOD 
    {
        GET,
        POST,   
        HEAD,
        PUT,
        DELETE,
        TRACE,
        OPTIONS,
        CONNECT,
        PATCH
    };
    enum CHECK_STATE
    {
        CHECK_STATE_REQUESTLINE,
        CHECK_STATE_HEADER,
        CHECK_STATE_CONNECT
    };
    enum HTTP_CODE
    {
        NO_REQUEST,
        GET_REQUEST,
        BAD_REQUEST,
        NO_RESOURCE,
        FORBIDDEN_REQUEST,
        FILE_REQUEST,
        INTERNAL_ERROR,
        CLOSED_CONTENTION
    };
    enum LINE_STAUS
    {
        LINE_OK,
        LINE_BAD,
        LINE_OPEN
    };
public:
    HttpServer();
    ~HttpServer();

public:
    void init(int sockfd, const sockaddr_in& sockAddr);
    void close(bool state = 1);
    void process();
    bool read();
    bool write();

    LINE_STAUS parse_line();

private:
    void reset();

private:
//解析http请求
    HTTP_CODE processRead();
    char* get_line()    { return readBuf + processingLineSt; }
    //判断http请求消息是否被完整读入
    HTTP_CODE parseContent(char* text);
    HTTP_CODE parseRequestlion(char* text);
    HTTP_CODE parseHeaders(char* text);
    HTTP_CODE doRequest();

    char readBuf[READBUFFERSIZE];
    int   processingLineSt;
    int checkIndex_;
    int readIndex_;
    
private:
//应答http
    bool processWrite(HTTP_CODE read_res);
    void unmmap();
    bool addResponse( const char* format, ... );
    bool addStatusLine(int status, const char* title);
    bool addHeaders(int contentLen);
    bool addContentLength(int contentLen);
    bool addLinger();
    bool addBlankLine();
    bool addContent(const char* content);

    char writeBuf_[WRITEBUFFERSIZE];
    int writeIndex_;
    struct iovec iv_[2];
    int ivCnt_;

public:
    //Http连接的socket
    int sockfd_;
    sockaddr_in sockAddr_;
    static int epollfd_;
private:
    //Http连接
    char filePath_[FILENAME];
    char* url_;
    //Http协议版本号
    char* version_;
    char* host_;
    //Http请求消息体长度
    int contentLength_;
    bool linger_;
    METHOD method_;
    //目标文件在mmap中的起始位置
    char* fileAddress_;
    //目标文件的状态
    struct stat fileStat_;

    //程序状态
    bool state;
    CHECK_STATE check_state_;
public:
    //连接用户数量
    static int userConn_cnt;

};


#endif