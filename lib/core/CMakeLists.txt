project(libCore LANGUAGES CXX)

# SDL 2
find_package(SDL2 REQUIRED)

add_library(
    core
    src/Application.cpp
    src/event/ApplicationEventListener.cpp
    src/event/Event.cpp
    src/event/EventDispatcher.cpp
    src/event/KeyboardEvent.cpp
    src/event/KeyboardEventListener.cpp
    src/event/MouseButtonEvent.cpp
    src/event/MouseEvent.cpp
    src/event/MouseEventListener.cpp
    src/event/MouseMotionEvent.cpp
)

target_include_directories(
    core
    PUBLIC include
    PRIVATE src
)

target_link_libraries(
    core
    PRIVATE SDL2
)
