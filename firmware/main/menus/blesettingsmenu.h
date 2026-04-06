#pragma once

// local includes
#include "bobbymenudisplay.h"

class BleSettingsMenu : public BobbyMenuDisplay
{
    using Base = BobbyMenuDisplay;

public:
    BleSettingsMenu();

    std::string title() const override;
    void back() override;
};
