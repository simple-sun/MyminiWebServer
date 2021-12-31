#include"HttpProcessRead.h"
#include"string.h"
#include"stdio.h"

HttpProcessRead::HttpProcessRead()
                :checkstate_(CHECK_REQUESTLINE)
{
    memset(readBuffer,'\0',READBUFFERSIZE);
}

HttpProcessRead::~HttpProcessRead(){}

HttpProcessRead::HTTPCODE HttpProcessRead::processRead()
{
    HTTPCODE ret = NO_REQUEST;
    LINESTAUS lineStatus = OK;
    char* text = getLine();
    printf("get 1 http request: %s",*text);

    while(lineStatus == OK)
    {
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
        else if(checkstate_ == CHECK_CONNECT)
        {
            ret = parseContent(text);
            if(ret == GET_REQUEST)
            {
                return doRequset();
            }
        }
    }
    return NO_REQUEST;
}

HttpProcessRead::HTTPCODE HttpProcessRead::parseContent(char* text)
{

}

HttpProcessRead::HTTPCODE HttpProcessRead::parseHead(char* text)
{

}

HttpProcessRead::HTTPCODE HttpProcessRead::parseRequest(char* text)
{

}

HttpProcessRead::HTTPCODE HttpProcessRead::doRequset()
{
    
}