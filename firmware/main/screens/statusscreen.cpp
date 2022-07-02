#include "statusscreen.h"

// 3rdparty lib includes
#include <cpputils.h>
#include <fmt/core.h>
#include <screenmanager.h>
#include <tftinstance.h>

// local lib includes
#include "analog_sticks.h"
#include "ble.h"
#include "globals.h"
#include "icons/alert.h"
#include "menus/mainmenu.h"

using namespace analog_sticks;

namespace {
constexpr auto static_radius = 30;
constexpr auto distance = 30;

auto left_x_mid   = distance + static_radius;
auto left_x_start = left_x_mid - static_radius;
auto left_x_end   = left_x_mid + static_radius;

// now the same for right analog stick, but subtract from screen width
auto right_x_mid = espgui::tft.height() - distance - static_radius;
auto right_x_start = right_x_mid - static_radius;
auto right_x_end = right_x_mid + static_radius;

auto y_mid = espgui::tft.width() - distance - static_radius;
auto y_start = y_mid - static_radius;
auto y_end = y_mid + static_radius;

constexpr char TEXT_NOT_CONNECTED[] = "Not connected!";
} // namespace

void StatusScreen::initScreen()
{
    Base::initScreen();

    espgui::tft.setTextColor(TFT_WHITE, TFT_BLACK);

    m_last_left_x = 0;
    m_last_left_y = 0;
    m_last_right_x = 0;
    m_last_right_y = 0;

    m_left_label.start();
    m_right_label.start();
    m_stats_label.start();
}

void StatusScreen::update()
{
    Base::update();

    if (!has_values(left_stick) || !has_values(right_stick))
        return;

    m_left_x = cpputils::mapValueClamped<float>(*left_stick.x, -100, 100, left_x_start, left_x_end);
    m_left_y = cpputils::mapValueClamped<float>(*left_stick.y * -1, -100, 100, y_start, y_end);

    m_right_x = cpputils::mapValueClamped<float>(*right_stick.x, -100, 100, right_x_start, right_x_end);
    m_right_y = cpputils::mapValueClamped<float>(*right_stick.y * -1, -100, 100, y_start, y_end);

    m_left_pressed = *left_stick.btn_pressed;
    m_right_pressed = *right_stick.btn_pressed;
}

void StatusScreen::redraw()
{
    static bool clear_needed = true;

    using namespace espgui;

    Base::redraw();

    if (!ble::isConnected())
    {
        if (clear_needed)
        {
            clear_needed = false;
            m_left_label.clear();
            m_right_label.clear();

            tft.fillScreen(TFT_BLACK);

            tft.setTextColor(TFT_RED);
            const auto fontOffset = tft.textWidth(TEXT_NOT_CONNECTED);
            tft.drawString(TEXT_NOT_CONNECTED, tft.width() / 2 - 10 - (fontOffset/2), (tft.height()-tft.fontHeight()) / 2);

            tft.setSwapBytes(true);
            tft.pushImage(tft.width() / 2 + (fontOffset/2), (tft.height()-bobbyicons::alert.HEIGHT) / 2, bobbyicons::alert.WIDTH, bobbyicons::alert.HEIGHT, bobbyicons::alert.buffer);
            tft.setSwapBytes(false);
        }
        return;
    }
    else
    {
        if (!clear_needed)
        {
            tft.fillScreen(TFT_BLACK);
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
        }
        clear_needed = true;
    }

    if (m_left_x != m_last_left_x || m_left_y != m_last_left_y || m_left_pressed != m_last_left_pressed)
    {
        tft.fillCircle(m_last_left_x,   m_last_left_y, static_radius-5, TFT_BLACK);

        m_last_left_x = m_left_x;
        m_last_left_y = m_left_y;
        m_last_left_pressed = m_left_pressed;

        tft.fillCircle(m_left_x, m_left_y, static_radius-5, m_left_pressed ? TFT_GREEN : TFT_WHITE);
        tft.drawCircle( left_x_mid, y_mid, static_radius, TFT_WHITE);
    }

    if (m_right_x != m_last_right_x || m_right_y != m_last_right_y || m_right_pressed != m_last_right_pressed)
    {
        tft.fillCircle(m_last_right_x,   m_last_right_y, static_radius-5, TFT_BLACK);

        m_last_right_x = m_right_x;
        m_last_right_y = m_right_y;
        m_last_right_pressed = m_right_pressed;

        tft.fillCircle(m_right_x, m_right_y, static_radius-5, m_right_pressed ? TFT_GREEN : TFT_WHITE);
        tft.drawCircle( right_x_mid, y_mid, static_radius, TFT_WHITE);
    }

    if (!has_values(left_stick) || !has_values(right_stick))
        return;

    m_left_label.redraw(fmt::format("left: x={:.2f}, y={:.2f}", *left_stick.x, *left_stick.y));
    m_right_label.redraw(fmt::format("right: x={:.2f}, y={:.2f}", *right_stick.x, *right_stick.y));

    if (bobbyStats.valid)
    {
        m_stats_label.redraw(fmt::format("Speed: {:.2f} km/h", bobbyStats.avgSpeed));
    }
}

void StatusScreen::buttonPressed(espgui::Button button)
{
    Base::buttonPressed(button);

    switch (button)
    {
    case espgui::Button::Left:
    case espgui::Button::Right:
        espgui::pushScreen<MainMenu>();
        break;
    default:;
    }
}
