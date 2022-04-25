# Copyright (C) Normal Fish Studios - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Dennis Law <normalfish.master@gmail.com>, April 2022

cmake_minimum_required (VERSION 3.8)

set(TARGET SquareConnectGenerator)

# Add source to this project's executable.
add_executable (${TARGET}
    "Board.cpp"
    "Filter.cpp"
    "Generator.cpp"
    "Main.cpp"
    "Movement.cpp"
    "Output.cpp"
    "Pos.cpp"
    "Profiler.cpp"
    
    "Solver.cpp"
    "Square.cpp"
    "Util.cpp"
)

# Set include directories
target_include_directories(${TARGET}
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/User"
)
