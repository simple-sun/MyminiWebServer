#ifndef LOGGER_H
#define LOGGER_H

#include "LogStream.h"
#include "LogFile.h"
#include <functional>

namespace log
{
  enum LogLevel
{
  INFO_,
  DEBUG_,
  FATAL_,
  HTTP_
};


  class Logger
  {
  public:


    Logger(std::function<void(char*, int)>);

    const char* levelToStr(const LogLevel);

    LogStream &receive(const char *fileName, int line, const char *funName, LogLevel level);

    unsigned int  findFileName(const char*);

    std::string dealFunName(const char * funName);

  private:


    void dealLogStr(const char*,std::string& buf,int maxSize=15);


    std::string fileN;   //文件名
    std::string funN;    //函数名 
    std::string levelS;  //log等级
    
    LogStream stream_;

    static const int maxNameSize=25;   //文件名等缓冲区的最大容量

  };

}

#endif
