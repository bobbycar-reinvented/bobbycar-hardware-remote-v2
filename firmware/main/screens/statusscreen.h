#pragma once

// 3rdparty lib includes
#include <widgets/label.h>

// local includes
#include "bobbydisplay.h"

class StatusScreen : public BobbyDisplay
{
    using Base = BobbyDisplay;
public:
    void initScreen() override;
    void redraw() override;

    void buttonPressed(espgui::Button button) override;
private:
    espgui::Label m_label_left_stick{10, 10};
    espgui::Label m_label_right_stick{10, 35};
};
