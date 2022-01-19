cmake_minimum_required(VERSION 3.5.1)

project(iso-rts LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set platform version for preprocessor checks
add_compile_definitions($<$<PLATFORM_ID:Linux>:LINUX=1>
                        $<$<PLATFORM_ID:Windows>:WINDOWS=1>)

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
    src/GameObjects/DefensiveTower.h
    src/GameObjects/DefensiveTower.cpp
    src/GameObjects/Diamonds.h
    src/GameObjects/Diamonds.cpp
    src/GameObjects/DiamondsGenerator.h
    src/GameObjects/DiamondsGenerator.cpp
    src/GameObjects/GameObject.h
    src/GameObjects/GameObject.cpp
    src/GameObjects/GameObjectAction.h
    src/GameObjects/ObjectData.h
    src/GameObjects/ObjectData.cpp
    src/GameObjects/ObjectsDataRegistry.h
    src/GameObjects/ObjectsDataRegistry.cpp
    src/GameObjects/PracticeTarget.h
    src/GameObjects/PracticeTarget.cpp
    src/GameObjects/RadarStation.h
    src/GameObjects/RadarStation.cpp
    src/GameObjects/ResourceGenerator.h
    src/GameObjects/ResourceGenerator.cpp
    src/GameObjects/SceneObject.h
    src/GameObjects/SceneObject.cpp
    src/GameObjects/Structure.h
    src/GameObjects/Structure.cpp
    src/GameObjects/Unit.h
    src/GameObjects/Unit.cpp
    src/GameObjects/Wall.h
    src/GameObjects/Wall.cpp
    src/Indicators/ConquestIndicator.h
    src/Indicators/ConquestIndicator.cpp
    src/Indicators/MoveIndicator.h
    src/Indicators/MoveIndicator.cpp
    src/Indicators/StructureIndicator.h
    src/Indicators/StructureIndicator.cpp
    src/Indicators/WallIndicator.h
    src/Indicators/WallIndicator.cpp
    src/Particles/DataParticleDamage.h
    src/Particles/DataParticleSingleLaser.h
    src/Particles/ParticleDamage.h
    src/Particles/ParticleDamage.cpp
    src/Particles/ParticleSingleLaser.h
    src/Particles/ParticleSingleLaser.cpp
    src/Particles/UpdaterDamage.h
    src/Particles/UpdaterDamage.cpp
    src/Particles/UpdaterSingleLaser.h
    src/Particles/UpdaterSingleLaser.cpp
    src/Screens/Screen.h
    src/Screens/Screen.cpp
    src/Screens/ScreenFactionSelection.h
    src/Screens/ScreenFactionSelection.cpp
    src/Screens/ScreenGame.h
    src/Screens/ScreenGame.cpp
    src/Screens/ScreenInit.h
    src/Screens/ScreenInit.cpp
    src/Screens/ScreenMainMenu.h
    src/Screens/ScreenMainMenu.cpp
    src/Screens/ScreenNewGame.h
    src/Screens/ScreenNewGame.cpp
    src/Screens/ScreenSettings.h
    src/Screens/ScreenSettings.cpp
    src/Screens/ScreenTest.h
    src/Screens/ScreenTest.cpp
    src/Screens/SharedScreenListener.h
    src/Screens/SharedScreenListener.cpp
    src/States/BaseGameState.h
    src/States/BaseGameState.cpp
    src/States/StateFactionSelection.h
    src/States/StateFactionSelection.cpp
    src/States/StateGame.h
    src/States/StateGame.cpp
    src/States/StateInit.h
    src/States/StateInit.cpp
    src/States/StateMainMenu.h
    src/States/StateMainMenu.cpp
    src/States/StateNewGame.h
    src/States/StateNewGame.cpp
    src/States/StateSettings.h
    src/States/StateSettings.cpp
    src/States/StatesIds.h
    src/States/StateTest.h
    src/States/StateTest.cpp
    src/Widgets/ButtonBuildNewUnit.h
    src/Widgets/ButtonBuildNewUnit.cpp
    src/Widgets/ButtonDialogBack.h
    src/Widgets/ButtonDialogBack.cpp
    src/Widgets/ButtonDialogContinue.h
    src/Widgets/ButtonDialogContinue.cpp
    src/Widgets/ButtonDialogSelect.h
    src/Widgets/ButtonDialogSelect.cpp
    src/Widgets/ButtonMainMenu.h
    src/Widgets/ButtonMainMenu.cpp
    src/Widgets/ButtonMainMenuSocial.h
    src/Widgets/ButtonMainMenuSocial.cpp
    src/Widgets/ButtonMainMenuWishlist.h
    src/Widgets/ButtonMainMenuWishlist.cpp
    src/Widgets/ButtonNavigation.h
    src/Widgets/ButtonNavigation.cpp
    src/Widgets/ButtonUnitsSelector.h
    src/Widgets/ButtonUnitsSelector.cpp
    src/Widgets/ButtonsPanel.h
    src/Widgets/ButtonsPanel.cpp
    src/Widgets/CellProgressBar.h
    src/Widgets/CellProgressBar.cpp
    src/Widgets/DialogNewElement.h
    src/Widgets/DialogNewElement.cpp
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
    src/Widgets/PanelResources.h
    src/Widgets/PanelResources.cpp
    src/Widgets/ProgressBarResources.h
    src/Widgets/ProgressBarResources.cpp
    src/Widgets/ResourceDisplay.h
    src/Widgets/ResourceDisplay.cpp
    src/Widgets/ShortcutButton.h
    src/Widgets/ShortcutButton.cpp
    src/Widgets/SimpleResourceDisplay.h
    src/Widgets/SimpleResourceDisplay.cpp
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
