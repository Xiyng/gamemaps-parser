cmake_minimum_required (VERSION 3.5)
project (gamemaps_parser)

set(CMAKE_C_STANDARD 99)
add_subdirectory(src)
add_subdirectory(test)

enable_testing()
add_test(NAME MyTest COMMAND Test)
