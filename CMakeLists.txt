cmake_minimum_required(VERSION 3.8)
project(Platformer)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Werror -pedantic -pedantic-errors -O3")

set(BOTH_HEADERS
        include/connect.h
        include/serialize.h
        include/structs.h)

set(BOTH_SOURCES
        src/serialize.c)

set(CLIENT_SOURCES
        src/client.c
        src/movement.c
        src/network.c
        ${BOTH_SOURCES})

set(SERVER_SOURCES
        src/server.c
        ${BOTH_SOURCES})

set(CLIENT_HEADERS
        include/main.h
        include/movement.h
        include/network.h
        ${BOTH_HEADERS})

set(SERVER_HEADERS
        include/server.h
        ${BOTH_HEADERS})

include_directories(include)

add_executable(game ${CLIENT_HEADERS} ${CLIENT_SOURCES})
target_link_libraries(game -lSDL2 -lm -lSDL2_image)
add_executable(server ${SERVER_HEADERS} ${SERVER_SOURCES})
