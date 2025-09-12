cmake_minimum_required(VERSION 3.22)
project(b64tool LANGUAGES CXX)

# Set the C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# === Base64 Library ===
# This creates our reusable 'base64_lib'
add_library(base64_lib src/base64.cpp)

# Make the 'src' directory available for #include "base64.hpp"
target_include_directories(base64_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# === Main Executable ===
# This creates the 'b64tool' command-line application
add_executable(b64tool src/main.cpp)

# Link our application against our library
target_link_libraries(b64tool PRIVATE base64_lib)

# === Testing ===
# Enable the CTest testing framework
enable_testing()

# Fetch the Catch2 testing library from GitHub
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.2
)
FetchContent_MakeAvailable(Catch2)

# Create the test executable
add_executable(b64_tests test/tests.cpp)

# Link the test executable against our library and Catch2
target_link_libraries(b64_tests PRIVATE base64_lib Catch2::Catch2WithMain)

# Add the test to CTest so it can be run automatically
add_test(NAME Base64Tests COMMAND b64_tests)