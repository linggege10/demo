cmake_minimum_required(VERSION 3.6)
project(echoServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/AyncClient.cpp src/FileUtil.cpp src/FileUtil.h)
add_executable(echoServer ${SOURCE_FILES})