project(libGraphic LANGUAGES CXX)

# SDL 2
find_package(SDL2 REQUIRED)

add_library(
    graphic
    src/DummyRenderable.cpp
    src/Font.cpp
    src/Image.cpp
    src/Renderable.cpp
    src/Renderer.cpp
    src/Text.cpp
    src/Texture.cpp
    src/TexturedRenderable.cpp
    src/TextureManager.cpp
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
    PRIVATE SDL2_ttf
)
