#pragma once

// system includes
#include <cstdint>

namespace utils {
float mapAnalogStick(uint16_t middle, uint16_t start, uint16_t end, uint16_t raw);
} // namespace utils
