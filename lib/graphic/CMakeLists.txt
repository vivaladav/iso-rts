project(libGraphic LANGUAGES CXX)

# SDL 2
find_package(SDL2 REQUIRED)

add_library(
    graphic
    include/graphic/Camera.h
    include/graphic/DummyRenderable.h
    include/graphic/Font.h
    include/graphic/FontManager.h
    include/graphic/GraphicConstants.h
    include/graphic/Image.h
    include/graphic/Particle.h
    include/graphic/ParticleData.h
    include/graphic/ParticlesManager.h
    include/graphic/ParticlesUpdater.h
    include/graphic/Renderable.h
    include/graphic/Renderer.h
    include/graphic/Text.h
    include/graphic/TexturedRenderable.h
    include/graphic/Texture.h
    include/graphic/TextureData.h
    include/graphic/TextureManager.h
    include/graphic/Window.h
    src/Camera.cpp
    src/DummyRenderable.cpp
    src/Font.cpp
    src/FontManager.cpp
    src/Image.cpp
    src/ParticlesManager.cpp
    src/ParticlesUpdater.cpp
    src/Renderable.cpp
    src/Renderer.cpp
    src/Text.cpp
    src/Texture.cpp
    src/TextureData.cpp
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
    PUBLIC core
)
