#ifndef GAMEMAPS_PARSER_H
#define GAMEMAPS_PARSER_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
    gamemaps_parser_err_none                = 0,
    gamemaps_parser_err_unknown             = 1,
    gamemaps_parser_err_null                = 2,
    gamemaps_parser_err_negative_length     = 3,
    gamemaps_parser_err_not_long_enough     = 4,
    gamemaps_parser_err_identifier_mismatch = 5,
    gamemaps_parser_err_allocation_failed   = 6
} gamemaps_parser_error_t;

///
/// A structure representing a plane
///
typedef struct {
    uint16_t *data;  ///< Plane data
    size_t   length; ///< The length of the plane data
} plane_t;

///
/// A structure representing a level
///
typedef struct {
    plane_t  *planes;    ///< Planes
    size_t   planes_num; ///< The number of planes
    uint16_t width;      ///< The width of the level
    uint16_t height;     ///< The height of the level
    uint8_t  name[16];   ///< The name of the level
} level_t;

///
/// A structure representing map data
///
typedef struct {
    level_t *levels;    ///< Levels
    size_t  levels_num; ///< The number of levels
} map_data_t;

///
/// Parses content of the map header file.
///
/// \return An error code representing either success or some specific error
///
gamemaps_parser_error_t parse_map_header(
    uint8_t  *header,                ///< [in]  The content of the map header file
    size_t   length,                 ///< [in]  The length of the map header file
    uint32_t **level_pointers,       ///< [out] The dynamically allocated level pointers array
    size_t   *level_pointers_length  ///< [out] The length of the level pointers array
);

///
/// Parses content of the map data file.
///
/// \return An error code representing either success or some specific error
///
gamemaps_parser_error_t parse_map_data(
    uint8_t    *data,             ///< [in]  Raw map data
    size_t     data_length,       ///< [in]  The length of the raw map data
    uint16_t   flag,              ///< [in]  The RLEW flag marking compressed data
    size_t     planes,            ///< [in]  The number of planes per level
    uint32_t   *level_offsets,    ///< [in]  Level header offsets
    size_t     level_offsets_num, ///< [in]  The number of level header offsets
    map_data_t *map_data          ///< [out] Parsed map data
);

#endif
