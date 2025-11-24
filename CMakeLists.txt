cmake_minimum_required(VERSION 3.15)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")


# Get vcpkg
include(FetchContent)
FetchContent_Declare(
  vcpkg
  GIT_REPOSITORY https://github.com/microsoft/vcpkg.git
  GIT_TAG 2025.10.17
)
FetchContent_MakeAvailable(vcpkg)
# Set cmake to use this vcpkg version
set(CMAKE_TOOLCHAIN_FILE ${vcpkg_SOURCE_DIR}/scripts/buildsystems/vcpkg.cmake CACHE STRING "")



project(mysql-interface LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Find dependencies
message("=============================\nSearching for mysql-connector\n=============================")
#find_package(mysql-connector-cpp CONFIG REQUIRED)
find_package(unofficial-mysql-connector-cpp CONFIG REQUIRED)

# Collect my sourcefiles
file(GLOB_RECURSE my_sources src/*.cpp)

# Create an executable
add_library(${PROJECT_NAME} ${my_sources})

# Set my include dir
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Link the mysql connector
target_link_libraries(${PROJECT_NAME} PUBLIC unofficial::mysql-connector-cpp::connector)


option(TESTS "Enable unit and integration tests" ON)

if(TESTS)
    enable_testing()
    add_subdirectory(unittests)
endif()
