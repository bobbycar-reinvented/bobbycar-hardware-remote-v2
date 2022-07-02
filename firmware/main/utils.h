#pragma once

// system includes
#include <cstdint>

struct MotorPwms {
    int16_t frontLeft;
    int16_t frontRight;
    int16_t backLeft;
    int16_t backRight;
};

namespace utils {
float mapAnalogStick(uint16_t middle, uint16_t start, uint16_t end, uint16_t raw);

float x_coord();
float y_coord();
MotorPwms calculatePwm();
} // namespace utils
