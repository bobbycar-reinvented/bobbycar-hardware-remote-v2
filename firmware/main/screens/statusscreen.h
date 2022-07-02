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
    void update() override;
    void redraw() override;

    void buttonPressed(espgui::Button button) override;
private:
    uint16_t m_left_x{};
    uint16_t m_left_y{};
    uint16_t m_right_x{};
    uint16_t m_right_y{};

    bool m_left_pressed{};
    bool m_right_pressed{};

    uint16_t m_last_left_x{};
    uint16_t m_last_left_y{};
    uint16_t m_last_right_x{};
    uint16_t m_last_right_y{};

    bool m_last_left_pressed{};
    bool m_last_right_pressed{};

    espgui::Label m_left_label{15, 15};
    espgui::Label m_right_label{15, 40};
    espgui::Label m_stats_label{15, 65};
};
