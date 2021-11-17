

cmake_minimum_required(VERSION 2.8)
project(template)

include_directories(inc)

aux_source_directory(src DIR_SRCS)

set(TEST_MATH ${DIR_SRCS})   #这句话要是不加，则不会生成src下的目录


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -lpthread -DDEBUG")   #对应g++的输入参数

add_executable(out.exe ${TEST_MATH}) 

