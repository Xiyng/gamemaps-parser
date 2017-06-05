#include <stdlib.h>
#include <string.h>
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

bool parse_map_data(
    uint8_t    *data,
    size_t     data_length,
    uint16_t   flag,
    size_t     planes,
    uint32_t   *level_offsets,
    size_t     level_offsets_num,
    map_data_t *map_data
) {
    if (
        data == NULL ||
        data_length < 0 ||
        level_offsets == NULL ||
        level_offsets < 0 ||
        map_data == NULL
    ) {
        return false;
    }

    const char prefix[] = "TED5v1.0";
    const size_t prefix_length = strlen(prefix);
    if (!strncmp(data, prefix, prefix_length)) {
        return false;
    }

    level_t *levels = malloc(level_offsets_num * sizeof(level_t));

    for (size_t level_i = 0; level_i < level_offsets_num; level_i++) {
        const uint32_t level_offset = level_offsets[level_i];
        const uint32_t plane_offsets_offset = 0;
        const uint32_t plane_data_offsets_offset = planes * 4;
        const uint32_t other_data_offset = plane_data_offsets_offset + planes * 2;

        levels[level_i].planes = malloc(planes * sizeof(plane_t));

        for (size_t plane_i = 0; plane_i < planes; plane_i++) {
            uint32_t plane_offset;
            read_little_endian_bytes(
                (uint8_t *)(level_offset + plane_offsets_offset + 4 * plane_i),
                4,
                (uint8_t *)&plane_offset
            );

            uint16_t plane_length;
            read_little_endian_bytes(
                (uint8_t *)(level_offset + plane_data_offsets_offset + 2 * plane_i),
                2,
                (uint8_t *)&plane_length
            );

            uint16_t *decompressed_plane_data;
            size_t decompressed_plane_length;
            rlew_decompress(
                (uint8_t *)(data + plane_offset),
                (size_t)plane_length,
                flag,
                &decompressed_plane_data,
                &decompressed_plane_length
            );

            levels[level_i].planes[plane_i].data = decompressed_plane_data;
            levels[level_i].planes[plane_i].length = decompressed_plane_length;
        }

        levels[level_i].planes_num = planes;

        uint16_t width;
        read_little_endian_bytes(
            (uint8_t *)(level_offset + other_data_offset),
            2,
            (uint8_t *)&(levels[level_i].width)
        );

        uint16_t height;
        read_little_endian_bytes(
            (uint8_t *)(level_offset + other_data_offset),
            4,
            (uint8_t *)&(levels[level_i].height)
        );

        uint8_t name[16];

        strncpy(&(levels[level_i].name), (const char *)(level_offset + other_data_offset + 4), 16);
    }

    map_data->levels = levels;
    map_data->levels_num = level_offsets_num;

    return true;
}

inline void read_little_endian_bytes(uint8_t *data, size_t length, uint8_t* result) {
    for (size_t i = 0; i < length; i++) {
        *(result + i) = data[length - i - 1];
    }
}

bool rlew_decompress(
    uint8_t  *data,
    size_t   data_length,
    uint16_t flag,
    uint16_t **result,
    size_t   *result_length
) {
    if (data == NULL || data_length < 1 || result == NULL || result_length == NULL) {
        return false;
    }

    size_t buffer_length = data_length;
    uint16_t *decompressed = malloc(buffer_length);
    if (decompressed == NULL) {
        return false;
    }
    size_t decompressed_length = 0;

    uint8_t flag_byte_1 = (uint8_t)(flag & (((uint16_t)0xff) << 4) >> 4);
    uint8_t flag_byte_2 = (uint8_t)(flag & ((uint16_t)0xff));

    size_t data_i = 0;
    size_t decompressed_i = 0;
    while (data_i < data_length) {
        if (
            data_i + 2 >= data_length &&
            data[data_i] == flag_byte_1 &&
            data[data_i + 1] == flag_byte_2
        ) {
            uint32_t n;
            read_little_endian_bytes(data + data_i + 2, 4, (uint8_t *)&n);
            if (data_i + n >= data_length) {
                return false;
            }
            uint16_t word;
            read_little_endian_bytes(data + data_i + 6, 2, (uint8_t *)&word);

            if (decompressed_i + n > buffer_length) {
                uint16_t *old = decompressed;

                buffer_length = n < buffer_length ? 2 * buffer_length : buffer_length + n;
                decompressed = malloc(buffer_length);

                free(old);
            }

            for (size_t i = 0; i < n; i++) {
                decompressed_i = decompressed_i + i;
                decompressed[decompressed_i] = word;
            }
            decompressed_length += n;

            data_i += 6 + n;
        }
        else {
            if (data_i + 2 >= data_length) {
                return false;
            }
            if (decompressed_i + 1 > buffer_length) {
                uint16_t *old = decompressed;

                buffer_length *= 2;
                decompressed = malloc(buffer_length);

                free(old);
            }

            uint16_t word;
            read_little_endian_bytes(data + data_i + 6, 2, (uint8_t *)&word);
            decompressed[decompressed_i] = word;
            decompressed_length++;

            data_i += 2;
        }
        decompressed_i++;
    }

    *result = decompressed;
    *result_length = decompressed_length;

    return true;
}
