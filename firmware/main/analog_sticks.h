#pragma once

// system includes
#include <cstdint>
#include <optional>

// local includes
#include "globals.h"

namespace analog_sticks {
typedef struct {
    std::optional<int16_t> raw_x, raw_y;
    std::optional<float> x, y;
    std::optional<bool> btn_pressed;
} analog_stick_t;

extern analog_stick_t left_stick;
extern analog_stick_t right_stick;

void init();
void update();
void read_buttons();

bool buttons_pressed();
bool both_buttons_pressed();
bool needs_calibration();
bool configs_are_valid();
bool has_raw_values(analog_stick_t stick);
bool has_values(analog_stick_t stick);
} // namespace analog_sticks
