#include "taskmanager.h"

// esp-idf includes
#include <esp_log.h>

// local includes
#include "analog_sticks.h"
#include "ble.h"
#include "debugconsole.h"
#include "dpad.h"
#include "gamecontroller/gamecontroller.h"

using namespace std::chrono_literals;

namespace {
using namespace espcpputils;
constexpr const char * const TAG = "TASKS";

void not_needed() {}

SchedulerTask schedulerTasksArr[] {
    SchedulerTask { "debugconsole",   init_debugconsole,    update_debugconsole,    50ms },
    SchedulerTask { "analog_sticks",  analog_sticks::init,  analog_sticks::update,  30ms },
    SchedulerTask { "ble",            ble::init,            ble::update,            50ms },
    SchedulerTask { "dpad",           buttons::init,        buttons::update,        30ms },
    SchedulerTask { "gamecontroller", gamecontroller::init, gamecontroller::update, 30ms },
};
} // namespace

cpputils::ArrayView<SchedulerTask> schedulerTasks{std::begin(schedulerTasksArr), std::end(schedulerTasksArr)};

void sched_pushStats(bool printTasks)
{
    if (printTasks)
        ESP_LOGI(TAG, "begin listing tasks...");

    for (auto &schedulerTask : schedulerTasks)
        schedulerTask.pushStats(printTasks);

    if (printTasks)
        ESP_LOGI(TAG, "end listing tasks");
}
