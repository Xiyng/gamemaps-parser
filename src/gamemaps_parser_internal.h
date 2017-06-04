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

///
/// Decompresses RLEW-compressed data.
///
/// \return `true` if successful, `false` otherwise
///
bool rlew_decompress(
    uint8_t  *data,         ///< [in]  Data to decompress
    size_t   data_length,   ///< [in]  The length of the data to decompress
    uint16_t flag,          ///< [in]  The RLEW flag marking compressed data
    uint16_t **result,      ///< [out] The dynamically allocated, decompressed data
    size_t   *result_length ///< [out] The length of the decompressed data
);

#endif
