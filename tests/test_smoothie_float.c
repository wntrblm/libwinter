/*
    Copyright (c) 2022 Alethea Katherine Flowers.
    Published under the standard MIT License.
    Full text available at: https://opensource.org/licenses/MIT
*/

/* Tests for src/wntr_smoothie_float.h */

#include "libwinter_test.h"
#include "wntr_smoothie_float.h"

TEST_CASE_BEGIN(smoothie_low_pass_only)
    struct WntrSmoothie smooth = {
        .cutoff = 1.0f,
        .sensitivity = 0.0f,
    };

    WntrSmoothie_init(&smooth, 0.0f);

    /* Moving slowly should let everything through */
    munit_assert_double(WntrSmoothie_step(&smooth, 100.0f, 0.0f), ==, 0.0f);
    munit_assert_double(WntrSmoothie_step(&smooth, 100.0f, 100.0f), >, 99.0f);
    munit_assert_double(WntrSmoothie_step(&smooth, 100.0f, 0.0f), <, 1.0f);

    /* Moving quickly should demonstrate exponential filtering */
    munit_assert_double(WntrSmoothie_step(&smooth, 1.0f, 100.0f), >, 50.0f);
    munit_assert_double(WntrSmoothie_step(&smooth, 1.0f, 100.0f), >, 75.0f);
    munit_assert_double(WntrSmoothie_step(&smooth, 1.0f, 100.0f), >, 90.0f);
TEST_CASE_END

TEST_CASE_BEGIN(smoothie_dynamic_filtering)
    struct WntrSmoothie smooth = {
        .cutoff = 1.0f,
        .sensitivity = 0.01f,
    };

    WntrSmoothie_init(&smooth, 0.0f);

    /* Moving slowly should let everything through, just like the low pass */
    munit_assert_double(WntrSmoothie_step(&smooth, 100.0f, 0.0f), ==, 0.0f);
    munit_assert_double(WntrSmoothie_step(&smooth, 100.0f, 100.0f), >, 99.0f);
    munit_assert_double(WntrSmoothie_step(&smooth, 100.0f, 0.0f), <, 1.0f);

    /* Moving a large distance quickly should raise the cut-off frequency and
        allow more through than the low pass alone. */
    munit_assert_double(WntrSmoothie_step(&smooth, 1.0f, 100.0f), >, 70.0f);
    munit_assert_double(WntrSmoothie_step(&smooth, 1.0f, 1000.0f), >, 900.0f);

    /* Moving a small distance quickly should get filtered out, since the
        rate of change isn't hight enough to trigger the dynamic behavior. */
    WntrSmoothie_init(&smooth, 0.0f);

    munit_assert_double(WntrSmoothie_step(&smooth, 0.1f, 0.1f), <, 0.02f);
    munit_assert_double(WntrSmoothie_step(&smooth, 0.1f, -0.1f), >, -0.02f);
    munit_assert_double(WntrSmoothie_step(&smooth, 0.1f, -0.05f), >, -0.03f);
TEST_CASE_END

static MunitTest test_suite_tests[] = {
    {.name = "low pass only", .test = test_smoothie_low_pass_only},
    {.name = "dynamic filtering", .test = test_smoothie_dynamic_filtering},
    {.test = NULL},
};

MunitSuite test_smoothie_float_suite = {
    .prefix = "smoothie (float): ",
    .tests = test_suite_tests,
    .iterations = 1,
};
