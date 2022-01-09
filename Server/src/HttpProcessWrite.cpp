#include"HttpProcessWrite.h"
#include"LogThread.h"
#include<algorithm>
#include"stdio.h"


const char* ok_200_title = "OK";
const char* error_400_title = "BAD REQUEST";
const char* error_400_form = 
        " Your request has bad syntax or is inherently impossbile to satisfy.\n";
const char* error_404_title = "NOT FOUND";
const char* error_404_form = "The request file was not found on this server.\n";

HttpProcessWrite::HttpProcessWrite(HttpProcessRead* processRead)
                :pRead(processRead)
{

}
HttpProcessWrite::~HttpProcessWrite()
{

}



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

bool HttpProcessWrite::WriteResponse(const std::string info)
{
    if(writeIndex_ > WRITEBUFFERSIZE)
    {
        LOG_FATAL << "writeIndex > WIRTEBUFFERSIZE, write saturation" << log::end;
        return false;
    }
    // 有问题
    //std::copy(write.begin()+writeIndex_,write.end(),info);
    void* d = memcpy(writeBuffer+writeIndex_,&info,WRITEBUFFERSIZE-writeIndex_);
    if( !d)
    {
        return false;
    }
    writeIndex_ += info.size();
    return true;
}

bool HttpProcessWrite::addStatus(int status, const char* title)
{
    std::string s = "HTTP/1.1" + std::to_string(status) + title;
    return WriteResponse(s);
}

bool HttpProcessWrite::addHead(int contentLen)
{
    return addContentLen(contentLen) && addLinger() && addBlankLine();
}

bool HttpProcessWrite::addContentLen(int conteneLen)
{
    std::string s = "ContentLength: " + std::to_string(conteneLen) + "\r\n";
    return WriteResponse(s);
}

bool HttpProcessWrite::addLinger()
{
    std::string st;
    if(pRead->linger_)
    {
        st = "keep-alive";
    }
    else
    {
        st = "close";
    }
    std::string s = "Connection: " + st + "\r\n";
    return WriteResponse(s);
}

bool HttpProcessWrite::addBlankLine()
{
    std::string s = "\r\n";
    return WriteResponse(s);
}

bool HttpProcessWrite::writeContent(const char* content)
{
    std::string s = content;
    return WriteResponse(s);
}
