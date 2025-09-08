cmake_minimum_required(VERSION 4.0)

project(SysML-v2-API-MCP-Server VERSION 1.0 LANGUAGES CXX)

set(TEST_NAME SysMLv2MCPServerTest)
set(APP_NAME SysMLv2MCPServer)

# Definition of the C++ language standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# === Dependencies ===

include(FetchContent)

# Get nlohmann::json (JSON library for modern C++)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)
include_directories(${CMAKE_SOURCE_DIR}/build/_deps/json-src/include)

# Get Catch2 unit test framework
FetchContent_Declare(catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git GIT_TAG v3.7.0)
FetchContent_MakeAvailable(catch2)

# Get latest argparse
FetchContent_Declare(argparse GIT_REPOSITORY https://github.com/p-ranav/argparse.git)
FetchContent_MakeAvailable(argparse)
include_directories(${CMAKE_SOURCE_DIR}/build/_deps/argparse-src/include)

# Get spdlog C++ logging framework
# FetchContent_Declare(spdlog GIT_REPOSITORY https://github.com/gabime/spdlog.git GIT_TAG v1.15.3)
# include_directories(${CMAKE_SOURCE_DIR}/build/_deps/spdlog-src/include)

# === Target: Unit Test ===

add_executable(${TEST_NAME}
    srctest/testsuite.cpp
    src/commandlineargumentparser.cpp
    src/mcpserver.cpp
)
target_link_libraries(${TEST_NAME} PRIVATE argparse Catch2::Catch2WithMain)

enable_testing()
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

# === Target: MCPServer Application ===

add_executable(${APP_NAME}
    src/main.cpp
    src/commandlineargumentparser.cpp
    src/mcpserver.cpp
)
target_link_libraries(${APP_NAME} PRIVATE argparse)

# === Compiler options ===

if(MSVC)
  target_compile_options(${TEST_NAME} PRIVATE /W4 /WX)
  target_compile_options(${APP_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${TEST_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
  target_compile_options(${APP_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()