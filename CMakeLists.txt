# Copyright (C) Normal Fish Studios - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Dennis Law <normalfish.master@gmail.com>, April 2022

cmake_minimum_required (VERSION 3.8)

project ("SquareConnectGenerator")

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add NDEBUG flag to disable asserts for release
add_compile_definitions(
    $<$<CONFIG:Release>:NDEBUG>
)

# Include sub-projects
add_subdirectory("SquareConnectGenerator")
