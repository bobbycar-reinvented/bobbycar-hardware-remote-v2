#include "blesettingsmenu.h"

// system includes
#include <optional>

// 3rdparty lib includes
#include <accessorinterface.h>
#include <espchrono.h>
#include <menuitem.h>
#include <screenmanager.h>
#include <actions/pushscreenaction.h>

// local includes
#include "actions/bleactions.h"
#include "icons/scan.h"
#include "menus/blescanmenu.h"

namespace {
constexpr char TEXT_DEVICES[] = "Devices";
constexpr char TEXT_START_SCAN[] = "Start scan";
} // namespace

BleSettingsMenu::BleSettingsMenu()
{
    using namespace espgui;

    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DEVICES>, PushScreenAction<BleScanMenu>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_START_SCAN>, StartBleScanAction, StaticMenuItemIcon<&bobbyicons::scan>>>();
}

std::string BleSettingsMenu::text() const
{
    return "Ble Settings";
}

void BleSettingsMenu::back()
{
    espgui::popScreen();
}
