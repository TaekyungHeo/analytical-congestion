# CMake requirement
cmake_minimum_required(VERSION 3.15)

# C++ requirement
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler requirement
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.3)
        message(FATAL_ERROR "g++ (GNU) version should be greater than 5.3, but found ${CMAKE_CXX_COMPILER_VERSION}")
    endif()
endif()

# Setup project
project (Congestion)

# Package requirement
#find_package(Boost 1.40 REQUIRED COMPONENTS program_options)

# Include src files to compile
file(GLOB_RECURSE srcs
        "${PROJECT_SOURCE_DIR}/*.hh"
        "${PROJECT_SOURCE_DIR}/*.cc"
        )

# Compile sources
add_executable(Congestion ${srcs})

# Link libraries
#target_link_libraries(AnalyticalAstra LINK_PUBLIC AstraSim)
#target_link_libraries(AnalyticalAstra LINK_PRIVATE Boost::program_options)

# Resulting binary location settings
set_target_properties(Congestion
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "bin/"
        LIBRARY_OUTPUT_DIRECTORY "lib/"
        ARCHIVE_OUTPUT_DIRECTORY "lib/"
        )
