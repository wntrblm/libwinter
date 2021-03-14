/*
    Copyright (c) 2021 Alethea Katherine Flowers.
    Published under the standard MIT License.
    Full text available at: https://opensource.org/licenses/MIT
*/

/* Tests for src/wntr_bitbang_spi.c */

#include "gpio_stub.h"
#include "libwinter_test.h"
#include "wntr_bitbang_spi.h"

static uint8_t pull_byte() {
    uint8_t val = 0;
    for (uint8_t bitmask = 0x80; bitmask; bitmask >>= 1) {
        if (gpio_stub_pop_pin_value()) {
            val |= bitmask;
        }
    }
    return val;
}

TEST_CASE_BEGIN(init)
    /* Test with valid pins, makes sure GPIO is set properly. */
    struct WntrBitBangSPI spi = {
        .sdo = (struct WntrGPIOPin){.port = 0, .pin = 1},
        .sdi = (struct WntrGPIOPin){.port = 0, .pin = 2},
        .sck = (struct WntrGPIOPin){.port = 0, .pin = 3},
        .clock_polarity = 0,
    };

    wntr_bitbang_spi_init(&spi, 100000);

    munit_assert_uint32(spi._clock_delay, ==, 5);
    munit_assert(GPIO_PORT_0_PINS[1].direction == true);
    munit_assert(GPIO_PORT_0_PINS[2].direction == false);
    munit_assert(GPIO_PORT_0_PINS[3].direction == true);
    munit_assert(GPIO_PORT_0_PINS[3].value == false);

    /* Test with clock polarity = 1, should set SCK to high. */
    spi.clock_polarity = 1;

    wntr_bitbang_spi_init(&spi, 100000);

    munit_assert(GPIO_PORT_0_PINS[3].value == true);

    /* Test with invalid pins for SDI and SDO, shouldn't error but shouldn't set anything. */
    gpio_stub_reset();
    spi.sdo = (struct WntrGPIOPin){.port = 0, .pin = 0};
    spi.sdi = (struct WntrGPIOPin){.port = 0, .pin = 0};

    wntr_bitbang_spi_init(&spi, 100000);

    munit_assert(GPIO_PORT_0_PINS[0].direction == false);
    munit_assert(GPIO_PORT_0_PINS[0].value == false);
TEST_CASE_END

TEST_CASE_BEGIN(write)
    gpio_stub_reset();

    struct WntrBitBangSPI spi = {
        .sdo = (struct WntrGPIOPin){.port = 0, .pin = 1},
        .sdi = (struct WntrGPIOPin){.port = 0, .pin = 2},
        .sck = (struct WntrGPIOPin){.port = 0, .pin = 3},
        .clock_polarity = 0,
    };

    gpio_stub_monitor_pin(1);

    wntr_bitbang_spi_init(&spi, 100000);

    uint8_t data_out[2] = {0xCA, 0xFE};

    wntr_bitbang_spi_write(&spi, data_out, 2);

    munit_assert_uint8(pull_byte(), ==, 0xCA);
    munit_assert_uint8(pull_byte(), ==, 0xFE);
TEST_CASE_END

static MunitTest test_suite_tests[] = {
    {.name = "init", .test = test_init},
    {.name = "write", .test = test_write},
    {.test = NULL},
};

MunitSuite test_bitbang_spi_suite = {
    .prefix = "bitbang SPI: ",
    .tests = test_suite_tests,
    .iterations = 1,
};
