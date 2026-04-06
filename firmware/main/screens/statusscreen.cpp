#include "statusscreen.h"

// system includes
#include <format>

// 3rdparty lib includes
#include <cpputils.h>
#include <espchrono.h>
#include <fontrenderer.h>
#include <screenmanager.h>

// local lib includes
#include "analog_sticks.h"
#include "ble.h"
#include "globals.h"
#include "icons/alert.h"
#include "menus/mainmenu.h"

using namespace analog_sticks;
using namespace std::chrono_literals;

namespace {
constexpr auto static_radius = 30;
constexpr auto distance = 30;

int left_x_mid, left_x_start, left_x_end   = 0;
int right_x_mid, right_x_start, right_x_end = 0;
int y_mid, y_start, y_end = 0;

void update_values(const espgui::TftInterface &tft)
{
    left_x_mid   = distance + static_radius;
    left_x_start = left_x_mid - static_radius;
    left_x_end   = left_x_mid + static_radius;

    right_x_mid = tft.width() - distance - static_radius;
    right_x_start = right_x_mid - static_radius;
    right_x_end = right_x_mid + static_radius;

    y_mid = tft.height() - distance - static_radius;
    y_start = y_mid - static_radius;
    y_end = y_mid + static_radius;
}

constexpr char TEXT_NOT_CONNECTED[] = "Not connected!";
} // namespace

void StatusScreen::initScreen(espgui::TftInterface &tft)
{
    update_values(tft);

    Base::initScreen(tft);

    m_last_left_x = 0;
    m_last_left_y = 0;
    m_last_right_x = 0;
    m_last_right_y = 0;

    m_left_label.start(tft);
    m_right_label.start(tft);
    m_stats_label.start(tft);

    clear_needed = true;
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

void StatusScreen::redraw(espgui::TftInterface &tft)
{
    using namespace espgui;

    update_values(tft);

    espgui::FontRenderer renderer{tft};

    Base::redraw(tft);

    if (!ble::isConnected())
    {
        if (clear_needed)
        {
            clear_needed = false;
            m_left_label.clear(tft, espgui::TFT_BLACK);
            m_right_label.clear(tft, espgui::TFT_BLACK);

            tft.fillScreen(espgui::TFT_BLACK);

            const auto fontOffset = renderer.textWidth(TEXT_NOT_CONNECTED, 4);
            renderer.drawString(TEXT_NOT_CONNECTED, tft.width() / 2 - 10 - (fontOffset/2), (tft.height()-renderer.fontHeight(4)) / 2, espgui::TFT_RED, espgui::TFT_BLACK, 4);

            tft.pushImage(tft.width() / 2 + (fontOffset/2), (tft.height()-bobbyicons::alert.HEIGHT) / 2, bobbyicons::alert.WIDTH, bobbyicons::alert.HEIGHT, bobbyicons::alert.buffer);
        }
        return;
    }
    else
    {
        if (!clear_needed)
        {
            tft.fillScreen(espgui::TFT_BLACK);
        }
        clear_needed = true;
    }

    if (m_left_x != m_last_left_x || m_left_y != m_last_left_y || m_left_pressed != m_last_left_pressed)
    {
        tft.fillCircle(m_last_left_x,   m_last_left_y, static_radius-5, espgui::TFT_BLACK);

        m_last_left_x = m_left_x;
        m_last_left_y = m_left_y;
        m_last_left_pressed = m_left_pressed;

        tft.fillCircle(m_left_x, m_left_y, static_radius-5, m_left_pressed ? espgui::TFT_GREEN : espgui::TFT_WHITE);
        tft.drawCircle( left_x_mid, y_mid, static_radius, espgui::TFT_WHITE);
    }

    if (m_right_x != m_last_right_x || m_right_y != m_last_right_y || m_right_pressed != m_last_right_pressed)
    {
        tft.fillCircle(m_last_right_x,   m_last_right_y, static_radius-5, espgui::TFT_BLACK);

        m_last_right_x = m_right_x;
        m_last_right_y = m_right_y;
        m_last_right_pressed = m_right_pressed;

        tft.fillCircle(m_right_x, m_right_y, static_radius-5, m_right_pressed ? espgui::TFT_GREEN : espgui::TFT_WHITE);
        tft.drawCircle( right_x_mid, y_mid, static_radius, espgui::TFT_WHITE);
    }

    if (!has_values(left_stick) || !has_values(right_stick))
        return;

    m_left_label.redraw(tft, std::format("left: x={:.2f}, y={:.2f}", *left_stick.x, *left_stick.y), espgui::TFT_WHITE, espgui::TFT_BLACK, 4);
    m_right_label.redraw(tft, std::format("right: x={:.2f}, y={:.2f}", *right_stick.x, *right_stick.y), espgui::TFT_WHITE, espgui::TFT_BLACK, 4);

    if (bobbyStats.valid)
    {
        m_stats_label.redraw(tft, std::format("Speed: {:.2f} km/h - {}ms", bobbyStats.avgSpeed, ble::lastUpdate ? espchrono::ago(*ble::lastUpdate) / 1ms : 0), espgui::TFT_WHITE, espgui::TFT_BLACK, 4);
    }
}

void StatusScreen::buttonPressed(const espgui::Button button)
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
