#include"HttpServer.h"
#include"EpollTools.h"
#include"LogThread.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//Http状态响应信息
const char* ok_200_title = "OK";
const char* error_400_title = "BAD REQUEST";
const char* error_400_form = " Your request has bad syntax or is inherently impossbile to satisfy.\n";
const char* error_403_title = "FORBIDDEN";
const char* error_403_form = " You do not have permission to get file from this server";
const char* error_404_title = "NOT FOUND";
const char* error_404_form = "The request file was not found on this server.";
const char* error_500_title = "INTERNAL ERROR";
const char* error_500_form = "There was an unusual problem serving the request file.\n";


const char* docRoot = "var/www/html";

HttpServer::HttpServer()
            :check_state_(CHECK_STATE_REQUESTLINE),
            linger_(false),
            method_(GET)
            {
                memset(readBuf,'\0',READBUFFERSIZE);
                memset(writeBuf_,'\0',WRITEBUFFERSIZE);
                memset(filePath_,'\0',FILENAME);
            }
HttpServer::~HttpServer(){}

int HttpServer::epollfd_ = -1;
int HttpServer::userConn_cnt = 0;

void HttpServer::init(int sockfd, const sockaddr_in& sockAddr)
{
    sockfd_ = sockfd;
    sockAddr_ = sockAddr;
    // epollfd_ = epoll_create(5);
    addfd(epollfd_,sockfd,true);
    userConn_cnt++;
    reset();
}

void HttpServer::close(bool state)
{
    if(state && (sockfd_ != -1))
    {
        removefd(epollfd_,sockfd_);
        sockfd_ = -1;
        userConn_cnt--;
    }
}

void HttpServer::process()
{
    HTTP_CODE read_res = processRead();
    if(read_res == NO_REQUEST)
    {
        modfd(epollfd_,sockfd_,EPOLLIN);
        return;
    }
    bool write_res = processWrite(read_res);
    if(!write_res)
    {
        close(true);
    }
    modfd(epollfd_,sockfd_,EPOLLOUT);
}

bool HttpServer::read()
{
    if(readIndex_ >= READBUFFERSIZE)
    {
        return false;
    }

    int byteRead = 0;
    while(1)
    {
        byteRead = recv(sockfd_,readBuf+readIndex_,READBUFFERSIZE-readIndex_,0);
        if(byteRead == -1)
        {
            if(errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break;
            }
            LOG_FATAL << "HttpServer::read():byteREAD = -1" << log::end;
            return false;
        }   
        else if(byteRead == 0)
        {
            LOG_FATAL << "HttpServer::read():byteREAD = 0" << log::end;
            return false;
        }
        readIndex_ += byteRead;
    }
    return true;
}

//private
//重置
void HttpServer::reset()
{
    check_state_ = CHECK_STATE_REQUESTLINE,
    linger_ = false;
    method_ = GET;
    url_ = 0;
    version_ = 0;
    contentLength_ = 0;
    host_ = 0;
    processingLineSt = 0;
    checkIndex_ = 0;
    readIndex_ = 0;
    writeIndex_ = 0;
    memset(readBuf,'\0',READBUFFERSIZE);
    memset(writeBuf_,'\0',WRITEBUFFERSIZE);
    memset(filePath_,'\0',FILENAME);
}


//Http从状态机
HttpServer::LINE_STAUS HttpServer::parse_line()
{
    char tmp;
    for(; checkIndex_ < readIndex_; checkIndex_++)
    {
        tmp = readBuf[checkIndex_];
        if(tmp == '\r')
        {
            if(checkIndex_ + 1 == readIndex_)
            {
                return LINE_OPEN;
            }
            else if( readBuf[checkIndex_+1] == '\n')
            {
                readBuf[checkIndex_++] = '\0';
                readBuf[checkIndex_++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
        else if(tmp == '\n')
        {
            if((checkIndex_ > 1) && (readBuf[checkIndex_-1] == '\r'))
            {
                readBuf[checkIndex_++] = '\0';
                readBuf[checkIndex_++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
    }
    return LINE_OPEN;
}

HttpServer::HTTP_CODE HttpServer::parseContent(char* text)
{
    if(readIndex_ >= (contentLength_ + checkIndex_))
    {
        text[contentLength_] = '\0';
        return GET_REQUEST;
    }
    return NO_REQUEST;
}

HttpServer::HTTP_CODE HttpServer::parseRequestlion(char* text)
{
    url_ =strpbrk(text," \t"); 
    if(!url_)
    {
        return BAD_REQUEST;
    }
    *url_++ = '\0';

    char* method = text;
    if(strcasecmp(method,"GET") == 0)
    {
        method_ = GET;
    }
    else
    {
        return BAD_REQUEST;
    }

    url_ += strspn(url_," \t");
    version_ = strpbrk(url_," \t");
    if(!version_)
    {
        return BAD_REQUEST;
    }
    *version_ += '\0';
    version_ += strspn(version_," \t");
    if(strcasecmp(version_,"HTTP/1.1") != 0)
    {
        return BAD_REQUEST;
    }
    if(strncasecmp(url_,"http://",7) == 0)
    {
        url_ += 7;
        url_ = strchr(url_,'/');
    }
    if( !url_ || url_[0] != '/')
    {
        return BAD_REQUEST;
    }
    check_state_ = CHECK_STATE_HEADER;
    return NO_REQUEST;    
}

//解析http请求的头部信息
HttpServer::HTTP_CODE HttpServer::parseHeaders(char* text)
{
    if( text[0] == '\0')
    {
        if(contentLength_ != 0)
        {
            check_state_ = CHECK_STATE_CONNECT;
            return NO_REQUEST;
        }
        return GET_REQUEST;
    }
    //处理connection头部
    else if( strncasecmp(text,"Connection:",11) == 0)
    {
        text += 11;
        text += strspn(text," \t");
        if( strcasecmp( text,"keep-alive" ) == 0)
        {
            linger_ = true;
        }
    }
    //处理connection-Length头部字段
    else if(strncasecmp(text,"Content-Length:",15) == 0)
    {
        text += 15;
        text += strspn(text," \t");
        contentLength_ = atol(text);
    }
    //处理Host头部字段
    else if( strncasecmp( text, "Host:",5) == 0)
    {
        text += 5;
        text += strspn(text,"\t");
        host_ = text;
    }
    else
    {
        LOG_FATAL << "unknow header" << text << log::end;
        printf("unknow header %s\n",text);
    }
    return NO_REQUEST;
}

//解析http请求
HttpServer::HTTP_CODE HttpServer::processRead()
{
    LINE_STAUS line_status = LINE_OK;
    HTTP_CODE ret = NO_REQUEST;
    char* text = 0;
    while(((check_state_ == CHECK_STATE_CONNECT) && (line_status == LINE_OK))
            || ((line_status = parse_line()) == LINE_OK ))
    {
        text = get_line();
        processingLineSt = checkIndex_;
        printf( "1 http lines gotted: %s\n",text);

        switch (check_state_)
        {
        case CHECK_STATE_CONNECT:
        {
            ret = parseContent( text);
            if( ret == GET_REQUEST)
            {
                return doRequest();
            }
            line_status = LINE_OPEN;
            break;
        }
        case CHECK_STATE_REQUESTLINE:
        {
            ret = parseRequestlion(text);
            if( ret == BAD_REQUEST)
            {
                return BAD_REQUEST;
            }
            break;
        }
        case CHECK_STATE_HEADER:
        {
            ret = parseHeaders(text);
            if( ret == BAD_REQUEST)
            {
                return BAD_REQUEST;
            }
            if( ret == GET_REQUEST)
            {
                return doRequest();
            }
            break;
        }
        default:
            return INTERNAL_ERROR;
        }
    }
    return NO_REQUEST;
}
//char* get_line()    { return read_buf + processing_line_s; }

//对req目标文件进行操作
HttpServer::HTTP_CODE HttpServer::doRequest()
{
    strcpy(filePath_,docRoot);
    int len = strlen(docRoot);
    strncpy(filePath_+len,url_,FILENAME-len-1);
    if(stat(filePath_,&fileStat_) < 0)
    {
        return NO_REQUEST;
    }

    if(!(fileStat_.st_mode & S_IROTH))
    {
        return FORBIDDEN_REQUEST;
    }

    if( S_ISDIR( fileStat_.st_mode))
    {
        return BAD_REQUEST;
    }

    int fd = open(filePath_,O_RDONLY);

    fileAddress_ = (char*)mmap(0,fileStat_.st_size,PROT_READ,
                                MAP_PRIVATE,fd,0);
    close(fd);
    return FILE_REQUEST;
}


void HttpServer::unmmap()
{
    if(fileAddress_)
    {
        munmap(fileAddress_,fileStat_.st_size);
        fileAddress_ = 0;
    }
}

bool HttpServer::write()
{
    int tmp = 0;
    int bytesSended = 0;
    int bytesTSend = writeIndex_;
    if(bytesTSend == 0)
    {
        modfd(epollfd_,sockfd_,EPOLLIN);
        reset();
        return 1;
    }
    while(1)
    {
        tmp = writev(sockfd_,iv_,ivCnt_);
        if(tmp <= -1)
        {
            if(errno == EAGAIN)
            {
                LOG_FATAL << "HttpServer::write():writev(sockfd_,iv_, ivCnt_): tmp <= -1 && errno = " << errno 
                                << log::end;
                modfd(epollfd_,sockfd_,EPOLLOUT);
                return true;
            }
            unmmap();
            return false;
        }

        bytesTSend -= tmp;
        bytesSended += tmp;
        if( bytesTSend <= bytesSended )
        {
            //发送HTTP响应成功，判断是否关闭连接
            unmmap();
            if(linger_)
            {
                reset();
                modfd( epollfd_, sockfd_, EPOLLIN);
                return true;
            }
            else
            {
                modfd(epollfd_, sockfd_, EPOLLIN);
                return false;
            }
        }
    }
}

bool HttpServer::addResponse( const char* format, ... )
{
    if(writeIndex_ >= WRITEBUFFERSIZE)
    {
        LOG_FATAL << "writeIndex_ >= WRITEBUFFERSIZE" << log::end;
        return false;
    }

    va_list argList;
    va_start (argList, format);
    int len = vsnprintf(writeBuf_,WRITEBUFFERSIZE - 1 - writeIndex_,
                            format, argList);
    if( len >= (WRITEBUFFERSIZE - 1 - writeIndex_ ))
    {
        return false;
    }
    writeIndex_ += len;
    va_end(argList);
    return true;
}

bool HttpServer::addStatusLine(int status, const char* title)
{
    return addResponse("%s %d %s\r\n", "HTTP/1.1",status, title);
}

bool HttpServer::addHeaders(int contentLen)
{
    bool ret1 = addContentLength(contentLen);
    bool ret2 = addLinger();
    bool ret3 = addBlankLine();
    return ret1 && ret2 && ret3;
}

bool HttpServer::addContentLength(int contentLen)
{
    return addResponse("ContentLength: %d\r\n", contentLen);
}

bool HttpServer::addLinger()
{
    return addResponse("Connection: %s\r\n", (linger_ == true) ? "keep-alive" : "close");
}

bool HttpServer::addBlankLine()
{
    return addResponse("%s","\r\n");
}

bool HttpServer::addContent(const char* content)
{
    return addResponse("%s",content);
}

bool HttpServer::processWrite(HTTP_CODE read_res)
{
    switch (read_res)
    {
    case INTERNAL_ERROR:
    {
        addStatusLine(500,error_500_title);
        addHeaders(strlen(error_500_form));
        if( !addContent(error_500_form))
        {
            LOG_FATAL << "HttpServer::processWrite() : addContent(error_500_form) failed." << log::end;
            return false;
        }
        break;
    }
    case BAD_REQUEST:
    {
        addStatusLine(400,error_400_title);
        addHeaders(strlen(error_400_form));
        if( !addContent(error_400_form))
        {
            LOG_FATAL << "HttpServer::processWrite() : addContent(error_400_form) failed." << log::end;
            return false;
        }
        break;
    }
    case NO_RESOURCE:
    {
        addStatusLine(404,error_404_title);
        addHeaders(strlen(error_404_form));
        if( !addContent(error_404_form))
        {
            LOG_FATAL << "HttpServer::processWrite() : addContent(error_404_form) failed." << log::end;
            return false;
        }
        break;
    }
    case FORBIDDEN_REQUEST:
    {
        addStatusLine(403,error_403_title);
        addHeaders(strlen(error_403_form));
        if( !addContent(error_403_form))
        {
            LOG_FATAL << "HttpServer::processWrite() : addContent(error_403_form) failed." << log::end;
            return false;
        }
        break;
    }
    case FILE_REQUEST:
    {
        addStatusLine(200,ok_200_title);
        if( fileStat_.st_size != 0)
        {
            addHeaders(fileStat_.st_size );
            iv_[0].iov_base = writeBuf_;
            iv_[0].iov_len = writeIndex_;
            iv_[1].iov_base = fileAddress_;
            iv_[1].iov_len = fileStat_.st_size;
            ivCnt_ = 2;
            return true;
        }
        else
        {
            const char* s = "<html><body></body><html>";
            addHeaders(strlen(s));
            if(!addContent(s))
            {
                return false;
            }
        }
    }
    default:
    {
        return false;
    }
    }
    iv_[0].iov_base = writeBuf_;
    iv_[0].iov_len = writeIndex_;
    ivCnt_ = 1;
    return true;
} 