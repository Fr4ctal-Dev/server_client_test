cmake_minimum_required(VERSION 3.14)
project(server_client_test C)

set(CMAKE_C_STANDARD 99)

add_executable(server_client_test main.c)
add_executable(client client.c)
add_executable(server server.c)