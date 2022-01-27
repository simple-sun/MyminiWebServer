#include"HttpProcessRead.h"
#include"LogThread.h"
#include"EpollTools.h"

#include"stdio.h"
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>


const char* docRoot = "/home/web/MyminiWebServer/Html";

HttpProcessRead::HttpProcessRead(char readbuffer[],int readIndex)
                :checkstate_(CHECK_REQUESTLINE),
                readIndex_(readIndex),
                linger_(false)
{
    memset(readBuffer,'\0',READBUFFERSIZE);
    memset(filePath_,'\0',200); 
    memcpy(readBuffer, readbuffer,(size_t)READBUFFERSIZE);    
}

HttpProcessRead::~HttpProcessRead(){
    //delete readBuffer;
    //delete url_;
    // delete version_;
    // delete host_;
    // delete fileAddr;
}

HttpProcessRead::HTTPCODE HttpProcessRead::processRead()
{
    HTTPCODE ret = NO_REQUEST;
    LINESTATUS lineStatus = LINE_OK;
    char* text = 0 ;
    

    while((lineStatus == LINE_OK && checkstate_ == CHECK_CONTENT)
            || (lineStatus = parseLine()) == LINE_OK)
    {
        text =  getLine();
        processPosition = checkIndex_;
        printf("get 1 http request: %s. \n",text);
        
        if(checkstate_ == CHECK_REQUESTLINE)
        {
            ret = parseRequest(text);
            if(ret == BAD_REQUEST)
            {
                return BAD_REQUEST;
            }
        }
        else if(checkstate_ == CHECK_HEAD)
        {
            ret = parseHead(text);
            if(ret == BAD_REQUEST)
            {
                return BAD_REQUEST;
            }
            else if( ret == GET_REQUEST)
            {
                return doRequset();
            }
        }
        else if(checkstate_ == CHECK_CONTENT)
        {
            ret = parseContent(text);
            if(ret == GET_REQUEST)
            {
                return doRequset();
            }
            lineStatus = LINE_HALFBAKED;
        }
    }
    return NO_REQUEST;
}

HttpProcessRead::HTTPCODE HttpProcessRead::parseContent(char* text)
{
    if(readIndex_ >= (contentLength_ + checkIndex_))
    {
        text[contentLength_] = '\0';
        postdata = text;
        //printf("parseContent: %s\n",text);
        return GET_REQUEST;
    }
    return NO_REQUEST;
}

HttpProcessRead::HTTPCODE HttpProcessRead::parseHead(char* text)
{
    if( text[0] == '\0')
    {
        if(contentLength_ != 0)
        {
            checkstate_ = CHECK_CONTENT;
            return NO_REQUEST;
        }
        return GET_REQUEST;
    }   

    //处理connection头部
    else if( strncasecmp(text,"Connection:",11) == 0)
    {
        text += 11;
        text += strspn(text," \t");
        if(strcasecmp(text,"keep-alive") == 0)
        {
            linger_ = true;
        }
    }
    else if(strncasecmp(text,"Content-Length:",15) == 0)
    {
        text += 15;
        text += strspn(text," \t");
        contentLength_ = atol(text); 
    }
    else if(strncasecmp( text, "Host:",5) == 0)
    {
        text += 5;
        text += strspn(text," \t");
        host_ = text;
    }
    else
    {
        LOG_FATAL << "unknow header" << text << log::end;
        //printf("unknow header %s\n",text);
    }
    return NO_REQUEST;
}

HttpProcessRead::HTTPCODE HttpProcessRead::parseRequest(char* text)
{
    url_ = strpbrk(text, " \t");
    if(!url_)
    {
        return BAD_REQUEST;
    }
    *url_++ = '\0';
    
    char* mtext = text;
    if(strcasecmp(mtext,"GET") == 0)
    {
        method_ = GET;
        
    }
    else if(strcasecmp(mtext,"POST") == 0)
    {
        method_ = POST;
        
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
    *version_++ = '\0';
    version_ += strspn(version_," \t");
    if(strcasecmp(version_,"HTTP/1.1"))
    {
        return BAD_REQUEST;
    }
    if(strncasecmp(url_,"http://",7) == 0)
    {
        url_ += 7;
        url_ = strchr(url_,'/');
    }
    if(strncasecmp(url_,"https://",8) == 0)
    {
        url_ += 8;
        url_ = strchr(url_,'/');
    }
    if(strncasecmp(url_,"/",1) == 0)
    {
        //strcat(url_,"try.html");
        if(method_ == GET)
        url_ = "/home.html";
        
    }
    if( !url_ || url_[0] != '/')
    {
        return BAD_REQUEST;
    }
    checkstate_ = CHECK_HEAD;
    return GET_REQUEST;  
}

HttpProcessRead::HTTPCODE HttpProcessRead::doRequset()
{
    strcpy(filePath_,docRoot);
    int len = strlen(docRoot);
    if(method_ == POST)
    {
        //链接数据库，读取用户名和密码
        char name[100];
        char password[100];
        int i = 5;
        for(; postdata[i] != '&';++i)
        {
            name[i-5] = postdata[i];
        }
        name[i-5] = '\0';

        int j  = 0;
        for(i = i+10; postdata[i] != '\0'; ++i,++j)
        {
            password[j] = postdata[i];
        }
        password[j] = '\0';

        //根据登录数据，查询是否在数据库中存在
        if( strlen(name) && strlen(password))
        {
            //查询数据：名字，密码
            printf("HttpProcessRead::doRequset:: username = %s, password = %s \n",name,password);
            if(1)
            {
                strcat(url_,"try.html");
            }
        }
    }
    strncpy(filePath_+len, url_, 200-len-1);
    if(stat(filePath_,&filestat_) < 0)
    {
        printf("%s is using by other \n",filePath_);
        return BAD_REQUEST;
    }
    if(filestat_.st_mode & S_IFDIR)
    {
        printf("%s is dir",filePath_);
        return BAD_REQUEST;
    }
    int fd = open(filePath_,O_RDONLY);
    
    fileAddr = (char*)mmap(0,filestat_.st_size,PROT_READ,
                                MAP_PRIVATE,fd,0);    
    close(fd);
    return FILE_REQUEST;    
}

HttpProcessRead::LINESTATUS HttpProcessRead::parseLine()
{
    char tmp;
    for(;checkIndex_ < readIndex_; checkIndex_++)
    {
        tmp = readBuffer[checkIndex_];
        if(tmp == '\r')
        {
            if(checkIndex_ + 1 == readIndex_)
            {
                return LINE_HALFBAKED;
            }
            else if(readBuffer[checkIndex_+1] == '\n')
            {
                readBuffer[checkIndex_++] = '\0';
                readBuffer[checkIndex_++] = '\0';
                return LINE_OK;
            }
        }
        else if(tmp == '\n')
        {
            return LINE_WRONG; 
        }
    }
    return LINE_HALFBAKED;
}


void HttpProcessRead::reset()
{
    processPosition = 0;
    checkIndex_ = 0;
    readIndex_ = 0;
    checkstate_ = CHECK_REQUESTLINE;

    url_ = 0;
    method_ = GET;
    version_ = 0;
    contentLength_ = 0;
    linger_ = false;
    host_ = 0;

    memset(filePath_,'\0',1024);
    memset(readBuffer,'\0',READBUFFERSIZE);

}