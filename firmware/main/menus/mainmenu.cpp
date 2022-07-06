#include "mainmenu.h"

// 3rdparty lib includes
#include <actions/pushscreenaction.h>
#include <menuitem.h>
#include <screenmanager.h>
#include <tftinstance.h>

// local includes
#include "icons/reboot.h"
#include "icons/settings.h"
#include "menus/settingsmenu.h"

namespace {
constexpr char TEXT_SETTINGS[] = "Settings";
constexpr char TEXT_REBOOT[] = "Reboot";

class RebootAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override;
};

void RebootAction::triggered()
{
    espgui::tft.fillScreen(TFT_BLACK);
    espgui::tft.setTextColor(TFT_YELLOW);

    espgui::tft.drawString("Reboot", 5, 5, 4);

    espgui::tft.fillRect(0, 34, espgui::tft.width(), 3, TFT_WHITE);

    espgui::tft.setTextColor(TFT_WHITE);
    espgui::tft.drawString("Rebooting now...", 0, 50, 4);

    esp_restart();
}
} // namespace

MainMenu::MainMenu()
{
    using namespace espgui;

    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETTINGS>, StaticMenuItemIcon<&bobbyicons::settings>, PushScreenAction<SettingsMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_REBOOT>, StaticMenuItemIcon<&bobbyicons::reboot>, RebootAction>>();
}

std::string MainMenu::text() const
{
    return "Main Menu";
}

void MainMenu::back()
{
    espgui::popScreen();
}
