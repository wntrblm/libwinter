#pragma once

#include <stdbool.h>
#include <stdint.h>

struct GPIOStubPin {
    bool direction;
    bool value;
};

extern struct GPIOStubPin GPIO_PORT_0_PINS[10];

void gpio_stub_reset();
void gpio_stub_monitor_pin(uint8_t pin);
bool gpio_stub_pop_pin_value();
