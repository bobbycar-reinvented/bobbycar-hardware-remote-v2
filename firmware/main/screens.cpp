#include "screens.h"

// esp-idf includes
#include <driver/ledc.h>
#include <esp32-hal-gpio.h>
#include <esp_log.h>

// 3rdparty lib includes
#include <TFT_eSPI.h>
#include <espstrutils.h>
#include <fontrenderer.h>
#include <screenmanager.h>
#include <tftcolors.h>

// local includes
#include "icons/logo.h"
#include "settings.h"

using namespace espgui;
using namespace std::chrono_literals;

constexpr const char * const TAG = "screens";

espgui::Label bootLabel{32, bobbyicons::logo.HEIGHT + 75};

namespace display {

int fixDuty(int duty)
{
#ifdef LEDBACKLIGHT_INVERTED
    return displayMaxDuty - duty;
#else
    return duty;
#endif
}

espchrono::millis_clock::time_point lastDisplayUpdate;
espchrono::millis_clock::time_point lastDisplayRedraw;

namespace {

TFT_eSPI tft;

} // namespace

void init()
{
    tft.init();

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    ledc_timer_config_t display_backlight{
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = display::displayBacklightResolution,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = display::displayBacklightFrequency,
        .clk_cfg = LEDC_AUTO_CLK,
    };

    if (const auto res = ledc_timer_config(&display_backlight); res != ESP_OK)
        ESP_LOGE(TAG, "ledc_timer_config() failed with %s", esp_err_to_name(res));
    else
        ESP_LOGI(TAG, "ledc_timer_config() succeeded");

    ledc_channel_config_t display_backlight_channel{
        .gpio_num = display::displayBacklightPin,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = display::displayBacklightChannel,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0,
    };

    if (const auto res = ledc_channel_config(&display_backlight_channel); res != ESP_OK)
        ESP_LOGE(TAG, "ledc_channel_config() failed with %s", esp_err_to_name(res));
    else
        ESP_LOGI(TAG, "ledc_channel_config() succeeded");

    if (const auto res = ledc_set_duty(LEDC_LOW_SPEED_MODE, display::displayBacklightChannel, display::fixDuty(0)); res != ESP_OK)
        ESP_LOGE(TAG, "ledc_set_duty() failed with %s", esp_err_to_name(res));
    else
        ESP_LOGI(TAG, "ledc_set_duty() succeeded");

    if (const auto res = ledc_update_duty(LEDC_LOW_SPEED_MODE, display::displayBacklightChannel); res != ESP_OK)
        ESP_LOGE(TAG, "ledc_update_duty() failed with %s", esp_err_to_name(res));
    else
        ESP_LOGI(TAG, "ledc_update_duty() succeeded");

    tft.fillScreen(espgui::TFT_WHITE);
    // tft.setRotation(configs.displayRotated.value() ? 1 : 3);

    auto renderer = espgui::FontRenderer{tft};

    tft.pushImage((320/2)-(bobbyicons::logo.WIDTH/2), 10, bobbyicons::logo.WIDTH, bobbyicons::logo.HEIGHT, bobbyicons::logo.buffer);
    renderer.drawString("Remote-OS", 32,  bobbyicons::logo.HEIGHT + 10 + 7, espgui::TFT_BLACK, espgui::TFT_WHITE, 4);
    renderer.drawString("booting...", 32, 225, espgui::TFT_BLACK, espgui::TFT_WHITE, 4);
    renderer.drawString("last reboot reason:", 32, 275, espgui::TFT_BLACK, espgui::TFT_WHITE, 2);
    renderer.drawString(espcpputils::toString(esp_reset_reason()), 32, 295, espgui::TFT_BLACK, espgui::TFT_WHITE, 2);
    bootLabel.start(tft);

    if (const auto res = ledc_set_duty(LEDC_LOW_SPEED_MODE, display::displayBacklightChannel, display::fixDuty(display::displayMaxDuty)); res != ESP_OK)
        ESP_LOGE(TAG, "ledc_set_duty() failed with %s", esp_err_to_name(res));
    else
        ESP_LOGI(TAG, "ledc_set_duty() succeeded");

    if (const auto res = ledc_update_duty(LEDC_LOW_SPEED_MODE, display::displayBacklightChannel); res != ESP_OK)
        ESP_LOGE(TAG, "ledc_update_duty() failed with %s", esp_err_to_name(res));
    else
        ESP_LOGI(TAG, "ledc_update_duty() succeeded");

    ESP_LOGI(TAG, "TFT init done (%dx%d, w*h=%d)", tft.width(), tft.height(), tft.width() * tft.height());
}

void update()
{
    if (tft.getRotation() != (configs.displayRotated.value() ? 1 : 3))
    {
        tft.setRotation(configs.displayRotated.value() ? 1 : 3);

        if (espgui::currentDisplay)
            espgui::currentDisplay->initScreen(tft);
    }

    if (espchrono::ago(lastDisplayUpdate) >= 1000ms/50)
    {
        lastDisplayUpdate = espchrono::millis_clock::now();

        if (espgui::currentDisplay)
            espgui::currentDisplay->update();

        if (espgui::changeScreenCallback)
        {
            espgui::changeScreenCallback(tft);
            espgui::changeScreenCallback = {};
        }
    }

    if (espchrono::ago(lastDisplayRedraw) >= 1000ms/60)
    {
        lastDisplayRedraw = espchrono::millis_clock::now();

        if (espgui::currentDisplay)
            espgui::currentDisplay->redraw(tft);
    }
}

void set_boot_msg(std::string_view msg)
{
    bootLabel.redraw(tft, msg, espgui::TFT_BLACK, espgui::TFT_WHITE, 4);
}

} // namespace display
