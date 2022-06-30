#include "bobbybuttons.h"

// esp-idf includes
#include <esp_log.h>

// local includes
#include "settings.h"

namespace {
    constexpr const char TAG[] = "BUTTONS";
} // namespace

[[nodiscard]] std::optional<espgui::Button> translateRawButton(uint8_t button)
{
    // Invalid
    if (button == INPUT_MAPPING_NONE)
        return std::nullopt;

    using espgui::Button;
    if (configs.dpadLeft.value() == button)
        return Button::Left;
    if (configs.dpadRight.value() == button)
        return Button::Right;
    if (configs.dpadUp.value() == button)
        return Button::Up;
    if (configs.dpadDown.value() == button)
        return Button::Down;
    if (configs.dpadLeft2.value() == button)
        return Button(BobbyButton::Left2);
    if (configs.dpadRight2.value() == button)
        return Button(BobbyButton::Right2);
    if (configs.dpadUp2.value() == button)
        return Button(BobbyButton::Up2);
    if (configs.dpadDown2.value() == button)
        return Button(BobbyButton::Down2);

    ESP_LOGW(TAG, "unknown raw button %hhu", button);

    return std::nullopt;
}

void buttonPressedCommon(espgui::Button button)
{
    switch (BobbyButton(button))
    {
    case BobbyButton::Left2:
    case BobbyButton::Right2:
    case BobbyButton::Up2:
    case BobbyButton::Down2:
        break;
    default:;
    }
}

void buttonReleasedCommon(espgui::Button button)
{
    switch (BobbyButton(button))
    {
    case BobbyButton::Left2:
    case BobbyButton::Right2:
    case BobbyButton::Up2:
    case BobbyButton::Down2:
        break;
    default:;
    }
}

void BobbyButtons::rawButtonPressed(uint8_t button)
{
    //Base::rawButtonPressed(button);
    if (const auto translated = translateRawButton(button))
        buttonPressed(*translated);
}

void BobbyButtons::rawButtonReleased(uint8_t button)
{
    //Base::rawButtonReleased(button);
    if (const auto translated = translateRawButton(button))
        buttonReleased(*translated);
}

void BobbyButtons::buttonPressed(espgui::Button button)
{
    //Base::buttonPressed(button);
    buttonPressedCommon(button);
}

void BobbyButtons::buttonReleased(espgui::Button button)
{
    //Base::buttonReleased(button);
    buttonReleasedCommon(button);
}
