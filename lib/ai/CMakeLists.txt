project(libAI LANGUAGES CXX)

add_library(
    ai
    include/ai/IPathMap.h
    include/ai/Pathfinder.h
    src/Pathfinder.cpp
    src/PathNode.h
)

target_include_directories(
    ai
    PUBLIC include
)
