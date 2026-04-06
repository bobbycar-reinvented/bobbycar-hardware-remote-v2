#pragma once

// local includes
#include "bobbymenudisplay.h"

class SettingsMenu : public BobbyMenuDisplay
{
    using Base = BobbyMenuDisplay;
public:
    SettingsMenu();

    std::string title() const override;

    void back() override;
};
