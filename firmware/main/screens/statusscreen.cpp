#include "statusscreen.h"

// 3rdparty lib includes
#include <fmt/core.h>
#include <screenmanager.h>
#include <tftinstance.h>

// local lib includes
#include "analog_sticks.h"
#include "menus/mainmenu.h"

void StatusScreen::initScreen()
{
    Base::initScreen();

    m_label_left_stick.start();
    m_label_left_stick.start();
}

void StatusScreen::redraw()
{
    using namespace analog_sticks;
    Base::redraw();

    espgui::tft.setTextColor(TFT_WHITE, TFT_BLACK);

    if (left_stick.x && left_stick.y && left_stick.btn_pressed)
        m_label_left_stick.redraw(fmt::format("l x:{}, y:{}, btn:{}", *(left_stick.x), *(left_stick.y), *(left_stick.btn_pressed)));
    else
        m_label_left_stick.redraw("No value for left stick");

    if (right_stick.x && right_stick.y && right_stick.btn_pressed)
        m_label_right_stick.redraw(fmt::format("r raw_x:{}, raw_y:{}, btn:{}", *(right_stick.x), *(right_stick.y), *(right_stick.btn_pressed)));
    else
        m_label_right_stick.redraw("No value for right stick");
}

void StatusScreen::buttonPressed(espgui::Button button)
{
    Base::buttonPressed(button);

    switch (button)
    {
    case espgui::Button::Left:
    case espgui::Button::Right:
        espgui::pushScreen<MainMenu>();
        break;
    default:;
    }
}
