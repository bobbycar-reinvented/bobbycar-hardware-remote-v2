#include "taskmanager.h"

#include "esp_log.h"

using namespace std::chrono_literals;

namespace {
using namespace espcpputils;
constexpr const char * const TAG = "TASKS";

void not_needed() {}

void debug() { ESP_LOGI(TAG, "Debug text"); }

SchedulerTask schedulerTasksArr[] {
    SchedulerTask { "debug_log", not_needed, debug, 1s }
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
