constexpr const char * const TAG = "BUTTON_CALIBRATION";
#include "buttonmapscreen.h"

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <screenmanager.h>

// local includes
#include "screens/statusscreen.h"
#include "settings.h"

void ButtonMapScreen::initScreen(espgui::TftInterface &tft)
{
    using namespace espgui;

    Base::initScreen(tft);

    m_dpad_left.start(tft);
    m_dpad_right.start(tft);

    m_label.start(tft);
}

void ButtonMapScreen::update()
{
    Base::update();
}

void ButtonMapScreen::redraw(espgui::TftInterface &tft)
{
    Base::redraw(tft);

    switch(m_current_button)
    {
    case 0:
        m_dpad_left.redraw(tft, UP);
        m_dpad_right.redraw(tft, None);
        break;
    case 1:
        m_dpad_left.redraw(tft, RIGHT);
        m_dpad_right.redraw(tft, None);
        break;
    case 2:
        m_dpad_left.redraw(tft, DOWN);
        m_dpad_right.redraw(tft, None);
        break;
    case 3:
        m_dpad_left.redraw(tft, LEFT);
        m_dpad_right.redraw(tft, None);
        break;
    case 4:
        m_dpad_left.redraw(tft, None);
        m_dpad_right.redraw(tft, UP);
        break;
    case 5:
        m_dpad_left.redraw(tft, None);
        m_dpad_right.redraw(tft, RIGHT);
        break;
    case 6:
        m_dpad_left.redraw(tft, None);
        m_dpad_right.redraw(tft, DOWN);
        break;
    case 7:
        m_dpad_left.redraw(tft, None);
        m_dpad_right.redraw(tft, LEFT);
        break;
    default:
        m_dpad_left.redraw(tft, None);
        m_dpad_right.redraw(tft, None);

        m_label.redraw(tft, "Press RIGHT to continue", espgui::TFT_WHITE, espgui::TFT_BLACK, 4);
        break;
    }
}

void ButtonMapScreen::buttonPressed(const espgui::Button button)
{
    ESP_LOGI(TAG, "Button %d pressed", std::to_underlying(button));
    // Base::buttonPressed(button);
    if (m_current_button < 7)
    {
        return;
    }

    if (button == espgui::Button::Right)
    {
        espgui::switchScreen<StatusScreen>();
    }
}

void ButtonMapScreen::buttonReleased(espgui::Button button)
{
    // Base::buttonReleased(button);
}

void ButtonMapScreen::rawButtonPressed(const uint8_t button)
{
    // Base::rawButtonPressed(button);
    switch(m_current_button)
    {
    case 0:
        if (const auto res = configs.write_config(configs.dpadUp2, button); !res)
        {
            ESP_LOGE(TAG, "Failed to write config: %s", res.error().c_str());
            return;
        }
        m_current_button++;
        break;
    case 1:
        if (const auto res = configs.write_config(configs.dpadRight2, button); !res)
        {
            ESP_LOGE(TAG, "Failed to write config: %s", res.error().c_str());
            return;
        }
        m_current_button++;
        break;
    case 2:
        if (const auto res = configs.write_config(configs.dpadDown2, button); !res)
        {
            ESP_LOGE(TAG, "Failed to write config: %s", res.error().c_str());
            return;
        }
        m_current_button++;
        break;
    case 3:
        if (const auto res = configs.write_config(configs.dpadLeft2, button); !res)
        {
            ESP_LOGE(TAG, "Failed to write config: %s", res.error().c_str());
            return;
        }
        m_current_button++;
        break;
    case 4:
        if (const auto res = configs.write_config(configs.dpadUp, button); !res)
        {
            ESP_LOGE(TAG, "Failed to write config: %s", res.error().c_str());
            return;
        }
        m_current_button++;
        break;
    case 5:
        if (const auto res = configs.write_config(configs.dpadRight, button); !res)
        {
            ESP_LOGE(TAG, "Failed to write config: %s", res.error().c_str());
            return;
        }
        m_current_button++;
        break;
    case 6:
        if (const auto res = configs.write_config(configs.dpadDown, button); !res)
        {
            ESP_LOGE(TAG, "Failed to write config: %s", res.error().c_str());
            return;
        }
        m_current_button++;
        break;
    case 7:
        if (const auto res = configs.write_config(configs.dpadLeft, button); !res)
        {
            ESP_LOGE(TAG, "Failed to write config: %s", res.error().c_str());
            return;
        }
        m_current_button++;
        break;
    default:;
    }

    Base::rawButtonPressed(button);
}

void ButtonMapScreen::rawButtonReleased(uint8_t button)
{
    // Base::rawButtonReleased(button);
}

std::string ButtonMapScreen::title() const
{
    return "Button Mapping";
}
