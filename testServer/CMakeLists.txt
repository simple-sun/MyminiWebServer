
#set(CMAKE_CXX_FLAGS"-std=c++11")
cmake_minimum_required(VERSION 3.2)
project(template)

#include_directories(TCP/inc)
#include_directories(LOG/inc)
#include_directories(time/inc)
#include_directories(old/inc)
#include_directories(HTTP/inc)

#aux_source_directory(TCP/src DIR_SRCS)
#aux_source_directory(LOG/src DIR_SRCS)
#aux_source_directory(time/src DIR_SRCS)
#aux_source_directory(old/src DIR_SRCS)
#aux_source_directory(HTTP/src DIR_SRCS)


aux_source_directory(./ DIR_SRCS)

set(TEST_MATH ${DIR_SRCS})   #这句话要是不加，则不会生成src下的目录


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -lpthread -DDEBUG")   #对应g++的输入参数

add_executable(out.exe ${TEST_MATH})

