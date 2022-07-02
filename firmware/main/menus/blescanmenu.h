#pragma once

// local includes
#include "bobbymenudisplay.h"

class BleScanMenu : public BobbyMenuDisplay
{
    using Base = BobbyMenuDisplay;

public:
    [[nodiscard]] std::string text() const override;
    void back() override;

    void initScreen() override;
    void update() override;

    void buttonPressed(espgui::Button button) override;
    void buttonReleased(espgui::Button button) override;
private:
    bool m_results_updated{false};
};
