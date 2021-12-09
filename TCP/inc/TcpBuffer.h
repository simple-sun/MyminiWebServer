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

    void writeIn(char data);
    void readOut(char data);

    void shrink();

private:
    std::vector<char> bufferData_;
    int readIndex_;
    int writeIndex_;

};


#endif