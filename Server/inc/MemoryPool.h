template<int ObjectSize, int  num = 20> class MemoryPool
{
private:
    const int mBlockSize; //每个内存块的大小
    const int ItemSize;     //每个内存节点的大小

    //内存节点结构
    struct FreeNode
    {
        FreeNode* pNext;
        char data[ObjectSize];
    };
    //内存块
    struct mBlock
    {
        mBlock *pNext;
        FreeNode data[num];
    };

    FreeNode* freeNodeHead;
    mBlock*   mBlockHead;  
    
public:
    MemoryPool();
    ~MemoryPool();

    void* malloc();
    void free(void* );
};


template<int ObjectSize, int  num> 
MemoryPool<ObjectSize,num>::MemoryPool()
            :ItemSize(ObjectSize + sizeof(FreeNode*)),
            mBlockSize(sizeof(mBlock*) + num*ItemSize)
{
    freeNodeHead = nullptr;
    mBlockHead = nullptr;
}

template<int ObjectSize, int  num> 
MemoryPool<ObjectSize,num>::~MemoryPool()
{
    mBlock* dPtr;
    while (mBlockHead)
    {
        dPtr = mBlockHead->pNext;
        delete mBlockHead;
        mBlockHead = dPtr;
    }    
}

template<int ObjectSize, int  num> 
void* MemoryPool<ObjectSize,num>::malloc()
{
    //无空闲节点
    if(freeNodeHead == nullptr)
    {
        mBlock* newBlock = new mBlock;
        newBlock->data[0].pNext = nullptr;
        for(int i = 1;i < num;i++)
        {
            newBlock->data[i].pNext = &(newBlock->data[i-1]);
        }
        freeNodeHead = &(newBlock->data[num-1]);
        newBlock->pNext = mBlockHead;
    }
    //返回空节点闲链表的第一个节点
    void* freeNode = freeNodeHead;
    freeNodeHead = freeNodeHead->pNext;
    return freeNode;
}

template<int ObjectSize, int  num> 
void MemoryPool<ObjectSize,num>::free(void* p)
{
    FreeNode* pNode = (FreeNode*)p;
    pNode->pNext = freeNodeHead;
    freeNodeHead = pNode;
}