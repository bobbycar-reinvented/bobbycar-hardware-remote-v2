#pragma once

// local includes
#include "dpadimpl.h"
#include "pins.h"

namespace buttons {

constexpr const std::array<pin_t, 2> OUT_PINS = {
    pins::OUT1,
    pins::OUT2
};

constexpr const std::array<pin_t, 2> IN_PINS = {
    pins::IN1,
    pins::IN2
};

extern Dpad<IN_PINS.size(), OUT_PINS.size(), IN_PINS, OUT_PINS> dpad;

void init();
void update();
} // namespace buttons
