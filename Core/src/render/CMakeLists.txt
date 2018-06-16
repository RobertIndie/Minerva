cmake_minimum_required (VERSION 3.8)

aux_source_directory(. RENDER_SRCS)

add_library(render STATIC ${RENDER_SRCS})