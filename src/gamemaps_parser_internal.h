#ifndef GAMEMAPS_PARSER_INTERNAL_H
#define GAMEMAPS_PARSER_INTERNAL_H

#include <stddef.h>
#include <stdint.h>

///
/// Reads several bytes of little-endian data to a single variable.
///
void read_little_endian_bytes(
    uint8_t *data,  ///< [in]  Source data to read
    size_t length,  ///< [in]  The number of bytes to read from the source data
    uint8_t* result ///< [out] The result
);

#endif
