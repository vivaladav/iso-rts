cmake_minimum_required(VERSION 3.5.1)

project(iso-rts LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SDL 2
find_package(SDL2 REQUIRED)

# include internal libraries
add_subdirectory(lib)

# main executable
add_executable(	game
				src/main.cpp
				)

target_link_libraries(	game
						PRIVATE SDL2
						PRIVATE graphic
						)
