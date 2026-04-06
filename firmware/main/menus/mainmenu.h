#pragma once

// local includes
#include "bobbymenudisplay.h"

class MainMenu : public BobbyMenuDisplay
{
    using Base = BobbyMenuDisplay;
public:
    MainMenu();

    std::string title() const override;

    void back() override;
};
