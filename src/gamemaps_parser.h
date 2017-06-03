#ifndef GAMEMAPS_PARSER_H
#define GAMEMAPS_PARSER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

///
/// Parses content of the map header file.
///
/// \return `true` if successful, `false` otherwise
///
bool parse_map_header(
    uint8_t  *header,               ///< [in]  The content of the map header file
    size_t   length,                ///< [in]  The length of the map header file
    uint32_t *level_pointers,       ///< [out] The level pointers array
    size_t   *level_pointers_length ///< [out] The length of the level pointers array
);

#endif
