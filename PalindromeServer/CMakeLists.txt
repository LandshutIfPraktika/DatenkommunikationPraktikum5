cmake_minimum_required(VERSION 3.0)



project(Server)
set(SOURCE_FILES_SERVER Server.c Server.h inet.h)
add_executable(Server ${SOURCE_FILES_SERVER})


project(Client)
set(SOURCE_FILES_CLIENT Client.c Client.h inet.h)
add_executable(Client ${SOURCE_FILES_CLIENT})

