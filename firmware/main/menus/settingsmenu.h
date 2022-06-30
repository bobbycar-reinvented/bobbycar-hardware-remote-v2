#pragma once

// local includes
#include "bobbymenudisplay.h"

class SettingsMenu : public BobbyMenuDisplay
{
    using Base = BobbyMenuDisplay;
public:
    SettingsMenu();

    [[nodiscard]] std::string text() const override;

    void back() override;
};
