project(libCore LANGUAGES CXX)

# SDL 2
find_package(SDL2 REQUIRED)

add_library(
    core
    include/core/Application.h
    include/core/Point.h
    include/core/Rect.h
    include/core/Size.h
    include/core/event/ApplicationEvent.h
    include/core/event/ApplicationEventListener.h
    include/core/event/Event.h
    include/core/event/KeyboardEvent.h
    include/core/event/KeyboardEventListener.h
    include/core/event/MouseButtonEvent.h
    include/core/event/MouseEvent.h
    include/core/event/MouseEventListener.h
    include/core/event/MouseMotionEvent.h
    src/Application.cpp
    src/event/ApplicationEventListener.cpp
    src/event/Event.cpp
    src/event/EventDispatcher.h
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
