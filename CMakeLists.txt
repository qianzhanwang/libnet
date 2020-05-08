cmake_minimum_required(VERSION 2.6)

#2.project name，指定项目的名称，一般和项目的文件夹名称对应
PROJECT(libnet)

#3.head file path，头文件目录
INCLUDE_DIRECTORIES(include)

#4.source directory，源文件目录
AUX_SOURCE_DIRECTORY(src DIR_SRCS)
AUX_SOURCE_DIRECTORY(include DIR_SRCS22)

# 编译选项
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Debug")
ENDIF(NOT CMAKE_BUILD_TYPE)

IF(WIN32)
ELSE()
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11 -O0 -Wall -g -ggdb -pthread")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11  -O3 -Wall")
ENDIF(WIN32)

#5.set environment variable，设置环境变量，编译用到的源文件全部都要放到这里，否则编译能够通过，但是执行的时候会出现各种问题，比如"symbol lookup error xxxxx , undefined symbol"
#SET(TEST_MATH ${DIR_SRCS})
#message("VAR DIR_SRCS= ${DIR_SRCS}")
#message("VAR TEST_MATH= ${TEST_MATH}")
#message("VAR PROJECT_NAME= ${PROJECT_NAME}")

#6.add executable file，添加要编译的可执行文件
ADD_EXECUTABLE(${PROJECT_NAME} ${DIR_SRCS} ${DIR_SRCS22})

#7.add link library，添加可执行文件所需要的库，比如我们用到了libm.so（命名规则：lib+name+.so），就添加该库的名称
TARGET_LINK_LIBRARIES(${PROJECT_NAME} m)