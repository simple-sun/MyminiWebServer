#ifndef LOG_THREAD
#define LOG_THREAD

#include <vector>
#include <queue>
#include <thread>
#include <mutex>

#include "logger.h"
#include <iostream>



const bool enableDebug=1;

const bool enableHttpDebug=1;


const log::LogLevel logLevel=log::LogLevel::INFO_;


#define LOG_INFO  if(logLevel<=log::LogLevel::INFO_)\
                    log::entity->logger.receive(__FILE__,__LINE__,__func__,log::INFO_)

#define LOG_DEBUG if(logLevel<=log::LogLevel::DEBUG_)\
                    log::entity->logger.receive(__FILE__,__LINE__,__func__,log::DEBUG_)

#define LOG_FATAL if(logLevel<=log::LogLevel::FATAL_)\
                    log::entity->logger.receive(__FILE__,__LINE__,__func__,log::FATAL_)                    
                    


#define LOG_HTTP  if(enableHttpDebug)\
                    log::entity->logger.receive(__FILE__,__LINE__,__func__,log::HTTP_)

#define LOG_COUT std::cout


namespace log
{



    class LogThread
    {
    public:
        ~LogThread();

        static LogThread *init();

        void apendMessage(char *line, int size); //这里size不包含 '\0'

        unsigned int freeSize() { return static_cast<int>(maxBufSize - currentBufIndex_); }

        void changeBuf();

        void logLoop();

        Logger logger;

    private:
        LogThread();
        void flushToFile(std::vector<std::pair<char *, int>> &buf);
        void deleteBuf(std::vector<std::pair<char *, int>> &buf, int beginIndex);

        LogFile logFile_;

        bool logging_;

        char *currentBuf_;
        int currentBufIndex_;
        char *preBuf_;

        std::vector<std::pair<char *, int>> readyBuf_;
        std::vector<std::pair<char *, int>> bufToWrite_;

        std::mutex mut_;

        static const int maxBufSize;
    };

    extern LogThread *entity;
    extern const char *logPath;
}

#endif
