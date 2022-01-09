#ifndef HTTPPROCESSWRITE_H
#define HTTPPROCESSWRITE_H

#include"HttpProcessRead.h"
#include<string>
#include<vector>
#include<memory>

const int WRITEBUFFERSIZE = 2048;

class HttpProcessWrite
{
public:
    HttpProcessWrite(HttpProcessRead* processRead);
    ~HttpProcessWrite();

    bool processWrite(HttpProcessRead::HTTPCODE readRet);
private:
    bool WriteResponse(const std::string info);
    bool addStatus(int status, const char* title);

    bool addHead(int contentLen);
    bool addContentLen(int contentLen);
    bool addLinger();
    bool addBlankLine();

    bool writeContent(const char* content);

private:
    char writeBuffer[WRITEBUFFERSIZE];
    int writeIndex_;
    //std::vector<char> write;    
    std::shared_ptr<HttpProcessRead> pRead;

    struct iovec iv_[2];
    int ivCnt_;
};

#endif