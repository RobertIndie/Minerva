cmake_minimum_required (VERSION 3.8)

aux_source_directory(. UTIL_SRCS)

add_library(util STATIC ${UTIL_SRCS})