project(libSGui LANGUAGES CXX)

# SDL 2
find_package(SDL2 REQUIRED)

add_library(
    sgui
    include/sgui/ButtonsGroup.h
    include/sgui/Label.h
    include/sgui/ProgressBar.h
    include/sgui/PushButton.h
    include/sgui/Stage.h
    include/sgui/WidgetContainer.h
    include/sgui/Widget.h
    src/ButtonsGroup.cpp
    src/Label.cpp
    src/ProgressBar.cpp
    src/PushButton.cpp
    src/Stage.cpp
    src/Widget.cpp
    src/WidgetContainer.cpp
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
