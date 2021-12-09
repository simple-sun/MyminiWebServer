#include"TcpBuffer.h"


TcpBuffer::TcpBuffer()
        : readIndex_(kInitPrepend),
        writeIndex_(kInitPrepend),
        bufferData_((kInitWrite+kInitPrepend))
        {}
TcpBuffer::~TcpBuffer(){}

void TcpBuffer::writeIn(char data)
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
            std::vector<char> (bufferData_.begin()+readIndex_,
                            bufferData_.begin()+readIndex_+writeIndex_).swap(bufferData_);
            bufferData_.resize(kInitPrepend+kInitWrite);            
        }
        else{
        std::vector<decltype(bufferData_)::value_type>
                            (bufferData_.begin()+readIndex_, 
                            bufferData_.begin()+writeIndex_).swap(bufferData_);
        bufferData_.resize(dataSize+writeIndex_-readIndex_);   

        readIndex_ = kInitPrepend;
        writeIndex_ = dataSize+writeIndex_-readIndex_;
        }
    }     
}

void TcpBuffer::readOut(char data)
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