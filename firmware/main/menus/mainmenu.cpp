#include "mainmenu.h"

// 3rdparty lib includes
#include <actions/pushscreenaction.h>
#include <fontrenderer.h>
#include <menuitem.h>
#include <screenmanager.h>

// local includes
#include "actions/gamecontrolleraction.h"
#include "icons/reboot.h"
#include "icons/reboot_grey.h"
#include "icons/settings.h"
#include "icons/settings_grey.h"
#include "menus/settingsmenu.h"

namespace {
constexpr char TEXT_SETTINGS[] = "Settings";
constexpr char TEXT_REBOOT[] = "Reboot";
constexpr char TEXT_GAMECONTROLLER[] = "Gamecontroller";

class RebootAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override;
};

void RebootAction::triggered()
{
    espgui::changeScreenCallback = [](espgui::TftInterface &tft) {
        espgui::FontRenderer renderer{tft};

        renderer.drawString("Reboot", 5, 5, espgui::TFT_YELLOW, espgui::TFT_BLACK, 4);

        tft.fillRect(0, 34, tft.width(), 3, espgui::TFT_WHITE);

        renderer.drawString("Rebooting now...", 0, 50, espgui::TFT_WHITE, espgui::TFT_BLACK, 4);

        esp_restart();
    };
}
} // namespace

MainMenu::MainMenu()
{
    using namespace espgui;

    // constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SETTINGS>, StaticMenuItemIcon<&bobbyicons::settings, &bobbyicons::settings_grey>, PushScreenAction<SettingsMenu>>>();
    // constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_GAMECONTROLLER>, BootIntoGamecontrollerAction>>();
    // constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_REBOOT>, StaticMenuItemIcon<&bobbyicons::reboot, &bobbyicons::reboot_grey>, RebootAction>>();
}

std::string MainMenu::title() const
{
    return "Main Menu";
}

void MainMenu::back()
{
    espgui::popScreen();
}
