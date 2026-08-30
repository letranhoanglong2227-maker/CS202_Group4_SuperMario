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

set(PERSON2_TEST_SOURCES ${SOURCES})
list(FILTER PERSON2_TEST_SOURCES EXCLUDE REGEX "[/\\\\]main\\.cpp$")
add_executable(Person2RuntimeContracts
    tests/Person2RuntimeContracts.cpp
    ${PERSON2_TEST_SOURCES}
)
target_include_directories(Person2RuntimeContracts PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)
target_link_libraries(Person2RuntimeContracts PRIVATE
    SFML::Graphics
    SFML::Audio
    SFML::Window
    SFML::System
)
if(MSVC)
    target_compile_options(Person2RuntimeContracts PRIVATE /W4)
else()
    target_compile_options(Person2RuntimeContracts PRIVATE -Wall -Wextra)
endif()

enable_testing()
add_test(NAME Person2RuntimeContracts COMMAND Person2RuntimeContracts)
set_tests_properties(Person2RuntimeContracts PROPERTIES
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)


add_executable(Person4AnimationContracts
    tests/Person4AnimationContracts.cpp
    ${PERSON2_TEST_SOURCES}
)
target_include_directories(Person4AnimationContracts PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)
target_link_libraries(Person4AnimationContracts PRIVATE
    SFML::Graphics
    SFML::Audio
    SFML::Window
    SFML::System
)
if(MSVC)
    target_compile_options(Person4AnimationContracts PRIVATE /W4)
else()
    target_compile_options(Person4AnimationContracts PRIVATE -Wall -Wextra)
endif()

add_test(NAME Person4AnimationContracts COMMAND Person4AnimationContracts)
set_tests_properties(Person4AnimationContracts PROPERTIES
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/Person1ApplicationSmoke.cpp")
    add_executable(Person1ApplicationSmoke tests/Person1ApplicationSmoke.cpp ${PERSON2_TEST_SOURCES})
    target_include_directories(Person1ApplicationSmoke PRIVATE "${PROJECT_SOURCE_DIR}/include")
    target_link_libraries(Person1ApplicationSmoke PRIVATE SFML::Graphics SFML::Audio SFML::Window SFML::System)
    add_test(NAME Person1ApplicationSmoke COMMAND Person1ApplicationSmoke)
    set_tests_properties(Person1ApplicationSmoke PROPERTIES WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/Person1StateFlowContracts.cpp")
    add_executable(Person1StateFlowContracts tests/Person1StateFlowContracts.cpp ${PERSON2_TEST_SOURCES})
    target_include_directories(Person1StateFlowContracts PRIVATE "${PROJECT_SOURCE_DIR}/include")
    target_link_libraries(Person1StateFlowContracts PRIVATE SFML::Graphics SFML::Audio SFML::Window SFML::System)
    add_test(NAME Person1StateFlowContracts COMMAND Person1StateFlowContracts)
    set_tests_properties(Person1StateFlowContracts PROPERTIES WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
endif()

add_executable(Person4BlockContracts tests/Person4BlockContracts.cpp ${PERSON2_TEST_SOURCES})
target_include_directories(Person4BlockContracts PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(Person4BlockContracts PRIVATE SFML::Graphics SFML::Audio SFML::Window SFML::System)
if(MSVC)
    target_compile_options(Person4BlockContracts PRIVATE /W4)
else()
    target_compile_options(Person4BlockContracts PRIVATE -Wall -Wextra)
endif()

add_test(NAME Person4BlockContracts COMMAND Person4BlockContracts)
set_tests_properties(Person4BlockContracts PROPERTIES WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
