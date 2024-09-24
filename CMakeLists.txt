cmake_minimum_required(VERSION 3.0)

project(SOCKET_COMMUNICATION)

set(CMAKE_C_FLAGS_DEBUG -g)
# set(CMAKE_C_COMPILER gcc)

set (SOURCE_CLIENT client_main.c client.c client.h types.h)
set (SOURCE_SERVER server_main.c server.c server.h types.h)

add_executable(client ${SOURCE_CLIENT})

add_executable(server ${SOURCE_SERVER})
