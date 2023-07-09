cmake_minimum_required(VERSION 3.20)
project(encrypt)

set(CMAKE_CXX_STANDARD 23)

add_executable(encrypt main.cpp utils.h utils.cpp)

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
target_link_libraries(encrypt ${OPENSSL_LIBRARIES})


find_package(FLTK REQUIRED)
include_directories(${FLTK_INCLUDE_DIRS})
link_directories(${FLTK_LIBRARY_DIRS})
target_link_libraries(encrypt ${FLTK_LIBRARIES})

