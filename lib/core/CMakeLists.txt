project(libCore LANGUAGES CXX)

# SDL 2
find_package(SDL2 REQUIRED)

add_library(core 
            src/Application.cpp
            )

target_include_directories(core PUBLIC include)

target_link_libraries(core PRIVATE SDL2)
