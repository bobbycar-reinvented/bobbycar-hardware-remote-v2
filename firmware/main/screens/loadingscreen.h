#pragma once

// 3rdparty lib includes
#include <display.h>
#include <screenmanager.h>
#include <tftinstance.h>

template <typename TNextMenu, typename TBoolAccessor, const char * Text>
class LoadingScreen : public espgui::Display
{
public:
    LoadingScreen() :
        m_text{Text}
    {}

    std::string text() const;

    void initScreen() override;
    void update() override;
    // void redraw() override;

    void rawButtonPressed(uint8_t button) override {}
    void rawButtonReleased(uint8_t button) override {}
    void buttonPressed(espgui::Button button) override {};
    void buttonReleased(espgui::Button button) override {};
private:
    const std::string_view m_text;
};

template <typename TNextMenu, typename TBoolAccessor, const char * Text>
std::string LoadingScreen<TNextMenu, TBoolAccessor, Text>::text() const
{
    return std::string{m_text};
}

template <typename TNextMenu, typename TBoolAccessor, const char * Text>
void LoadingScreen<TNextMenu, TBoolAccessor, Text>::initScreen()
{
    using namespace espgui;

    const auto text_x = tft.width() / 2;
    const auto text_y = tft.height() / 2;
    const auto text_height = tft.fontHeight();

    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);

    tft.setTextFont(4);

    tft.drawCentreString(m_text, text_x, text_y-text_height, 4);

    tft.drawCircle(text_x, text_y + 10 + text_height, 20, TFT_WHITE);
}

template <typename TNextMenu, typename TBoolAccessor, const char * Text>
void LoadingScreen<TNextMenu, TBoolAccessor, Text>::update()
{
    if (TBoolAccessor{}.getValue())
    {
        espgui::switchScreen<TNextMenu>();
    }
}
