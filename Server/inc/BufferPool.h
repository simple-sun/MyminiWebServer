#ifndef BUFFERPOOL_H
#define BUFFERPOOL_H


template<typename T>
class BufferPool
{
public:
    BufferPool();
    ~BufferPool(){};

private:
    int size;
    int capacity;
    
};


template<typename T>
BufferPool<T>::BufferPool()
{

}



#endif