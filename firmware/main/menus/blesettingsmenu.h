#pragma once

// local includes
#include "bobbymenudisplay.h"

class BleSettingsMenu : public BobbyMenuDisplay
{
    using Base = BobbyMenuDisplay;

public:
    BleSettingsMenu();

    [[nodiscard]] std::string text() const override;
    void back() override;
};
