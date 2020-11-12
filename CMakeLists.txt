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
    src/Cell2D.h
    src/Game.h
    src/Game.cpp
    src/GameConstants.h
    src/GameMap.h
    src/GameMap.cpp
    src/GameMapCell.h
    src/IsoLayer.h
    src/IsoLayer.cpp
    src/IsoMap.h
    src/IsoMap.cpp
    src/Player.h
    src/Player.cpp
    src/AI/ActionAI.h
    src/AI/PlayerAI.h
    src/AI/PlayerAI.cpp
    src/Screens/Screen.h
    src/Screens/Screen.cpp
    src/Screens/ScreenGame.h
    src/Screens/ScreenGame.cpp
    src/Screens/ScreenMainMenu.h
    src/Screens/ScreenMainMenu.cpp
    src/Screens/ScreenNewGame.h
    src/Screens/ScreenNewGame.cpp
    src/Screens/ScreenTest.h
    src/Screens/ScreenTest.cpp
    src/Screens/SharedScreenListener.h
    src/Screens/SharedScreenListener.cpp
    src/States/BaseGameState.h
    src/States/BaseGameState.cpp
    src/States/StateGame.h
    src/States/StateGame.cpp
    src/States/StateMainMenu.h
    src/States/StateMainMenu.cpp
    src/States/StateNewGame.h
    src/States/StateNewGame.cpp
    src/States/StateTest.h
    src/States/StateTest.cpp
    src/Widgets/ButtonMainMenu.h
    src/Widgets/ButtonMainMenu.cpp
    src/Widgets/ButtonPanelPlayer.h
    src/Widgets/ButtonPanelPlayer.cpp
    src/Widgets/ButtonUnitsSelector.h
    src/Widgets/ButtonUnitsSelector.cpp
    src/Widgets/ButtonsPanel.h
    src/Widgets/ButtonsPanel.cpp
    src/Widgets/CellProgressBar.h
    src/Widgets/CellProgressBar.cpp
    src/Widgets/GameButton.h
    src/Widgets/GameButton.cpp
    src/Widgets/PanelGameOver.h
    src/Widgets/PanelGameOver.cpp
    src/Widgets/PanelGameWon.h
    src/Widgets/PanelGameWon.cpp
    src/Widgets/PanelPlayer.h
    src/Widgets/PanelPlayer.cpp
    src/Widgets/UnitsSelector.h
    src/Widgets/UnitsSelector.cpp
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
