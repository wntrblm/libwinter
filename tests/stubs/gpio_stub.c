#include "gpio_stub.h"
#include "assert.h"
#include "wntr_gpio.h"
#include <stdio.h>

struct GPIOStubPin GPIO_PORT_0_PINS[10];
static uint8_t monitor_pin_ = 0;
static bool pin_value_list_[1024];
static size_t pin_value_list_head_ = 0;
static size_t pin_value_list_tail_ = 0;

void gpio_stub_reset() {
    for (size_t i = 0; i < 10; i++) {
        GPIO_PORT_0_PINS[i] = (struct GPIOStubPin){
            .direction = false,
            .value = false,
        };
    }

    pin_value_list_head_ = 0;
    pin_value_list_tail_ = 0;
}

void gpio_stub_monitor_pin(uint8_t pin) { monitor_pin_ = pin; }

bool gpio_stub_pop_pin_value() {
    bool val = pin_value_list_[pin_value_list_tail_];
    pin_value_list_tail_++;
    return val;
}

void gpio_stub_push_pin_value(bool val) {
    pin_value_list_[pin_value_list_head_] = val;
    pin_value_list_head_++;
}

void wntr_gpio_set_as_output(uint8_t port, uint8_t pin) {
    assert(port == 0);
    assert(pin < 10);

    GPIO_PORT_0_PINS[pin].direction = true;
}

void wntr_gpio_set_as_input(uint8_t port, uint8_t pin, bool pullup) {
    (void)(pullup);
    assert(port == 0);
    assert(pin < 10);

    GPIO_PORT_0_PINS[pin].direction = false;
}

void wntr_gpio_set(uint8_t port, uint8_t pin, bool value) {
    assert(port == 0);
    assert(pin < 10);

    GPIO_PORT_0_PINS[pin].value = value;

    if (pin == monitor_pin_) {
        gpio_stub_push_pin_value(value);
    }
}

bool wntr_gpio_get(uint8_t port, uint8_t pin) {
    assert(port == 0);
    assert(pin < 10);

    return GPIO_PORT_0_PINS[pin].value;
}

void wntr_gpio_configure_alt(uint8_t port, uint8_t pin, uint8_t alt) {
    (void)(port);
    (void)(pin);
    (void)(alt);
}
