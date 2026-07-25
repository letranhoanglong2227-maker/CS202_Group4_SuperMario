cmake_minimum_required(VERSION 3.20)
project(SuperMarioProject VERSION 1.0 LANGUAGES CXX)

# Require C++20 standard (needed for SFML 3)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find SFML 3.x
find_package(SFML 3.0 REQUIRED COMPONENTS Graphics Audio Window System)

# Gather source files
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
)

# Define executable target
add_executable(SuperMario ${SOURCES})

# Target Include Directories pointing to include/
target_include_directories(SuperMario PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)

# Link SFML libraries
target_link_libraries(SuperMario PRIVATE
    SFML::Graphics
    SFML::Audio
    SFML::Window
    SFML::System
)

# Compilation warning flags configuration
if(MSVC)
    target_compile_options(SuperMario PRIVATE /W4)
else()
    target_compile_options(SuperMario PRIVATE -Wall -Wextra)
endif()
