cmake_minimum_required(VERSION 3.7)
project(transport)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/utils.h src/udpsocket.cpp src/udpsocket.h src/packet.cpp src/packet.h src/packetbuffer.cpp src/packetbuffer.h)
add_executable(transport ${SOURCE_FILES})