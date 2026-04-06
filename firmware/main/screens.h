#pragma once

// esp-idf includes
#include <hal/gpio_types.h>
#include <driver/ledc.h>
#include <hal/adc_types.h>

// 3rdparty lib includes
#include <widgets/label.h>

extern espgui::Label bootLabel;

namespace display {

constexpr auto displayBacklightPin = PIN_LED_BACKLIGHT;
constexpr auto displayBacklightChannel = ledc_channel_t::LEDC_CHANNEL_0;
constexpr auto displayBacklightFrequency = 5000;
constexpr auto displayBacklightResolution = ledc_timer_bit_t::LEDC_TIMER_13_BIT;
constexpr auto displayFadeDivider = 18;
constexpr auto displayMaxDuty = (1 << displayBacklightResolution);

void init();
void update();
void set_boot_msg(std::string_view msg);
} // namespace display
