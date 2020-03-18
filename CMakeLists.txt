cmake_minimum_required(VERSION 3.0)
project(OWL)

SET(CMAKE_CXX_COMPILER "g++")
set( CMAKE_BUILD_TYPE Release )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread" )

INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/include)

add_executable(main
               ${PROJECT_SOURCE_DIR}/src/main.cc               
               ${PROJECT_SOURCE_DIR}/src/httpconn.cc
               ${PROJECT_SOURCE_DIR}/src/Epoll.cc
               ${PROJECT_SOURCE_DIR}/src/Socket.cc)