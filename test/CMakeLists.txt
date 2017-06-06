set(GAME_DATA_LOCATION "\"${CMAKE_CURRENT_SOURCE_DIR}/game_data\"")

set(GENERATED_FILE_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/generated")
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/game_data.h.in"
    "${GENERATED_FILE_LOCATION}/game_data.h"
)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${GENERATED_FILE_LOCATION})
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
add_executable(run_tests ${SOURCES})
target_link_libraries(run_tests gamemaps_parser)
