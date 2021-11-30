#include"BufferUnion.h"
#include<string.h>

using namespace SUNSQ;


// template<int SIZE>
// BufferUnion<SIZE>::BufferUnion(){}
// template<int SIZE>
// BufferUnion<SIZE>::~BufferUnion() {}
template class BufferUnion<BUFFERSIZE>;

//template<int SIZE>
//void BufferUnion<SIZE>::bzero()


template<int SIZE>
void BufferUnion<SIZE>::append(const char* buf, int len)
{
    if(avail() > 0)
    {
       memcpy(cur_, buf, len); 
    }
}

template<int SIZE>
int BufferUnion<SIZE>::length ()const
{
    return static_cast<int> (cur_ - data_);
}



// template<int SIZE>
// void BufferUnion<SIZE>::release()
// {
//     data_.erase();
//     cur_ = "";
//     end();
// }