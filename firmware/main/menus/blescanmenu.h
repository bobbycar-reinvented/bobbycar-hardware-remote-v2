#pragma once

// local includes
#include "bobbymenudisplay.h"

class BleScanMenu : public BobbyMenuDisplay
{
    using Base = BobbyMenuDisplay;

public:
    std::string title() const override;
    void back() override;

    void initScreen(espgui::TftInterface &tft) override;
    void update() override;

    void buttonPressed(espgui::Button button) override;
    void buttonReleased(espgui::Button button) override;
private:
    bool m_results_updated{false};
};
