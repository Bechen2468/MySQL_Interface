cmake_minimum_required(VERSION 3.15)
project(mysql-interface-unittests LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Failsave for if docker is not installed
find_program(DOCKER_EXECUTABLE docker)
if(NOT DOCKER_EXECUTABLE)
    message(WARNING "Docker not found; MySQL integration tests disabled.")
    return()
endif()

find_package(GTest CONFIG REQUIRED)


add_executable(${PROJECT_NAME} main.cpp)

# Set my include dir
target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    GTest::gtest 
    mysql-interface
)



# --- GTest integration ---
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})



# --- Custom target to actually RUN the tests ---
add_custom_target(run_tests
    COMMAND $<TARGET_FILE:mysql-interface-unittests>
    DEPENDS mysql-interface-unittests
    COMMENT "Running integration test suite with MySQL Docker"
)

message(STATUS "CMAKE_MSVC_RUNTIME_LIBRARY=${CMAKE_MSVC_RUNTIME_LIBRARY}")
message(STATUS "CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")

