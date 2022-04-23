/*
    Copyright (c) 2022 Alethea Katherine Flowers.
    Published under the standard MIT License.
    Full text available at: https://opensource.org/licenses/MIT
*/

/* Tests for src/teeth.c */

#include "libwinter_test.h"
#include "teeth.h"
#include <stdlib.h>
#include <time.h>

TEST_CASE_BEGIN(fixed_length)
    srand(time(NULL));

    size_t size = 512;
    uint8_t source_buf[size];
    uint8_t dest_buf[TEETH_ENCODED_LENGTH(size)];
    uint8_t result_buf[size];

    for (size_t n = 0; n < 100; n++) {
        for (size_t i = 0; i < size; i++) { source_buf[i] = (uint8_t)rand(); }

        teeth_encode(source_buf, size, dest_buf);
        teeth_decode(dest_buf, TEETH_ENCODED_LENGTH(size), result_buf);

        munit_assert_memory_equal(size, source_buf, result_buf);
    }
TEST_CASE_END

TEST_CASE_BEGIN(random_length)
    srand(time(NULL));

    size_t max_size = 512;
    uint8_t source_buf[max_size];
    uint8_t dest_buf[TEETH_ENCODED_LENGTH(max_size)];
    uint8_t result_buf[max_size];

    for (size_t n = 0; n < 100; n++) {
        size_t len = rand() % max_size;
        for (size_t i = 0; i < len; i++) { source_buf[i] = (uint8_t)rand(); }

        teeth_encode(source_buf, len, dest_buf);
        teeth_decode(dest_buf, TEETH_ENCODED_LENGTH(len), result_buf);

        munit_assert_memory_equal(len, source_buf, result_buf);
    }
TEST_CASE_END

TEST_SUITE_BEGIN
    TEST_SUITE_TEST(fixed_length, "random data, fixed length")
    TEST_SUITE_TEST(random_length, "random data, random length")
TEST_SUITE_END(teeth)
