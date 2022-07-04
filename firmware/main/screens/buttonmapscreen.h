#pragma once

// 3rdparty lib includes
#include <tftinstance.h>
#include <widgets/label.h>

// local includes
#include "bobbydisplaywithtitle.h"

namespace {
enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    None
};

template<uint16_t x, uint16_t y>
class DrawDpad
{
public:
    void start();
    void redraw(Direction highlight);
private:
    Direction m_last_direction{None};
};

template<uint16_t x, uint16_t y>
void DrawDpad<x,y>::start()
{
    using namespace espgui;

    tft.drawCircle(x - 35, y, 20, TFT_WHITE);
    tft.drawCircle(x + 35, y, 20, TFT_WHITE);
    tft.drawCircle(x, y - 35, 20, TFT_WHITE);
    tft.drawCircle(x, y + 35, 20, TFT_WHITE);
}

template<uint16_t x, uint16_t y>
void DrawDpad<x,y>::redraw(Direction highlight)
{
    using namespace espgui;

    tft.fillCircle(x - 35, y, 15, highlight == LEFT  ? TFT_RED : TFT_BLACK);
    tft.fillCircle(x + 35, y, 15, highlight == RIGHT ? TFT_RED : TFT_BLACK);
    tft.fillCircle(x, y - 35, 15, highlight == UP    ? TFT_RED : TFT_BLACK);
    tft.fillCircle(x, y + 35, 15, highlight == DOWN  ? TFT_RED : TFT_BLACK);
}
} // namespace

class ButtonMapScreen : public BobbyDisplayWithTitle
{
    using Base = BobbyDisplayWithTitle;
public:
    void initScreen() override;
    void update() override;
    void redraw() override;

    std::string text() const override;

    void buttonPressed(espgui::Button button) override;
    void buttonReleased(espgui::Button button) override;

    void rawButtonPressed(uint8_t button) override;
    void rawButtonReleased(uint8_t button) override;
private:
    DrawDpad<70, 240-70> m_dpad_left;
    DrawDpad<320-70, 240-70> m_dpad_right;

    espgui::Label m_label{25, 80};

    uint8_t m_current_button{0};
};
