constexpr const char * const TAG = "ANALOG_STICKS";
#include "analog_sticks.h"

// esp-idf includes
#include "esp32-hal-gpio.h"
#include <esp_adc/adc_cali.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_log.h>

// local includes
#include "bobbybuttons.h"
#include "dualboot.h"
#include "gamecontroller/gamecontroller.h"
#include "pins.h"
#include "screenmanager.h"
#include "settings.h"
#include "utils.h"

namespace analog_sticks {

analog_stick_t left_stick;
analog_stick_t right_stick;

namespace {
bool last_left_state;
bool last_right_state;

adc_oneshot_unit_handle_t adc1_handle;

adc_unit_t adc_unit = ADC_UNIT_1;

adc_channel_t ADC_CHANNEL_LEFT_X;
adc_channel_t ADC_CHANNEL_LEFT_Y;

adc_channel_t ADC_CHANNEL_RIGHT_X;
adc_channel_t ADC_CHANNEL_RIGHT_Y;
} // namespace

void init()
{
    adc_oneshot_io_to_channel(pins::left_x, &adc_unit, &ADC_CHANNEL_LEFT_X);
    adc_oneshot_io_to_channel(pins::left_y, &adc_unit, &ADC_CHANNEL_LEFT_Y);
    adc_oneshot_io_to_channel(pins::right_x, &adc_unit, &ADC_CHANNEL_RIGHT_X);
    adc_oneshot_io_to_channel(pins::right_y, &adc_unit, &ADC_CHANNEL_RIGHT_Y);


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

    pinMode(pins::left_btn, INPUT_PULLUP);
    pinMode(pins::right_btn, INPUT_PULLUP);

    // analog reads
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    if (const auto result = adc_oneshot_new_unit(&init_config1, &adc1_handle); result != ESP_OK)
        ESP_LOGE(TAG, "adc_oneshot_new_unit() failed with %s", esp_err_to_name(result));

    adc_oneshot_chan_cfg_t config = {
        .atten = ADC_ATTEN_DB_11,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    if (const auto result = adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_LEFT_X, &config); result != ESP_OK)
        ESP_LOGE(TAG, "adc_oneshot_config_channel() failed with %s", esp_err_to_name(result));
    if (const auto result = adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_LEFT_Y, &config); result != ESP_OK)
        ESP_LOGE(TAG, "adc_oneshot_config_channel() failed with %s", esp_err_to_name(result));
    if (const auto result = adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_RIGHT_X, &config); result != ESP_OK)
        ESP_LOGE(TAG, "adc_oneshot_config_channel() failed with %s", esp_err_to_name(result));
    if (const auto result = adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_RIGHT_Y, &config); result != ESP_OK)
        ESP_LOGE(TAG, "adc_oneshot_config_channel() failed with %s", esp_err_to_name(result));

    last_left_state = false;
    last_right_state = false;

    update();
}

void update()
{
    constexpr auto sampleMultipleTimes = [](adc_channel_t channel){
        int sum{};
        const auto sampleCount = configs.sampleCount.value();
        for (int i = 0; i < sampleCount; i++)
        {
            int value;
            if (const auto result = adc_oneshot_read(adc1_handle, channel, &value); result != ESP_OK)
                ESP_LOGE(TAG, "adc_oneshot_read_channel() failed with %s", esp_err_to_name(result));
            sum += value;
        }
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
        left_stick.raw_x = sampleMultipleTimes(ADC_CHANNEL_LEFT_X);
    if (!left_stick.raw_y)
        left_stick.raw_y = sampleMultipleTimes(ADC_CHANNEL_LEFT_Y);
    if (!left_stick.btn_pressed)
        left_stick.btn_pressed = !digitalRead(pins::left_btn);

    if (!right_stick.raw_x)
        right_stick.raw_x = sampleMultipleTimes(ADC_CHANNEL_RIGHT_X);
    if (!right_stick.raw_y)
        right_stick.raw_y = sampleMultipleTimes(ADC_CHANNEL_RIGHT_Y);
    if (!right_stick.btn_pressed)
        right_stick.btn_pressed = !digitalRead(pins::right_btn);

    if (left_stick.btn_pressed && *left_stick.btn_pressed != last_left_state)
    {
        last_left_state = *left_stick.btn_pressed;

        if (espgui::currentDisplay && !boot_gamecontroller)
        {
            if (last_left_state)
                espgui::currentDisplay->buttonPressed(espgui::Button(BobbyButton::LeftStick));
            else
                espgui::currentDisplay->buttonReleased(espgui::Button(BobbyButton::LeftStick));
        }
        else if (boot_gamecontroller)
        {
            if (last_left_state)
                gamecontroller::press(BobbyButton::LeftStick);
            else
                gamecontroller::release(BobbyButton::LeftStick);
        }
    }

    if (right_stick.btn_pressed && *right_stick.btn_pressed != last_right_state)
    {
        last_right_state = *right_stick.btn_pressed;

        if (espgui::currentDisplay && !boot_gamecontroller)
        {
            if (last_right_state)
                espgui::currentDisplay->buttonPressed(espgui::Button(BobbyButton::RightStick));
            else
                espgui::currentDisplay->buttonReleased(espgui::Button(BobbyButton::RightStick));
        }
        else if (boot_gamecontroller)
        {
            if (last_right_state)
                gamecontroller::press(BobbyButton::RightStick);
            else
                gamecontroller::release(BobbyButton::RightStick);
        }
    }

    // apply calibration
    if (!needs_calibration() && configs_are_valid())
    {
        if (left_stick.raw_x)
        {
            left_stick.x = utils::mapAnalogStick(*configs.leftStickXMiddleCal.value(),
                                                 *configs.leftStickXStartCal.value(),
                                                 *configs.leftStickXEndCal.value(),
                                                 *left_stick.raw_x) * (configs.invertLeftX.value() ? -1 : 1);
        }
        if (left_stick.raw_y)
        {
            left_stick.y = utils::mapAnalogStick(*configs.leftStickYMiddleCal.value(),
                                                 *configs.leftStickYStartCal.value(),
                                                 *configs.leftStickYEndCal.value(),
                                                 *left_stick.raw_y) * (configs.invertLeftY.value() ? -1 : 1);
        }
        if (right_stick.raw_x)
        {
            right_stick.x = utils::mapAnalogStick(*configs.rightStickXMiddleCal.value(),
                                                  *configs.rightStickXStartCal.value(),
                                                  *configs.rightStickXEndCal.value(),
                                                  *right_stick.raw_x) * (configs.invertRightX.value() ? -1 : 1);
        }
        if (right_stick.raw_y)
        {
            right_stick.y = utils::mapAnalogStick(*configs.rightStickYMiddleCal.value(),
                                                  *configs.rightStickYStartCal.value(),
                                                  *configs.rightStickYEndCal.value(),
                                                  *right_stick.raw_y) * (configs.invertRightY.value() ? -1 : 1);
        }
    }
}

void read_buttons()
{
    left_stick.btn_pressed = !digitalRead(pins::left_btn);
    right_stick.btn_pressed = !digitalRead(pins::right_btn);
}

bool buttons_pressed()
{
    return left_stick.btn_pressed.value_or(false) || right_stick.btn_pressed.value_or(false);
}

bool both_buttons_pressed()
{
    return left_stick.btn_pressed.value_or(false) && right_stick.btn_pressed.value_or(false);
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

bool configs_are_valid()
{
    return (
        // left stick
        configs.leftStickXStartCal.value().has_value() &&
        configs.leftStickXMiddleCal.value().has_value() &&
        configs.leftStickXEndCal.value().has_value() &&
        configs.leftStickYStartCal.value().has_value() &&
        configs.leftStickYMiddleCal.value().has_value() &&
        configs.leftStickYEndCal.value().has_value() &&
        // right stick
        configs.rightStickXStartCal.value().has_value() &&
        configs.rightStickXMiddleCal.value().has_value() &&
        configs.rightStickXEndCal.value().has_value() &&
        configs.rightStickYStartCal.value().has_value() &&
        configs.rightStickYMiddleCal.value().has_value() &&
        configs.rightStickYEndCal.value().has_value()
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
