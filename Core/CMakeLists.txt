# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("Core")

include_directories("./include")

link_directories("./lib")

list(APPEND LIBS "glfw3")
list(APPEND LIBS "opengl32")

# Include sub-projects.
add_subdirectory ("./src")
