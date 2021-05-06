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
    src/GameData.h
    src/GameData.cpp
    src/GameMap.h
    src/GameMap.cpp
    src/GameMapCell.cpp
    src/GameMapCell.h
    src/IsoLayer.h
    src/IsoLayer.cpp
    src/IsoMap.h
    src/IsoMap.cpp
    src/IsoObject.h
    src/IsoObject.cpp
    src/MapLoader.h
    src/MapLoader.cpp
    src/Player.h
    src/Player.cpp
    src/StatValue.h
    src/StatValue.cpp
    src/AI/ActionAI.h
    src/AI/ConquerPath.h
    src/AI/ConquerPath.cpp
    src/AI/ObjectPath.h
    src/AI/ObjectPath.cpp
    src/AI/PlayerAI.h
    src/AI/PlayerAI.cpp
    src/AI/WallBuildPath.h
    src/AI/WallBuildPath.cpp
    src/GameObjects/Base.h
    src/GameObjects/Base.cpp
    src/GameObjects/Blobs.h
    src/GameObjects/Blobs.cpp
    src/GameObjects/BlobsGenerator.h
    src/GameObjects/BlobsGenerator.cpp
    src/GameObjects/Collectable.h
    src/GameObjects/Collectable.cpp
    src/GameObjects/CollectableGenerator.h
    src/GameObjects/CollectableGenerator.cpp
    src/GameObjects/Diamonds.h
    src/GameObjects/Diamonds.cpp
    src/GameObjects/DiamondsGenerator.h
    src/GameObjects/DiamondsGenerator.cpp
    src/GameObjects/GameObject.h
    src/GameObjects/GameObject.cpp
    src/GameObjects/GameObjectAction.h
    src/GameObjects/ResourceGenerator.h
    src/GameObjects/ResourceGenerator.cpp
    src/GameObjects/SceneObject.h
    src/GameObjects/SceneObject.cpp
    src/GameObjects/Unit.h
    src/GameObjects/Unit.cpp
    src/GameObjects/Wall.h
    src/GameObjects/Wall.cpp
    src/Indicators/ConquestIndicator.h
    src/Indicators/ConquestIndicator.cpp
    src/Indicators/MoveIndicator.h
    src/Indicators/MoveIndicator.cpp
    src/Indicators/WallIndicator.h
    src/Indicators/WallIndicator.cpp
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
    src/States/StatesIds.h
    src/States/StateTest.h
    src/States/StateTest.cpp
    src/Widgets/ButtonCloseDialog.h
    src/Widgets/ButtonCloseDialog.cpp
    src/Widgets/ButtonMainMenu.h
    src/Widgets/ButtonMainMenu.cpp
    src/Widgets/ButtonNavigation.h
    src/Widgets/ButtonNavigation.cpp
    src/Widgets/ButtonPanelPlayer.h
    src/Widgets/ButtonPanelPlayer.cpp
    src/Widgets/ButtonUnitsSelector.h
    src/Widgets/ButtonUnitsSelector.cpp
    src/Widgets/ButtonsPanel.h
    src/Widgets/ButtonsPanel.cpp
    src/Widgets/CellProgressBar.h
    src/Widgets/CellProgressBar.cpp
    src/Widgets/DialogNewUnit.h
    src/Widgets/DialogNewUnit.cpp
    src/Widgets/DigitsDisplay.h
    src/Widgets/DigitsDisplay.cpp
    src/Widgets/GameButton.h
    src/Widgets/GameButton.cpp
    src/Widgets/GameUIData.h
    src/Widgets/GameUIData.cpp
    src/Widgets/MapPreview.h
    src/Widgets/MapPreview.cpp
    src/Widgets/ObjectActionButton.h
    src/Widgets/ObjectActionButton.cpp
    src/Widgets/PanelGameOver.h
    src/Widgets/PanelGameOver.cpp
    src/Widgets/PanelGameWon.h
    src/Widgets/PanelGameWon.cpp
    src/Widgets/PanelObjectActions.h
    src/Widgets/PanelObjectActions.cpp
    src/Widgets/PanelPlayer.h
    src/Widgets/PanelPlayer.cpp
    src/Widgets/PanelResources.h
    src/Widgets/PanelResources.cpp
    src/Widgets/ProgressBarResources.h
    src/Widgets/ProgressBarResources.cpp
    src/Widgets/ResourceDisplay.h
    src/Widgets/ResourceDisplay.cpp
    src/Widgets/ShortcutButton.h
    src/Widgets/ShortcutButton.cpp
    src/Widgets/SlotDialogNewUnit.h
    src/Widgets/SlotDialogNewUnit.cpp
    src/Widgets/UnitsSelector.h
    src/Widgets/UnitsSelector.cpp
)

target_include_directories(
    game
    PRIVATE src/
)

target_link_libraries(
    game
    PRIVATE ai
    PRIVATE core
    PRIVATE graphic
    PRIVATE sgui
    PRIVATE utilities
)

# copy data to build directory when CMake is executed
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data/)
file(COPY ${CMAKE_SOURCE_DIR}/data/ DESTINATION ${CMAKE_BINARY_DIR}/data/)
