#pragma once

// 3rdparty lib includes
#include <widgets/label.h>

// local includes
#include "bobbydisplay.h"

class CalibrateAnalogStickScreen : public BobbyDisplay
{
    static constexpr const auto label_start = 70;

    using Base = BobbyDisplay;
public:
    explicit CalibrateAnalogStickScreen(bool from_boot = false);

    void initScreen() override;
    void redraw() override;
    void update() override;

    void buttonPressed(espgui::Button button) override;
private:

    espgui::Label m_label_leftx_stick {10, label_start + 10};
    espgui::Label m_label_lefty_stick {10, label_start + 35};
    espgui::Label m_label_rightx_stick{10, label_start + 60};
    espgui::Label m_label_righty_stick{10, label_start + 85};
    espgui::Label m_label_status      {10, label_start + 110};

    uint16_t m_left_x_min;
    uint16_t m_left_x_max;
    uint16_t m_left_x_mid;
    uint16_t m_left_y_min;
    uint16_t m_left_y_max;
    uint16_t m_left_y_mid;

    uint16_t m_right_x_min;
    uint16_t m_right_x_max;
    uint16_t m_right_x_mid;
    uint16_t m_right_y_min;
    uint16_t m_right_y_max;
    uint16_t m_right_y_mid;

    bool is_done() const;

    const bool m_from_boot;
};
