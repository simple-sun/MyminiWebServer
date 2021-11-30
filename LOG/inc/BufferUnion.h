#ifndef BUFFERINIT_H
#define BUFFERINIT_H

#include<boost/ptr_container/ptr_vector.hpp>
#include<vector>

/*
#define BUFFERSIZE =  4000*1000;
#define SIZE1 =  1000;
*/

namespace SUNSQ{

const int BUFFERSIZE = 4000*1000;


template<int SIZE>
class BufferUnion
{

public:
    BufferUnion():cur_(data_){};
    BufferUnion(const BufferUnion& ) = delete; 
    ~BufferUnion(){};

    void bzero()
        { memset(data_,0,sizeof(data_));};
    void append(const char* buf, int len);
    int avail() { return static_cast<int>( end() - cur_ ); };
    int length () const;
    //void release();
    
private:
    const char* end()  { return data_ + sizeof(data_); };
    //const int SIZE1 = 1000;
    size_t bufferSize;
    char* cur_;
    char data_[SIZE];

};
}

#endif