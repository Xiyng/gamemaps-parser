#include <stdio.h>
#include "utils.h"
#include "gamemaps_parser_internal.h"

bool test_utils() {
    return
        test_read_little_endian_bytes() &&
        test_rlew_decompress();
}

bool test_read_little_endian_bytes() {
    printf("Testing function: read_little_endian_bytes\n");
    bool success = true;

    uint8_t data1[] = { 0x0 };
    uint8_t result1;
    read_little_endian_bytes(data1, 1, &result1);
    success = result1 == data1[0];
    printf(success ? "Test %d: PASS\n" : "Test %d: FAIL\n", 1);
    if (!success) {
        return false;
    }

    uint8_t data2[] = { 0x1 };
    uint8_t result2;
    read_little_endian_bytes(data2, 1, &result2);
    success = result2 == data2[0];
    printf(success ? "Test %d: PASS\n" : "Test %d: FAIL\n", 2);
    if (!success) {
        return false;
    }

    uint8_t data3[] = { 0xcd, 0xab };
    uint16_t result3;
    read_little_endian_bytes(data3, 2, &result3);
    success = result3 == 0xabcd;
    printf(success ? "Test %d: PASS\n" : "Test %d: FAIL\n", 3);
    if (!success) {
        return false;
    }

    uint8_t data4[] = { 0x11, 0x22, 0x33, 0x44 };
    uint32_t result4;
    read_little_endian_bytes(data4, 4, &result4);
    success = result4 == 0x44332211;
    printf(success ? "Test %d: PASS\n" : "Test %d: FAIL\n", 4);
    if (!success) {
        return false;
    }

    uint8_t *data5 = malloc(2);
    data5[0] = 0xcd;
    data5[1] = 0xab;
    uint16_t result5;
    read_little_endian_bytes(data5, 2, &result5);
    success = result5 == 0xabcd;
    printf(success ? "Test %d: PASS\n" : "Test %d: FAIL\n", 5);
    if (!success) {
        return false;
    }

    return true;
}

bool test_rlew_decompress() {
    printf("Testing function: rlew_decompress\n");
    bool success;
    uint16_t flag = 0xacbd;
    uint8_t flag_byte_1 = ((uint8_t *)&flag)[0];
    uint8_t flag_byte_2 = ((uint8_t *)&flag)[1];
    gamemaps_parser_error_t err_code;

    uint8_t data1[] = { 0x0 };
    uint16_t *result1;
    size_t result1_length;
    err_code = rlew_decompress(data1, sizeof(data1), flag, &result1, &result1_length);
    success = err_code = gamemaps_parser_err_not_long_enough;
    printf(success ? "Test %d: PASS\n" : "Test %d: FAIL\n", 1);
    if (!success) {
        return false;
    }

    uint8_t data2[] = { 0xab, 0xcd };
    uint16_t *result2;
    size_t result2_length;
    err_code = rlew_decompress(data2, sizeof(data2), flag, &result2, &result2_length);
    success =
        err_code == gamemaps_parser_err_none &&
        result2_length == 1 &&
        result2[0] == 0xcdab;
    printf(success ? "Test %d: PASS\n" : "Test %d: FAIL\n", 2);
    if (!success) {
        return false;
    }
    free(result2);

    uint8_t data3[] = { flag_byte_1, flag_byte_2, 0x01, 0x00, 0x00, 0x00, 0xde, 0xed };
    uint16_t *result3;
    size_t result3_length;
    err_code = rlew_decompress(data3, sizeof(data3), flag, &result3, &result3_length);
    success =
        err_code == gamemaps_parser_err_none &&
        result3_length == 1 &&
        result3[0] == 0xedde;
    printf(success ? "Test %d: PASS\n" : "Test %d: FAIL\n", 3);
    if (!success) {
        return false;
    }
    free(result3);

    uint8_t data4[] = { flag_byte_1, flag_byte_2, 0x02, 0x00, 0x00, 0x00, 0xde, 0xed };
    uint16_t *result4;
    size_t result4_length;
    err_code = rlew_decompress(data4, sizeof(data4), flag, &result4, &result4_length);
    success =
        err_code == gamemaps_parser_err_none &&
        result4_length == 2 &&
        result4[0] == 0xedde &&
        result4[1] == 0xedde;
    printf(success ? "Test %d: PASS\n" : "Test %d: FAIL\n", 4);
    if (!success) {
        return false;
    }
    free(result4);

    uint8_t data5[] = { 0xab, 0xcd, flag_byte_1, flag_byte_2, 0x02, 0x00, 0x00, 0x00, 0xde, 0xed, 0xfe, 0xef };
    uint16_t *result5;
    size_t result5_length;
    err_code = rlew_decompress(data5, sizeof(data5), flag, &result5, &result5_length);
    success =
        err_code == gamemaps_parser_err_none &&
        result5_length == 4 &&
        result5[0] == 0xcdab &&
        result5[1] == 0xedde &&
        result5[2] == 0xedde &&
        result5[3] == 0xeffe;
    printf(success ? "Test %d: PASS\n" : "Test %d: FAIL\n", 5);
    if (!success) {
        return false;
    }
    free(result5);

    uint8_t data6[] = { 0x63, 0xf6, 0xab, 0xcd, flag_byte_1, flag_byte_2, 0x02, 0x00, 0x00, 0x00, 0xde, 0xed, 0xfe, 0xef };
    uint16_t *result6;
    size_t result6_length;
    err_code = rlew_decompress(data6, sizeof(data6), flag, &result6, &result6_length);
    success =
        err_code == gamemaps_parser_err_none &&
        result6_length == 5 &&
        result6[0] == 0xf663 &&
        result6[1] == 0xcdab &&
        result6[2] == 0xedde &&
        result6[3] == 0xedde &&
        result6[4] == 0xeffe;
    printf(success ? "Test %d: PASS\n" : "Test %d: FAIL\n", 6);
    if (!success) {
        return false;
    }
    free(result6);

    return true;
}
