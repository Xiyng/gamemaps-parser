file(GLOB SOURCES "*.c")
add_library(gamemaps_parser STATIC ${SOURCES})
set_target_properties(gamemaps_parser PROPERTIES PREFIX "")
