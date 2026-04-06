constexpr const char * const TAG = "BOBBY_REMOTE";

// esp-idf includes
#include <esp_log.h>
#include <esp_pm.h>
#include <esp_system.h>

// 3rdparty lib includes
#include <espchrono.h>
#include <screenmanager.h>

// local includes
#include "analog_sticks.h"
#include "ble.h"
#include "dualboot.h"
#include "screens.h"
#include "screens/buttonmapscreen.h"
#include "screens/calibrateanalogsticksscreen.h"
#include "screens/statusscreen.h"
#include "settings.h"
#include "taskmanager.h"

using namespace std::chrono_literals;

extern "C" void app_main()
{
    esp_reset_reason_t reset_reason = esp_reset_reason();
    if (reset_reason != ESP_RST_SW) {
        boot_gamecontroller = false;
    }

    if (const auto result = configs.init("bobbyremote"); result != ESP_OK)
        ESP_LOGE(TAG, "config_init_settings() failed with %s", esp_err_to_name(result));

    display::init();

    display::set_boot_msg("analog_sticks");

    analog_sticks::read_buttons();

    for (const auto &task : schedulerTasks)
    {
        ESP_LOGI(TAG, "Init task %s", task.name());
        display::set_boot_msg(task.name());
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
    else if (analog_sticks::needs_calibration() || analog_sticks::both_buttons_pressed())
    {
        espgui::switchScreen<CalibrateAnalogStickScreen>(true);
        ble::disableAutoConnect();
    }
    else if (!boot_gamecontroller)
    {
        espgui::switchScreen<StatusScreen>();
    }
    else
    {
        ESP_LOGI(TAG, "Booting into gamecontroller");
    }

    while (true)
    {
        for (auto &schedulerTask : schedulerTasks)
        {
            schedulerTask.loop();
        }

        display::update();

        vTaskDelay(1);
    }
}
