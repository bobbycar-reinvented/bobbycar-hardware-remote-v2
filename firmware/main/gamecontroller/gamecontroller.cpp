#include "gamecontroller.h"

// 3rdparty lib includes
#include <BleGamepad.h>
#include <fmt/core.h>
#include <tftinstance.h>

// local includes
#include "analog_sticks.h"
#include "settings.h"
#include "dualboot.h"

namespace gamecontroller {
namespace {
BleGamepad bleGamepad{
    fmt::format("{}-controller", defaultHostname()),
    "bobbycar-graz",
    100
};
} // namespace

void init()
{
    using namespace espgui;
    if (!boot_gamecontroller)
    {
        return;
    }
    int32_t next_text_x{0};

    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextFont(2);
    next_text_x += tft.drawString("Initializing gamecontroller...", 0, 0);

    BleGamepadConfiguration bleGamepadConfiguration;
    bleGamepadConfiguration.setButtonCount(10);
    bleGamepadConfiguration.setAxesMin(-100);
    bleGamepadConfiguration.setAxesMax(100);
    bleGamepadConfiguration.setWhichAxes(true, true, false, true, true, false, false, false);

    bleGamepad.begin(&bleGamepadConfiguration);
    tft.drawString("Done", next_text_x, 0);
}

void update()
{
    const auto y = espgui::tft.fontHeight() + 2;
    static bool first_connected = true;
    if (!boot_gamecontroller)
    {
        return;
    }

    if (!bleGamepad.isConnected())
    {
        if (!first_connected)
        {
            using namespace espgui;
            tft.fillRect(0, y, 100, y*2, TFT_BLACK);
            first_connected = true;
        }
        return;
    }
    else if (first_connected)
    {
        using namespace espgui;
        first_connected = false;
        tft.drawString("Connected!", 0, y);
        bleGamepad.sendReport();
    }

    if (analog_sticks::has_values(analog_sticks::left_stick) && analog_sticks::has_values(analog_sticks::right_stick) && analog_sticks::configs_are_valid())
    {
        using namespace analog_sticks;
        const int16_t lx = left_stick.x ? left_stick.x.value() : 0;
        const int16_t ly = left_stick.y ? left_stick.y.value() : 0;
        const int16_t rx = right_stick.x ? right_stick.x.value() : 0;
        const int16_t ry = right_stick.y ? right_stick.y.value() : 0;

        bleGamepad.setAxes(lx, ly, 0, 0, rx, ry, 0, 0);
    }
}

void press(uint16_t button)
{
    bleGamepad.press(button+1);
    bleGamepad.sendReport();
}

void release(uint16_t button)
{
    bleGamepad.release(button+1);
    bleGamepad.sendReport();
}
} // namespace gamecontroller
