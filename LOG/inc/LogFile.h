#ifndef LOGFILE_H
#define LOGFILE_H

#include<memory>    //对内存操作的头文件
#include<mutex>
#include<thread>
#include<string>

namespace SUNSQ
{

class LogFile
{
public:
    LogFile(const std::string &basename, off_t rollSize,
                const int flushInterval);
    LogFile(const LogFile &) = delete;
    ~LogFile();
    
    void append(const char* logline, int len);
    void flash();
    bool rollFile();
    off_t writtenByte(FILE* file_){    return writtenBytes;};
    void writeToFile(const char* logLine, size_t len);
    void flush()    {   ::fflush(file_);};
    

private:


    std::string basename_;
    std::mutex mutex_;
    FILE* file_;
    off_t writtenBytes;
    off_t rollSize_;
    
    time_t lastRoll_;
    time_t lastFlush_;
    time_t timeOfstartOfFile;
    

    const static int sPerHour = 60*60;
    const int flushInterval;
};


}







#endif