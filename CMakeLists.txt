cmake_minimum_required(VERSION 3.0)

project(SOCKET_COMMUNICATION)

set(CMAKE_C_FLAGS_DEBUG -g)
# set(CMAKE_C_COMPILER gcc)

set (SOCKET_COM 0)
set (SOURCE_CLIENT main.c client.c client.h types.h)
add_executable(client ${SOURCE_CLIENT})
target_compile_definitions(client PRIVATE -DSOCKET_COM=${SOCKET_COM})

set (SOCKET_COM 1)
set (SOURCE_SERVER main.c server.c server.h types.h)
add_executable(server ${SOURCE_SERVER})
target_compile_definitions(server PRIVATE -DSOCKET_COM=${SOCKET_COM})

