cmake_minimum_required(VERSION 3.5.1)

project(iso-rts LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include internal libraries
add_subdirectory(lib)

# main executable
add_executable(
    game
    src/main.cpp
    src/Game.cpp
    src/IsoMap.cpp
    src/Player.cpp
    src/Screens/Screen.cpp
    src/Screens/ScreenGame.cpp
    src/Screens/ScreenMainMenu.cpp
    src/Screens/ScreenTest.cpp
    src/Screens/SharedScreenListener.cpp
    src/States/BaseGameState.cpp
    src/States/StateGame.cpp
    src/States/StateMainMenu.cpp
    src/States/StateTest.cpp
    src/Widgets/PanelPlayer.cpp
)

target_include_directories(
    game
    PRIVATE src/
)

target_link_libraries(
    game
    PRIVATE core
    PRIVATE graphic
    PRIVATE sgui
    PRIVATE utilities
)

# copy data to build directory when CMake is executed
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data/)
file(COPY ${CMAKE_SOURCE_DIR}/data/ DESTINATION ${CMAKE_BINARY_DIR}/data/)
