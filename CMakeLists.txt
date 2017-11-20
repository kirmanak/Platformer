cmake_minimum_required(VERSION 3.8)
project(Project)

set(CMAKE_C_STANDARD 11)

set(BOTH_HEADERS
        include/connect.h
        include/network.h
        include/serialize.h
        include/structs.h)

set(BOTH_SOURCES
        src/network.c
        src/serialize.c)

set(CLIENT_SOURCES
        src/client.c
        src/movement.c
        ${BOTH_SOURCES})

set(SERVER_SOURCES
        src/server.c
        ${BOTH_SOURCES})

set(CLIENT_HEADERS
        include/main.h
        include/movement.h
        ${BOTH_HEADERS})

set(SERVER_HEADERS
        include/server.h
        ${BOTH_HEADERS})

set(prefix "/usr")
set(exec_prefix "${prefix}")
set(libdir "${exec_prefix}/lib")
set(SDL2_PREFIX "/usr")
set(SDL2_EXEC_PREFIX "/usr")
set(SDL2_LIBDIR "${exec_prefix}/lib")
set(SDL2_INCLUDE_DIRS "${prefix}/include/SDL2")
set(SDL2_LIBRARIES "-L${SDL2_LIBDIR} -lSDL2")

include_directories(include ${SDL2_INCLUDE_DIRS})

add_executable(game ${CLIENT_HEADERS} ${CLIENT_SOURCES})
target_link_libraries(game ${SDL2_LIBRARIES} -lSDL2_image)
add_executable(server ${SERVER_HEADERS} ${SERVER_SOURCES})
