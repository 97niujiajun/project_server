cmake_minimum_required(VERSION 3.25)
project(project_server C)

set(CMAKE_C_STANDARD 11)

add_executable(project_server server.c shm.c shm.h sem.c sem.h)
add_executable(project_client client.c shm.c shm.h sem.c sem.h)