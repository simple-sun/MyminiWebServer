#include"BufferUnion.h"

using namespace SUNSQ;


template<int SIZE>
BufferUnion<SIZE>::BufferUnion(){}
template<int SIZE>
BufferUnion<SIZE>::~BufferUnion() {}


template<int SIZE>
void BufferUnion<SIZE>::append(const char* buf, int len)
{
    if(avail() > 0)
    {
       memcpy(cur_, buf, len); 
    }
}