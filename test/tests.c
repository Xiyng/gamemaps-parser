#include <stdbool.h>
#include <stdio.h>
#include "utils.h"

int main(int argc, char **argv) {
    bool success = true;
    success = success && test_utils();
    printf(success ? "TESTS PASSED\n" : "TESTS  F A I L E D\n");
}
