#pragma once

#include <stdint.h>

extern uint32_t SystemCoreClock;
inline static void SysTick_Config(uint32_t unused) { (void)unused; };
