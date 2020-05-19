project(libGraphic LANGUAGES CXX)

# SDL 2
find_package(SDL2 REQUIRED)

add_library(
    graphic
    src/Image.cpp
    src/Renderer.cpp
    src/Texture.cpp
    src/Window.cpp
)

target_include_directories(
    graphic
    PUBLIC include
)

target_link_libraries(
    graphic
    PRIVATE SDL2
    PRIVATE SDL2_image
)
