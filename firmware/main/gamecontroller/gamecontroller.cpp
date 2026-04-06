#include "gamecontroller.h"

// esp-idf includes
#include <esp32-hal-gpio.h>

// 3rdparty lib includes
#include <BleGamepad.h>
#include <screenmanager.h>
#include <tftcolors.h>
#include <tftinterface.h>

// local includes
#include "analog_sticks.h"
#include "dualboot.h"
#include "fontrenderer.h"
#include "settings.h"

namespace gamecontroller {
namespace {
BleGamepad bleGamepad{
    std::format("{}-controller", defaultHostname()),
    "bobbycar-graz",
    100
};
uint16_t log_y = 0;
} // namespace

constexpr auto font = 2;

void reset_log(espgui::TftInterface &tft)
{
    log_y = (espgui::FontRenderer{tft}.fontHeight(font) + 2) * 2;
    tft.fillRect(0, log_y, tft.width(), tft.height() - log_y, espgui::TFT_BLACK);
}

void print_log(std::string_view text)
{

    if (configs.gamecontrollerBacklight.value())
    {
        return;
    }

    espgui::changeScreenCallback = [&](espgui::TftInterface &tft) {
        espgui::FontRenderer font_renderer{tft};

        log_y += font_renderer.fontHeight(font) + 2;
        if (log_y > tft.height() - font_renderer.fontHeight(font) - 2)
        {
            reset_log(tft);
        }

        font_renderer.drawString(text, 0, log_y, espgui::TFT_WHITE, espgui::TFT_BLACK, font);
    };
}

void init()
{
    if (!boot_gamecontroller)
    {
        return;
    }
    int32_t next_text_x{0};

    espgui::changeScreenCallback = [&](espgui::TftInterface &tft) {
        espgui::FontRenderer font_renderer{tft};

        tft.fillScreen(espgui::TFT_BLACK);
        // tft.setTextColor(espgui::TFT_WHITE, espgui::TFT_BLACK);
        // tft.setTextFont(2);
        next_text_x += font_renderer.drawString("Initializing gamecontroller...", 0, 0, espgui::TFT_WHITE, espgui::TFT_BLACK, font);

        BleGamepadConfiguration bleGamepadConfiguration;
        bleGamepadConfiguration.setButtonCount(10);
        bleGamepadConfiguration.setAxesMin(-100);
        bleGamepadConfiguration.setAxesMax(100);
        bleGamepadConfiguration.setWhichAxes(true, true, false, true, true, false, false, false);

        bleGamepad.begin(&bleGamepadConfiguration);
        font_renderer.drawString("Done", next_text_x, 0, espgui::TFT_WHITE, espgui::TFT_BLACK, font);
        reset_log(tft);
    };

    digitalWrite(PIN_LED_BACKLIGHT, configs.gamecontrollerBacklight.value());
}

void update()
{
    static bool first_connected = true;
    if (!boot_gamecontroller)
    {
        return;
    }

    espgui::changeScreenCallback = [&](espgui::TftInterface &tft) {
        espgui::FontRenderer font_renderer{tft};

        const auto y = font_renderer.fontHeight(font) + 2;

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
            font_renderer.drawString("Connected!", 0, y, espgui::TFT_WHITE, espgui::TFT_BLACK, font);
            bleGamepad.sendReport();
        }
    };

    if (analog_sticks::has_values(analog_sticks::left_stick) && analog_sticks::has_values(analog_sticks::right_stick) && analog_sticks::configs_are_valid())
    {
        using namespace analog_sticks;
        const int16_t lx = left_stick.x ? left_stick.x.value() : 0;
        const int16_t ly = left_stick.y ? left_stick.y.value() * -1 : 0;
        const int16_t rx = right_stick.x ? right_stick.x.value() : 0;
        const int16_t ry = right_stick.y ? right_stick.y.value() * -1 : 0;

        bleGamepad.setAxes(lx, ly, 0, 0, rx, ry, 0, 0);
    }
}

void press(uint16_t button)
{
    bleGamepad.press(button+1);
    print_log(std::format("pressed button {}", button));
    bleGamepad.sendReport();
}

void release(uint16_t button)
{
    bleGamepad.release(button+1);
    print_log(std::format("released button {}", button));
    bleGamepad.sendReport();
}
} // namespace gamecontroller
