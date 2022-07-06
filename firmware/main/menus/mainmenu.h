#pragma once

// local includes
#include "bobbymenudisplay.h"

class MainMenu : public BobbyMenuDisplay
{
    using Base = BobbyMenuDisplay;
public:
    MainMenu();

    [[nodiscard]] std::string text() const override;

    void back() override;
};
