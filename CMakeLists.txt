cmake_minimum_required (VERSION 2.6)
set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
project (WUIPM)

set(Header
       wuipm_node.h
       wuipm_tree.h
   )

set(Source
       wuipm_node.cpp
       wuipm_tree.cpp
   )

set(Example
       main.cpp
   )

add_library(wuipm ${Header} ${Source})

add_executable(wuipmdemo ${Header} ${Source} ${Example})

