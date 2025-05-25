cmake_minimum_required(VERSION 3.31)
project(hotel_management)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

add_executable(hotel_management
        main.cpp
        src/Persoana.cpp
        src/Client.cpp
        src/Receptioner.cpp
        src/Camera.cpp
        src/Hotel.cpp
        src/exceptions.cpp
        include/exceptions.h
        include/utils.h
        include/Persoana.h
        include/Client.h
        include/Receptioner.h
        include/Camera.h
        include/Hotel.h
        src/exceptions.cpp
        src/Persoana.cpp
        src/Client.cpp
        src/Receptioner.cpp
        src/Camera.cpp
        src/Hotel.cpp
)

target_include_directories(hotel_management PRIVATE include)
enable_testing()