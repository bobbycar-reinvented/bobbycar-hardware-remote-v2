#pragma once

namespace analog_sticks::pins {
constexpr auto left_x = PIN_ANALOG_LEFT_X;
constexpr auto left_y = PIN_ANALOG_LEFT_Y;
constexpr auto left_btn = PIN_ANALOG_LEFT_BTN;

constexpr auto right_x = PIN_ANALOG_RIGHT_X;
constexpr auto right_y = PIN_ANALOG_RIGHT_Y;
constexpr auto right_btn = PIN_ANALOG_RIGHT_BTN;
} // namespace analog_sticks::pins

namespace buttons::pins {
constexpr auto OUT1 = PIN_BTN_OUT1;
constexpr auto OUT2 = PIN_BTN_OUT2;
constexpr auto IN1 = PIN_BTN_IN1;
constexpr auto IN2 = PIN_BTN_IN2;
} // namespace buttons::pins
