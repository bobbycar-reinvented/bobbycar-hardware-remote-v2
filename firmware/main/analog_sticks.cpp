constexpr const char * const TAG = "ANALOG_STICKS";
#include "analog_sticks.h"

// esp-idf includes
#include <esp32-hal-adc.h>
#include <esp32-hal-gpio.h>
#include <esp_log.h>

// local includes
#include "bobbybuttons.h"
#include "pins.h"
#include "screenmanager.h"
#include "settings.h"

namespace analog_sticks {

analog_stick_t left_stick;
analog_stick_t right_stick;

namespace {
bool last_left_state;
bool last_right_state;
} // namespace

void init()
{
    left_stick = analog_stick_t{
        .raw_x = std::nullopt,
        .raw_y = std::nullopt,
        .x = std::nullopt,
        .y = std::nullopt,
        .btn_pressed = std::nullopt
    };

    right_stick = analog_stick_t{
        .raw_x = std::nullopt,
        .raw_y = std::nullopt,
        .x = std::nullopt,
        .y = std::nullopt,
        .btn_pressed = std::nullopt
    };

    pinMode(pins::left_x,   INPUT);
    pinMode(pins::left_y,   INPUT);
    pinMode(pins::left_btn, INPUT_PULLUP);

    pinMode(pins::right_x,   INPUT);
    pinMode(pins::right_y,   INPUT);
    pinMode(pins::right_btn, INPUT_PULLUP);

    last_left_state = false;
    last_right_state = false;

    update();
}

void update()
{
    [[maybe_unused]]
    constexpr auto sampleMultipleTimes = [](uint8_t pin){
        analogRead(pin);
        float sum{};
        const auto sampleCount = configs.sampleCount.value();
        for (uint8_t i = 0; i < sampleCount; i++)
            sum += analogRead(pin);
        return sum / sampleCount;
    };

    left_stick = analog_stick_t{
            .raw_x = std::nullopt,
            .raw_y = std::nullopt,
            .btn_pressed = std::nullopt
    };

    right_stick = analog_stick_t{
            .raw_x = std::nullopt,
            .raw_y = std::nullopt,
            .btn_pressed = std::nullopt
    };

    if (!left_stick.raw_x)
        left_stick.raw_x = sampleMultipleTimes(pins::left_x);
    if (!left_stick.raw_y)
        left_stick.raw_y = sampleMultipleTimes(pins::left_y);
    if (!left_stick.btn_pressed)
        left_stick.btn_pressed = !digitalRead(pins::left_btn);

    if (!right_stick.raw_x)
        right_stick.raw_x = sampleMultipleTimes(pins::right_x);
    if (!right_stick.raw_y)
        right_stick.raw_y = sampleMultipleTimes(pins::right_y);
    if (!right_stick.btn_pressed)
        right_stick.btn_pressed = !digitalRead(pins::right_btn);

    if (left_stick.btn_pressed && *left_stick.btn_pressed != last_left_state)
    {
        last_left_state = *left_stick.btn_pressed;

        if (espgui::currentDisplay)
        {
            if (last_left_state)
                espgui::currentDisplay->buttonPressed(espgui::Button(BobbyButton::LeftStick));
            else
                espgui::currentDisplay->buttonReleased(espgui::Button(BobbyButton::LeftStick));
        }
    }

    if (right_stick.btn_pressed && *right_stick.btn_pressed != last_right_state)
    {
        last_right_state = *right_stick.btn_pressed;

        if (espgui::currentDisplay)
        {
            if (last_right_state)
                espgui::currentDisplay->buttonPressed(espgui::Button(BobbyButton::RightStick));
            else
                espgui::currentDisplay->buttonReleased(espgui::Button(BobbyButton::RightStick));
        }
    }

    // apply calibration
    if (!needs_calibration())
    {
        // calibration: left_x_min, left_x_max, left_x_middle, left_y_min, left_y_max, left_y_middle
        // calibration: right_x_min, right_x_max, right_x_middle, right_y_min, right_y_max, right_y_middle
        // configs.deadband => apply to both sticks, if value is in deadband, set to 0, otherwise apply range - deadband
        // map those to x => -1000 to 1000 and y => -1000 to 1000

        // ToDo: actually write the code
    }
}

bool needs_calibration()
{
    return !(
      // left stick
      configs.leftStickXStartCal.touched() &&
      configs.leftStickXMiddleCal.touched() &&
      configs.leftStickXEndCal.touched() &&
      configs.leftStickYStartCal.touched() &&
      configs.leftStickYMiddleCal.touched() &&
      configs.leftStickYEndCal.touched() &&
      // right stick
      configs.rightStickXStartCal.touched() &&
      configs.rightStickXMiddleCal.touched() &&
      configs.rightStickXEndCal.touched() &&
      configs.rightStickYStartCal.touched() &&
      configs.rightStickYMiddleCal.touched() &&
      configs.rightStickYEndCal.touched()
    );
}

bool has_raw_values(analog_stick_t stick)
{
    return (
      stick.raw_x &&
      stick.raw_y &&
      stick.btn_pressed
    );
}

bool has_values(analog_stick_t stick)
{
    return (
        stick.x &&
        stick.y &&
        stick.btn_pressed
    );
}
} // namespace analog_sticks
