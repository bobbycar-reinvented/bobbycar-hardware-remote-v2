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
#include "screens.h"
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

    for (const auto &task : schedulerTasks)
    {
        bootLabel.redraw(task.name());
        task.setup();
    }

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
