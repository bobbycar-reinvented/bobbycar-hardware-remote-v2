#include "screens.h"

// 3rdparty lib includes
#include <tftinstance.h>
#include <screenmanager.h>

// local includes
#include "icons/logo.h"
#include "settings.h"

using namespace espgui;
using namespace std::chrono_literals;

espgui::Label bootLabel{32, bobbyicons::logo.HEIGHT + 75};

namespace display {

espchrono::millis_clock::time_point lastDisplayUpdate;
espchrono::millis_clock::time_point lastDisplayRedraw;

void init()
{
    tft.init();
    tft.setRotation(configs.displayRotated.value() ? 1 : 3);
    tft.fillScreen(TFT_WHITE);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.setTextFont(4);
    tft.setSwapBytes(true);
    tft.pushImage((320/2)-(bobbyicons::logo.WIDTH/2), 10, bobbyicons::logo.WIDTH, bobbyicons::logo.HEIGHT, bobbyicons::logo.buffer);
    tft.setSwapBytes(false);
    tft.drawString("Remote-OS", 32,  bobbyicons::logo.HEIGHT + 10 +  7);
    tft.drawString("booting...", 32, bobbyicons::logo.HEIGHT + 10 + 32);
    bootLabel.start();
}

void update()
{
    if (tft.getRotation() != (configs.displayRotated.value() ? 1 : 3))
    {
        tft.setRotation(configs.displayRotated.value() ? 1 : 3);

        if (espgui::currentDisplay)
            espgui::currentDisplay->initScreen();
    }

    if (espchrono::ago(lastDisplayUpdate) >= 1000ms/50)
    {
        lastDisplayUpdate = espchrono::millis_clock::now();

        if (espgui::currentDisplay)
            espgui::currentDisplay->update();

        if (espgui::changeScreenCallback)
        {
            espgui::changeScreenCallback();
            espgui::changeScreenCallback = {};
        }
    }

    if (espchrono::ago(lastDisplayRedraw) >= 1000ms/60)
    {
        lastDisplayRedraw = espchrono::millis_clock::now();

        if (espgui::currentDisplay)
            espgui::currentDisplay->redraw();
    }
}
} // namespace display
