#pragma once

// system includes
#include <cstdint>

// 3rdparty lib includes
#include <tftinterface.h>

namespace gamecontroller {
void init();
void update();
void press(uint16_t button);
void release(uint16_t button);
} // namespace gamecontroller
