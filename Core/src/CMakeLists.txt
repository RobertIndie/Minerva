﻿# CMakeList.txt : CMake project for Core, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

aux_source_directory(. SRCS)

add_subdirectory ("./render")
add_subdirectory ("./util")

# Add source to this project's executable.
add_executable (Minerva ${SRCS})

# TODO: Add tests and install targets if needed.
target_link_libraries(Minerva ${LIBS})
target_link_libraries(Minerva render)
target_link_libraries(Minerva util)