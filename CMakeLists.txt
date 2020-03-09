cmake_minimum_required (VERSION 2.6)
project(CHttpServer)
set(CMAKE_BUILD_TYPE "Debug")
#设置debug模式下的编译参数
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O2 -Werror -g -ggdb")
add_compile_options(-std=c++11 -pthread)
add_subdirectory(server)

