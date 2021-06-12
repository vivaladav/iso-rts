project(libUtilities LANGUAGES CXX)

# SDL 2
find_package(SDL2 REQUIRED)

add_library(
    utilities
    include/utilities/ISystem.h
    include/utilities/LoadedDie.h
    include/utilities/State.h
    include/utilities/StateManager.h
    include/utilities/System.h
    $<$<PLATFORM_ID:Linux>:include/utilities/linux/System.h>
    $<$<PLATFORM_ID:Windows>:include/utilities/windows/System.h>
    include/utilities/UniformDistribution.h
    src/LoadedDie.cpp
    src/State.cpp
    src/StateManager.cpp
    $<$<PLATFORM_ID:Linux>:src/linux/System.cpp>
    $<$<PLATFORM_ID:Windows>:src/windows/System.cpp>
    src/UniformDistribution.cpp
)

target_include_directories(
    utilities
    PUBLIC include
)
