cmake_minimum_required(VERSION 3.15)
project(StrategicBoardGame)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/board.cpp
    src/game.cpp
    src/ai.cpp
)

# Executable for standalone testing
add_executable(game_test
    src/main.cpp
    ${SOURCES}
)

# Shared library (DLL) for Flutter integration
add_library(game_engine SHARED
    ${SOURCES}
    src/api.cpp
)

# Set output directory
set_target_properties(game_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build
)

set_target_properties(game_engine PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build
)

# Enable warnings
if(MSVC)
    target_compile_options(game_test PRIVATE /W4)
    target_compile_options(game_engine PRIVATE /W4)
else()
    target_compile_options(game_test PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(game_engine PRIVATE -Wall -Wextra -Wpedantic)
endif()
