constexpr const char * const TAG = "BOBBY_REMOTE";

// esp-idf includes
#include <esp32-hal-gpio.h>
#include <esp_chip_info.h>
#include <esp_log.h>
#include <esp_pm.h>

// 3rdparty lib includes
#include <espchrono.h>
#include <espwifistack.h>
#include <espwifistack.h>
#include <schedulertask.h>
#include <screenmanager.h>

// local includes
#include "analog_sticks.h"
#include "ble.h"
#include "screens.h"
#include "screens/buttonmapscreen.h"
#include "screens/calibrateanalogsticksscreen.h"
#include "screens/statusscreen.h"
#include "settings.h"
#include "taskmanager.h"

using namespace std::chrono_literals;

extern "C" void app_main()
{
    pinMode(PIN_LED_BACKLIGHT, OUTPUT);
    digitalWrite(PIN_LED_BACKLIGHT, LOW);

    display::init();

    bootLabel.redraw("settings");

    if (const auto result = configs.init("bobbyremote"); result != ESP_OK)
        ESP_LOGE(TAG, "config_init_settings() failed with %s", esp_err_to_name(result));

    bootLabel.redraw("analog_sticks");

    analog_sticks::read_buttons();

    for (const auto &task : schedulerTasks)
    {
        ESP_LOGI(TAG, "Init task %s", task.name());
        bootLabel.redraw(task.name());
        task.setup();
    }

    // check if analog sticks are mapped
    if (
        configs.dpadUp.value() == INPUT_MAPPING_NONE ||
        configs.dpadDown.value() == INPUT_MAPPING_NONE ||
        configs.dpadLeft.value() == INPUT_MAPPING_NONE ||
        configs.dpadRight.value() == INPUT_MAPPING_NONE ||
        configs.dpadUp2.value() == INPUT_MAPPING_NONE ||
        configs.dpadDown2.value() == INPUT_MAPPING_NONE ||
        configs.dpadLeft2.value() == INPUT_MAPPING_NONE ||
        configs.dpadRight2.value() == INPUT_MAPPING_NONE
       )
    {
        espgui::switchScreen<ButtonMapScreen>();
        ble::disableAutoConnect();
    }
    else if (analog_sticks::needs_calibration() || analog_sticks::buttons_pressed())
    {
        espgui::switchScreen<CalibrateAnalogStickScreen>(true);
        ble::disableAutoConnect();
    }
    else
        espgui::switchScreen<StatusScreen>();

    while (true)
    {
        const auto now = espchrono::millis_clock::now();

        for (auto &schedulerTask : schedulerTasks)
        {
            schedulerTask.loop();
        }

        display::update();

        vTaskDelay(1);
    }
}
