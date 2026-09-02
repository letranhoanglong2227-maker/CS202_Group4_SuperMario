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

add_executable(RuntimeContracts
    tests/RuntimeContracts.cpp
)
target_link_libraries(RuntimeContracts PRIVATE SuperMarioCore)
if(MSVC)
    target_compile_options(RuntimeContracts PRIVATE /W4)
else()
    target_compile_options(RuntimeContracts PRIVATE -Wall -Wextra)
endif()

enable_testing()
add_test(NAME RuntimeContracts COMMAND RuntimeContracts)
set_tests_properties(RuntimeContracts PROPERTIES
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)

function(add_core_contract TEST_NAME TEST_SOURCE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME} PRIVATE SuperMarioCore)
    if(MSVC)
        target_compile_options(${TEST_NAME} PRIVATE /W4)
    else()
        target_compile_options(${TEST_NAME} PRIVATE -Wall -Wextra)
    endif()
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    set_tests_properties(${TEST_NAME} PROPERTIES WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
endfunction()

add_core_contract(EntityFoundationContracts tests/EntityFoundationContracts.cpp)
add_core_contract(PlayerContracts tests/PlayerContracts.cpp)
add_core_contract(EnemyContracts tests/EnemyContracts.cpp)
add_core_contract(ItemFactoryContracts tests/ItemFactoryContracts.cpp)

add_executable(StateFlowContracts tests/StateFlowContracts.cpp)
target_link_libraries(StateFlowContracts PRIVATE SuperMarioCore)
add_test(NAME StateFlowContracts COMMAND StateFlowContracts)
set_tests_properties(StateFlowContracts PROPERTIES
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)

add_executable(ApplicationSmoke tests/ApplicationSmoke.cpp)
target_link_libraries(ApplicationSmoke PRIVATE SuperMarioCore)
add_test(
    NAME ApplicationSmoke
    COMMAND ApplicationSmoke "${CMAKE_BINARY_DIR}"
)
set_tests_properties(ApplicationSmoke PROPERTIES
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)

add_executable(GuiContracts
    tests/GuiContracts.cpp
    src/UI/GUI.cpp
)
target_include_directories(GuiContracts PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)
target_link_libraries(GuiContracts PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)
if(MSVC)
    target_compile_options(GuiContracts PRIVATE /W4)
else()
    target_compile_options(GuiContracts PRIVATE -Wall -Wextra)
endif()
add_test(NAME GuiContracts COMMAND GuiContracts)

add_executable(AnimationContracts
    tests/AnimationContracts.cpp
    src/Components/AnimationComponent.cpp
)
target_include_directories(AnimationContracts PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)
target_link_libraries(AnimationContracts PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)
if(MSVC)
    target_compile_options(AnimationContracts PRIVATE /W4)
else()
    target_compile_options(AnimationContracts PRIVATE -Wall -Wextra)
endif()
add_test(NAME AnimationContracts COMMAND AnimationContracts)

add_executable(AssetLocatorContracts
    tests/AssetLocatorContracts.cpp
    src/Core/AssetLocator.cpp
)
target_include_directories(AssetLocatorContracts PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)
if(MSVC)
    target_compile_options(AssetLocatorContracts PRIVATE /W4)
else()
    target_compile_options(AssetLocatorContracts PRIVATE -Wall -Wextra)
endif()
add_test(NAME AssetLocatorContracts COMMAND AssetLocatorContracts)
set_tests_properties(AssetLocatorContracts PROPERTIES
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)

