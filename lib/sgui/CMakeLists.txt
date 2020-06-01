project(libSGui LANGUAGES CXX)

# SDL 2
find_package(SDL2 REQUIRED)

add_library(
    sgui
    src/PushButton.cpp
    src/Widget.cpp
)

target_link_libraries(
    sgui
    PUBLIC core
    PUBLIC graphic
)

target_include_directories(
    sgui
    PUBLIC include
)
