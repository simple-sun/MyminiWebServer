#include"TcpBuffer.h"
#include"sys/uio.h"

TcpBuffer::TcpBuffer()
        : readIndex_(kInitPrepend),
        writeIndex_(kInitPrepend),
        bufferData_((kInitWrite+kInitPrepend))
        {}
TcpBuffer::~TcpBuffer(){}

void TcpBuffer::rePlace(std::vector<char> bufferData_)
{
    std::vector<char> (bufferData_.begin()+readIndex_,
                            bufferData_.begin()+readIndex_+writeIndex_).swap(bufferData_);
            bufferData_.resize(kInitPrepend+kInitWrite); 
}

void TcpBuffer::reSizeBuffer(std::vector<char> bufferData_, int dataSize)
{
    std::vector<decltype(bufferData_)::value_type>
                            (bufferData_.begin() + readIndex_, 
                            bufferData_.begin() + writeIndex_).swap(bufferData_);
        bufferData_.resize(dataSize + writeIndex_ - readIndex_);   
}

void TcpBuffer::writeIn(const char data)
{
    int dataSize = sizeof(data);
    if(dataSize < (sizeof(bufferData_) - writeIndex_))
    {
        bufferData_.push_back(data);
        writeIndex_ += dataSize;
    }else
    {
        if((readIndex_ + sizeof(bufferData_) - writeIndex_) > dataSize)
        {            
            rePlace(bufferData_); 
            bufferData_.push_back(data);     
            writeIndex_ += dataSize;     
        }
        else{        
        reSizeBuffer(bufferData_,dataSize);
        bufferData_.push_back(data);             
        readIndex_ = kInitPrepend;
        writeIndex_ = dataSize+writeIndex_-readIndex_;
        }
    }     
}

void TcpBuffer::readOut(const char data)
{
    int dataSize = sizeof(data);
    readIndex_ += dataSize;
    if(readIndex_ == writeIndex_)
    {
        readIndex_ = kInitPrepend;
        writeIndex_ = kInitPrepend;
    }
}

void TcpBuffer::shrink()
{
    bufferData_.resize(kInitPrepend+kInitWrite);
}

size_t TcpBuffer::readFd(int fd, int* savedError)
{
    char data[65536];
    struct iovec vec[1];
    vec[0].iov_base = data;
    vec[0].iov_len = sizeof(data);
    size_t n = readv(fd,vec,1);
    writeIn(*data);
}

