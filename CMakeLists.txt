cmake_minimum_required(VERSION 3.10)

project(Measurement-Exercise)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# For GoogleTest, which requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set up GTest v1.14.0
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/f8d7d77c06936315286eb55f8de22cd23c188571.zip
)
FetchContent_MakeAvailable(googletest)
enable_testing()

# Setup Variables
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
if (OUTPUT_NAME_BASE)
  set(APP_NAME ${OUTPUT_NAME_BASE})
  set(LIB_NAME ${OUTPUT_NAME_BASE})
  set(APP_NAME_TEST ${OUTPUT_NAME_BASE}-Test)
else()
  set(APP_NAME Measurement)
  set(LIB_NAME Measurement)
  set(APP_NAME_TEST Measurement-Test)
endif()

SET(BASEPATH ${CMAKE_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${BASEPATH})

# Handle custom compiler flags
if(LOGGING)
  add_compile_definitions(LOGGING)
endif()

# Add Subfolders
add_subdirectory(${CMAKE_SOURCE_DIR}/src)