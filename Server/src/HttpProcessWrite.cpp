#include"HttpProcessWrite.h"
#include"LogThread.h"

#include<algorithm>
#include"stdio.h"
#include"stdlib.h"
#include"stdarg.h"
#include<chrono>

const char* ok_200_title = "OK";
const char* error_400_title = "BAD REQUEST";
const char* error_400_form = 
        " Your request has bad syntax or is inherently impossbile to satisfy.\n";
const char* error_404_title = "NOT FOUND";
const char* error_404_form = "The request file was not found on this server.\n";

HttpProcessWrite::HttpProcessWrite(std::shared_ptr<HttpProcessRead> processRead)
                :pRead(processRead)
{}
HttpProcessWrite::~HttpProcessWrite(){}



bool HttpProcessWrite::processWrite(HttpProcessRead::HTTPCODE readRet)
{
    switch(readRet)
    {     
        case HttpProcessRead::HTTPCODE::BAD_REQUEST:
        {
            addStatus(400,error_400_title);
            addHead(strlen(error_400_form));
            if( !writeContent(error_400_form))
            {
                LOG_FATAL << "HttpServer::processWrite() : addContent(error_400_form) failed." << log::end;
                return false;
            }
            break;
        }
        case HttpProcessRead::HTTPCODE::NO_RESOURCE:
        {
            addStatus(404,error_404_form);
            addHead(strlen(error_404_form));
            if( !writeContent(error_404_form))
            {
                LOG_FATAL << "HttpServer::processWrite() : addContent(error_404_form) failed." << log::end;
                return false;
            }
            break;
        }
        case HttpProcessRead::HTTPCODE::FILE_REQUEST:
        {
            addStatus(200,ok_200_title);       
            if(pRead->filestat_.st_size != 0)
            { 
                addHead(pRead->filestat_.st_size);
                
                iv_[0].iov_base = writeBuffer;
                iv_[0].iov_len = writeIndex_;
                iv_[1].iov_base = pRead->fileAddr;
                iv_[1].iov_len = pRead->filestat_.st_size;
                ivCnt_ = 2;
                return true;
            }
            else
            {
                const char* s = "<html><body></body><html>";
                addHead(strlen(s));
                if( !writeContent(s))
                {
                    return false;
                }
            }
            break;
        }
        default:
        {
            return false;
        }
    }
    iv_[0].iov_base = writeBuffer;
    iv_[0].iov_len = writeIndex_;
    ivCnt_ = 1;
    return true;
}

void HttpProcessWrite::reset()
{
    writeIndex_ = 0;
    memset(writeBuffer,'\0',WRITEBUFFERSIZE);
}

bool HttpProcessWrite::WriteResponse(const char* format, ...)
{
    if(writeIndex_ >= WRITEBUFFERSIZE)
    {
        LOG_FATAL << "writeIndex > WIRTEBUFFERSIZE, write saturation" << log::end;
        return false;
    }
    
    va_list argList;
    va_start (argList, format);
    int len = vsnprintf(writeBuffer+writeIndex_,WRITEBUFFERSIZE-1-writeIndex_,
                            format,argList);
    if(len > WRITEBUFFERSIZE-1-writeIndex_)
    {
        va_end(argList);
        return false;
    }

    writeIndex_ += len;
    va_end(argList);    
    return true;
}

bool HttpProcessWrite::addStatus(int status, const char* title)
{    
    return WriteResponse("%s %d %s\r\n", "HTTP/1.1",status, title);
}

bool HttpProcessWrite::addHead(int contentLen)
{
    return addContentType() && addContentLen(contentLen) && addConnection() 
             && addBlankLine();
}
bool HttpProcessWrite::addDate()
{
    return WriteResponse("Date: Sun, 13 Jan 2022 09:31:07 GMT\r\n");
}

bool HttpProcessWrite::addContentType()
{
    return WriteResponse("Content-Type:%s\r\n", "text/html");
}

bool HttpProcessWrite::addContentLen(int contentLen)
{
    return WriteResponse("ContentLength:%d\r\n", contentLen);
}

bool HttpProcessWrite::addMod()
{
    return  WriteResponse("Last-Modified: Thu, 14 Sep 2017 03:04:00 GMT\r\n");
}

bool HttpProcessWrite::addConnection()
{
    
    //return WriteResponse("Connection:%s\r\n", (pRead->linger_ == true) ? "Keep-Alive" : "close");
    pRead->linger_ = false;
    //return WriteResponse("Connection: %s\r\n", "Keep-alive");
    return WriteResponse("Connection: %s\r\n", "Close");
}

bool HttpProcessWrite::addVary()
{
    return WriteResponse("Vary: Accept-Encoding\r\n");
}

bool HttpProcessWrite::addServer()
{
    return WriteResponse("Server: 124.24.176.229:9527\r\n");
}

bool HttpProcessWrite::addComp()
{
    return WriteResponse("X-UA-Compatible: IE=Edge,chrome=1\r\n");
}

bool HttpProcessWrite::addPragma()
{
    return WriteResponse("Pragma: no-cache\r\n");
}

bool HttpProcessWrite::addAccpRange()
{
    return WriteResponse("Accept-Ranges: bytes\r\n");
}

bool HttpProcessWrite::addBlankLine()
{
    return WriteResponse("%s","\r\n");
}

bool HttpProcessWrite::writeContent(const char* content)
{
    return WriteResponse("%s",content);
}
