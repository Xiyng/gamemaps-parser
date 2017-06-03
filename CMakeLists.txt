cmake_minimum_required (VERSION 3.6)
project (gamemaps_parser)

set(CMAKE_C_STANDARD 99)

file(GLOB SOURCES "src/*.c")

add_library(gamemaps_parser STATIC ${SOURCES})
set_target_properties(gamemaps_parser PROPERTIES PREFIX "")
