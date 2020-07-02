project(libUtilities LANGUAGES CXX)

# SDL 2
find_package(SDL2 REQUIRED)

add_library(
    utilities
    include/utilities/State.h
    include/utilities/StateManager.h
    src/State.cpp
    src/StateManager.cpp
)

target_include_directories(
    utilities
    PUBLIC include
)
