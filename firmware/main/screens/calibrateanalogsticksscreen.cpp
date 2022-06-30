#include "calibrateanalogsticksscreen.h"

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <fmt/core.h>
#include <screenmanager.h>
#include <tftinstance.h>

// local lib includes
#include "analog_sticks.h"
#include "bobbybuttons.h"
#include "menus/mainmenu.h"
#include "screens/statusscreen.h"
#include "settings.h"

namespace {
constexpr const char * const TAG = "CALIBRATE_ANALOG_STICKS";
} // namespace

CalibrateAnalogStickScreen::CalibrateAnalogStickScreen(bool from_boot) :
    m_left_x_min{4095},
    m_left_x_max{0},
    m_left_x_mid{2000},
    m_left_y_min{4095},
    m_left_y_max{0},
    m_left_y_mid{2000},
    m_right_x_min{4095},
    m_right_x_max{0},
    m_right_x_mid{2000},
    m_right_y_min{4095},
    m_right_y_max{0},
    m_right_y_mid{2000},
    m_from_boot{from_boot}
{}

void CalibrateAnalogStickScreen::initScreen()
{
    Base::initScreen();

    m_left_x_min = 4095;
    m_left_x_mid = 2000;
    m_left_x_max = 0;
    m_left_y_min = 4095;
    m_left_y_mid = 2000;
    m_left_y_max = 0;

    m_right_x_min = 4095;
    m_right_x_mid = 2000;
    m_right_x_max = 0;
    m_right_y_min = 4095;
    m_right_y_mid = 2000;
    m_right_y_max = 0;

    espgui::tft.setTextColor(TFT_WHITE, TFT_BLACK);

    espgui::tft.drawString("Press Stick to save middles.", 10,  5);
    espgui::tft.drawString("Press OK to save min/max.", 10, 40);

    m_label_leftx_stick.start();
    m_label_lefty_stick.start();
    m_label_rightx_stick.start();
    m_label_righty_stick.start();
    m_label_status.start();
}

void CalibrateAnalogStickScreen::redraw()
{
    using namespace analog_sticks;
    Base::redraw();

    m_label_leftx_stick.redraw(fmt::format( "&sLX Min:&m {} &sMax:&m {} &sMid:&m {}", m_left_x_min,  m_left_x_max,  m_left_x_mid));
    m_label_lefty_stick.redraw(fmt::format( "&sLY Min:&m {} &sMax:&m {} &sMid:&m {}", m_left_y_min,  m_left_y_max,  m_left_y_mid));
    m_label_rightx_stick.redraw(fmt::format("&sRX Min:&m {} &sMax:&m {} &sMid:&m {}", m_right_x_min, m_right_x_max, m_right_x_mid));
    m_label_righty_stick.redraw(fmt::format("&sRY Min:&m {} &sMax:&m {} &sMid:&m {}", m_right_y_min, m_right_y_max, m_right_y_mid));
    m_label_status.redraw(fmt::format("{}", (has_raw_values(left_stick) && has_raw_values(right_stick)) ? "Ready" : "No raw values"));
}

void CalibrateAnalogStickScreen::update()
{
    using namespace analog_sticks;
    Base::update();

    if (!analog_sticks::has_raw_values(left_stick))
    {
        ESP_LOGI(TAG, "left stick has no values");
        return;
    }

    if (!analog_sticks::has_raw_values(right_stick))
    {
        ESP_LOGI(TAG, "right stick has no values");
        return;
    }

    if (const auto left_x_val = analog_sticks::left_stick.raw_x; left_x_val)
    {
        m_left_x_min = std::min<uint16_t>(*left_x_val, m_left_x_min);
        m_left_x_max = std::max<uint16_t>(*left_x_val, m_left_x_max);
    }
    else
        ESP_LOGI(TAG, "left stick has no x value");

    if (const auto left_y_val = analog_sticks::left_stick.raw_y; left_y_val)
    {
        m_left_y_min = std::min<uint16_t>(*left_y_val, m_left_y_min);
        m_left_y_max = std::max<uint16_t>(*left_y_val, m_left_y_max);
    }
    else
        ESP_LOGI(TAG, "left stick has no y value");

    if (const auto right_x_val = analog_sticks::right_stick.raw_x; right_x_val)
    {
        m_right_x_min = std::min<uint16_t>(*right_x_val, m_right_x_min);
        m_right_x_max = std::max<uint16_t>(*right_x_val, m_right_x_max);
    }
    else
        ESP_LOGI(TAG, "right stick has no x value");

    if (const auto right_y_val = analog_sticks::right_stick.raw_y; right_y_val)
    {
        m_right_y_min = std::min<uint16_t>(*right_y_val, m_right_y_min);
        m_right_y_max = std::max<uint16_t>(*right_y_val, m_right_y_max);
    }
    else
        ESP_LOGI(TAG, "right stick has no y value");
}

void CalibrateAnalogStickScreen::buttonPressed(espgui::Button button)
{
    Base::buttonPressed(button);

    if (!is_done())
    {
        using namespace analog_sticks;
        switch (BobbyButton(button))
        {
        case BobbyButton::LeftStick:
            if (has_raw_values(left_stick))
            {
                m_left_x_mid = *left_stick.raw_x;
                m_left_y_mid = *left_stick.raw_y;
            }
            break;
        case BobbyButton::RightStick:
            if (has_raw_values(right_stick))
            {
                m_right_x_mid = *right_stick.raw_x;
                m_right_y_mid = *right_stick.raw_y;
            }
            break;
        default:;
        }
    }
    else
    {
        switch (button)
        {
        case espgui::Button::Left:
        case espgui::Button::Right:
        {
            if (const auto res = configs.write_config(configs.leftStickXStartCal, m_left_x_min); !res)
            {
                ESP_LOGE(TAG, "Failed to write left x min: %s", res.error().c_str());
            }

            if (const auto res = configs.write_config(configs.leftStickXEndCal, m_left_x_max); !res)
            {
                ESP_LOGE(TAG, "Failed to write left x max: %s", res.error().c_str());
            }

            if (const auto res = configs.write_config(configs.leftStickXMiddleCal, m_left_x_mid); !res)
            {
                ESP_LOGE(TAG, "Failed to write left x mid: %s", res.error().c_str());
            }

            if (const auto res = configs.write_config(configs.leftStickYStartCal, m_left_y_min); !res)
            {
                ESP_LOGE(TAG, "Failed to write left y min: %s", res.error().c_str());
            }

            if (const auto res = configs.write_config(configs.leftStickYEndCal, m_left_y_max); !res)
            {
                ESP_LOGE(TAG, "Failed to write left y max: %s", res.error().c_str());
            }

            if (const auto res = configs.write_config(configs.leftStickYMiddleCal, m_left_y_mid); !res)
            {
                ESP_LOGE(TAG, "Failed to write left y mid: %s", res.error().c_str());
            }

            if (const auto res = configs.write_config(configs.rightStickXStartCal, m_right_x_min); !res)
            {
                ESP_LOGE(TAG, "Failed to write right x min: %s", res.error().c_str());
            }

            if (const auto res = configs.write_config(configs.rightStickXEndCal, m_right_x_max); !res)
            {
                ESP_LOGE(TAG, "Failed to write right x max: %s", res.error().c_str());
            }

            if (const auto res = configs.write_config(configs.rightStickXMiddleCal, m_right_x_mid); !res)
            {
                ESP_LOGE(TAG, "Failed to write right x mid: %s", res.error().c_str());
            }

            if (const auto res = configs.write_config(configs.rightStickYStartCal, m_right_y_min); !res)
            {
                ESP_LOGE(TAG, "Failed to write right y min: %s", res.error().c_str());
            }

            if (const auto res = configs.write_config(configs.rightStickYEndCal, m_right_y_max); !res)
            {
                ESP_LOGE(TAG, "Failed to write right y max: %s", res.error().c_str());
            }

            if (const auto res = configs.write_config(configs.rightStickYMiddleCal, m_right_y_mid); !res)
            {
                ESP_LOGE(TAG, "Failed to write right y mid: %s", res.error().c_str());
            }

            if (m_from_boot)
                espgui::switchScreen<StatusScreen>();
            else
                espgui::pushScreen<MainMenu>();
            break;
        }
        default:;
        }
    }
}

bool CalibrateAnalogStickScreen::is_done() const
{
    return (
        m_left_x_min != 4095 &&
        m_left_x_max != 0 &&
        m_left_x_mid != 2000 &&
        m_left_y_min != 4095 &&
        m_left_y_max != 0 &&
        m_left_y_mid != 2000 &&
        m_right_x_min != 4095 &&
        m_right_x_max != 0 &&
        m_right_x_mid != 2000 &&
        m_right_y_min != 4095 &&
        m_right_y_max != 0 &&
        m_right_y_mid != 2000
    );
}
