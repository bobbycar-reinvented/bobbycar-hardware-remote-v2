#include "dualboot.h"

// esp-idf includes
#include "esp_system.h"

RTC_NOINIT_ATTR bool boot_gamecontroller;

void boot_into_gamecontroller()
{
    boot_gamecontroller = true;
    esp_restart();
}
