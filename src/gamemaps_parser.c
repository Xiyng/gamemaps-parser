#include "gamemaps_parser.h"
#include "gamemaps_parser_internal.h"

bool parse_map_header(
    uint8_t *header,
    size_t length,
    uint32_t *level_pointers,
    size_t *level_pointers_length
) {
    const size_t min_length = 402;
    if (length < min_length) {
        return false;
    }

    uint16_t start;
    read_little_endian_bytes(header, 2, (uint8_t *)(&start));
    if (start != 0xabcd) {
        return false;
    }

    *level_pointers_length = 100;
    for (size_t i = 0; i < *level_pointers_length; i++) {
        size_t offset = 4 * i + 2;
        uint32_t *target = &level_pointers[i];
        read_little_endian_bytes(header + offset, 4, (uint8_t *)target);
    }

    return true;
}

inline void read_little_endian_bytes(uint8_t *data, size_t length, uint8_t* result) {
    for (size_t i = 0; i < length; i++) {
        *(result + i) = data[length - i - 1];
    }
}
