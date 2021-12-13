#ifndef TCPBUFFER_H
#define TCPBUFFER_H

#include"vector"

const int kInitPrepend = 8;
const int kInitWrite = 1536;

class TcpBuffer
{
public:
    TcpBuffer();
    ~TcpBuffer();

    void writeIn(const char data);
    void readOut(const char data);
    void readOut(const size_t dataLen) {readIndex_ += dataLen;}
    void shrink();

    int readableBytes() { return  writeIndex_-readIndex_; }
    const char* itAtRdIndex() { return &*bufferData_.begin()+readIndex_;}
    void reset()    
    {
        readIndex_ = kInitPrepend;
        writeIndex_ = kInitWrite;
    }

    size_t readFd(int fd, int* savedError);

private:

    void rePlace(std::vector<char> bufferData_);
    void reSizeBuffer(std::vector<char> bufferData_,int dataSize);
    std::vector<char> bufferData_;
    int readIndex_;
    int writeIndex_;

};


#endif