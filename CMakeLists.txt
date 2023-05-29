cmake_minimum_required(VERSION 3.23)
project(encrypt)

set(CMAKE_CXX_STANDARD 23)

set(OPENSSL_INCLUDE_DIR "C:/Program Files/FireDaemon OpenSSL 3/include")
set(OPENSSL_CRYPTO_LIBRARY "C:/Program Files/FireDaemon OpenSSL 3/lib/libcrypto.lib")

add_executable(encrypt main.cpp)

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
target_link_libraries(encrypt ${OPENSSL_LIBRARIES})

