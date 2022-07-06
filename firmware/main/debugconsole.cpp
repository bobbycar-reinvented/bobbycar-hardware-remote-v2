#include "debugconsole.h"

// system includes
#include <string_view>

// esp-idf includes
#include <driver/uart.h>
#include <esp_log.h>

// 3rdparty lib includes
#include <changevaluedisplay_string.h>
#include <screenmanager.h>
#include <tftinstance.h>

// local includes
#include "settings.h"

namespace {
constexpr const char * const TAG = "DEBUG";

uint8_t consoleControlCharsReceived{};
bool uart0InitDone{};

void handleNormalChar(char c);
void handleSpecialChar(char c);
} // namespace

void init_debugconsole()
{
    if (const auto result = uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, 3, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE); result != ESP_OK)
    {
        ESP_LOGE(TAG, "uart_set_pin() failed with %s", esp_err_to_name(result));
    }

    if (const auto result = uart_driver_install(UART_NUM_0, SOC_UART_FIFO_LEN + 1, 0, 10, nullptr, 0); result != ESP_OK)
        ESP_LOGE(TAG, "uart_driver_install() failed with %s", esp_err_to_name(result));
    else
        uart0InitDone = true;
}

void update_debugconsole()
{
    if (!uart0InitDone)
        return;

    size_t length{};

    if (const auto result = uart_get_buffered_data_len(UART_NUM_0, &length); result != ESP_OK)
    {
        ESP_LOGW(TAG, "uart_get_buffered_data_len() failed with %s", esp_err_to_name(result));
    }
    else if (length)
    {
        char data[length];
        length = uart_read_bytes(UART_NUM_0, data, length, 0);

        for (char c : std::string_view{data, length})
        {
            if (consoleControlCharsReceived < 2)
            {
                switch (c)
                {
                case '\x1b':
                {
                    if (consoleControlCharsReceived == 0)
                        consoleControlCharsReceived = 1;
                    else
                        consoleControlCharsReceived = 0;
                    break;
                }
                case '\x5b':
                {
                    if (consoleControlCharsReceived == 1)
                        consoleControlCharsReceived = 2;
                    else
                        consoleControlCharsReceived = 0;
                    break;
                }
                default:
                    consoleControlCharsReceived = 0;
                    handleNormalChar(c);
                }
            }
            else
            {
                consoleControlCharsReceived = 0;
                handleSpecialChar(c);
            }
        }
    }
}

namespace {
void handleNormalChar(char c)
{
    if (auto display = espgui::currentDisplay ? espgui::currentDisplay->asChangeValueDisplayString() : nullptr)
    {
        switch (c)
        {
        case ' ' ... '~':
            display->setShownValue(display->shownValue() + c);
            break;
        case 8:
            if (auto val = display->shownValue(); !val.empty())
            {
                val.pop_back();
                display->setShownValue(std::move(val));
            }
            break;
        default:
            ESP_LOGW(TAG, "unknown char %hhx %c", c, c);
        }
    }
    else
    {
        switch (c)
        {
        case 'r':
        case 'R':
            ESP_LOGI(TAG, "Rebooting...");
            esp_restart();
            break;
        case 'i':
        case 'I':
            if (auto &display = espgui::currentDisplay; display)
            {
                display->initScreen();
            }
            break;
        case 'q':
        case 'Q':
            /*
            configs.reset_config(configs.dpadUp);
            configs.reset_config(configs.dpadDown);
            configs.reset_config(configs.dpadRight);
            configs.reset_config(configs.dpadLeft);
            configs.reset_config(configs.dpadUp2);
            configs.reset_config(configs.dpadDown2);
            configs.reset_config(configs.dpadRight2);
            configs.reset_config(configs.dpadLeft2);
            ESP_LOGI(TAG, "Reset all configs");
            */
            break;
        }
    }
}

void handleSpecialChar(char c)
{
    switch (c)
    {
    case 'A': // Up arrow pressed
        if (espgui::currentDisplay)
        {
            espgui::currentDisplay->buttonPressed(espgui::Button::Up);
            espgui::currentDisplay->buttonReleased(espgui::Button::Up);
        }
        break;
    case 'B': // Down arrow pressed
        if (espgui::currentDisplay)
        {
            espgui::currentDisplay->buttonPressed(espgui::Button::Down);
            espgui::currentDisplay->buttonReleased(espgui::Button::Down);
        }
        break;
    case 'C': // Right arrow pressed
        if (espgui::currentDisplay)
        {
            espgui::currentDisplay->buttonPressed(espgui::Button::Right);
            espgui::currentDisplay->buttonReleased(espgui::Button::Right);
        }
        break;
    case 'D': // Left arrow pressed
        if (espgui::currentDisplay)
        {
            espgui::currentDisplay->buttonPressed(espgui::Button::Left);
            espgui::currentDisplay->buttonReleased(espgui::Button::Left);
        }
        break;
    default:
        ESP_LOGI(TAG, "unknown control char received: %hhx", c);
    }
}
} // namespace
