cmake_minimum_required(VERSION 3.20)

if(WIN32)
    find_program(NATIVE_RC NAMES windres.exe HINTS "C:/msys64/ucrt64/bin" "C:/msys64/mingw64/bin" "C:/msys64/clang64/bin")
    if(NATIVE_RC)
        set(CMAKE_RC_COMPILER "${NATIVE_RC}" CACHE FILEPATH "RC compiler" FORCE)
    endif()
endif()

project(SuperMarioProject VERSION 1.0 LANGUAGES C CXX)

# Require C++20 standard (needed for SFML 3)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Sửa lỗi compatibility với FreeType của SFML trên các bản CMake mới
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# ==============================================================================
# DÙNG SFML 3.1 ĐÃ CÀI, TỰ ĐỘNG TẢI NẾU CHƯA CÓ
# ==============================================================================
set(SFML_STATIC_LIBRARIES ON)
find_package(SFML 3.1 COMPONENTS Graphics Audio Window System CONFIG QUIET)

if(NOT SFML_FOUND)
    include(FetchContent)

    # Tắt build các phần không cần thiết của SFML để tăng tốc độ biên dịch
    set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_DOCS OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        3.1.0
    )
    FetchContent_MakeAvailable(SFML)
endif()
# ==============================================================================

# Gather source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
)

set(GAME_CORE_SOURCES ${SOURCES})
list(FILTER GAME_CORE_SOURCES EXCLUDE REGEX "[/\\\\]main\\.cpp$")

add_library(SuperMarioCore STATIC ${GAME_CORE_SOURCES})

target_include_directories(SuperMarioCore PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

target_link_libraries(SuperMarioCore PUBLIC
    SFML::Graphics
    SFML::Audio
    SFML::Window
    SFML::System
)

if(MSVC)
    target_compile_options(SuperMarioCore PRIVATE /W4)
else()
    target_compile_options(SuperMarioCore PRIVATE -Wall -Wextra)
endif()

add_executable(SuperMario src/main.cpp)
target_link_libraries(SuperMario PRIVATE SuperMarioCore)

