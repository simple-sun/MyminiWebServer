#include"LogFile.h"
#include<assert.h>
#include<errno.h>
#include<string.h>
#include<time.h>


using namespace SUNSQ;

thread_local char t_strerror_buf[512];

LogFile::LogFile(const std::string &basename,
                    off_t rollSize,const int flushInterval):
    basename_(basename),
    writtenBytes(0),
    file_(::fopen(basename.c_str(),"ae")),
    mutex_(),
    rollSize_(rollSize),
    lastRoll_(0),
    flushInterval(flushInterval)
    {
        assert(basename.find('/') == std::string::npos);        
    }
LogFile::~LogFile()
{
    ::fclose(file_);
}

void LogFile::append(const char* buf, int len)
{
    mutex_.lock();
    if(writtenByte(file_) > rollSize_)
    {
        rollFile();
    }
    else{
        time_t now = time(NULL);
        if(now - lastFlush_ > flushInterval)
        {
            flush();
            lastFlush_ = now;
        }
    }
    mutex_.unlock();
}



void LogFile::writeToFile(const char* logLine, size_t len)
{
    size_t hadWrite = 0;

    while (hadWrite != len)
    {
        size_t n = ::fwrite(logLine+hadWrite,1,len,file_);
        size_t restToWrite = len -hadWrite;
        if(restToWrite < n)
        {
            int error_ = ferror(file_);
            if(error_)
            {
                fprintf(stderr,"LogFile::writeToFile failed s%\n", 
                        strerror_r (error_,t_strerror_buf,
                        sizeof(t_strerror_buf)));
                        break;
            }
        }hadWrite += n;
    } 

    writtenBytes += hadWrite;
}

bool LogFile::rollFile()
{
    time_t now = time(NULL);
    std::string filename = basename_;

    char timebuf[32];
    struct tm *tm;
    tm = localtime(&now);
    //time_t *now = &now;
  //gmtime_r(now, &tm); // FIXME: localtime_r ?
  //可以使用strftime（）函数将时间格式化为我们想要的格式
    strftime(timebuf, sizeof timebuf, ".%Y%m%d-%H%M%S.", tm);
    filename += timebuf;

    //每h更新一次
    time_t start = now/sPerHour * sPerHour;

    if(now > lastRoll_)
    {
        lastRoll_  = now;
        timeOfstartOfFile = start;
        //file_ = new (FILE);
        file_ = ::fopen(filename.c_str(),"ae");
        return true;   
    }
    return false;
}