cmake_minimum_required(VERSION 3.20)
project(SuperMarioProject VERSION 1.0 LANGUAGES CXX)

# Require C++20 standard (needed for SFML 3)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Sửa lỗi compatibility với FreeType của SFML trên các bản CMake mới
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# ==============================================================================
# TỰ ĐỘNG TẢI & TÍCH HỢP SFML 3.0 VIA FETCHCONTENT
# ==============================================================================
include(FetchContent)

# Tắt build các phần không cần thiết của SFML để tăng tốc độ biên dịch
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOCS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.0.0 # Tải chính xác bản SFML 3.0.0 Stable
)
FetchContent_MakeAvailable(SFML)
# ==============================================================================

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