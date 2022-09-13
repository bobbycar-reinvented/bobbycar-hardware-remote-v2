#pragma once

// system includes
#include <cstdint>

namespace gamecontroller {
void init();
void update();
void press(uint16_t button);
void release(uint16_t button);
} // namespace gamecontroller
