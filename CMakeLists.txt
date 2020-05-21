cmake_minimum_required(VERSION 3.5.1)

project(iso-rts LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include internal libraries
add_subdirectory(lib)

# main executable
add_executable(
    game
    src/Game.cpp
    src/main.cpp
    src/Screens/Screen.cpp
    src/Screens/ScreenTest.cpp
)

target_include_directories(
    game
    PRIVATE src/
)

target_link_libraries(
    game
    PRIVATE core
    PRIVATE graphic
    PRIVATE utilities
)

# copy data to build directory when CMake is executed
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data/)
file(COPY ${CMAKE_SOURCE_DIR}/data/ DESTINATION ${CMAKE_BINARY_DIR}/data/)
