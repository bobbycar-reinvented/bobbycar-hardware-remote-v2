#include "settingsmenu.h"

// 3rdparty lib includes
#include <actions/popscreenaction.h>
#include <actions/pushscreenaction.h>
#include <menuitem.h>
#include <screenmanager.h>
#include <textwithvaluehelper.h>

// local includes
#include "accessors/settingsaccessors.h"
#include "bobbychangevaluedisplay.h"
#include "bobbycheckbox.h"
#include "typesafeenumchangemenu.h"

namespace {
constexpr char TEXT_DEADBAND[] = "Deadband";
constexpr char TEXT_INVERTLEFTX[] = "Invert left X";
constexpr char TEXT_INVERTLEFTY[] = "Invert left Y";
constexpr char TEXT_INVERTRIGHTX[] = "Invert right X";
constexpr char TEXT_INVERTRIGHTY[] = "Invert right Y";
constexpr char TEXT_ANALOG_STICK_MODE[] = "Analog stick mode";

using namespace espgui;

using DeadbandChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<uint16_t>,
    espgui::StaticText<TEXT_DEADBAND>,
    DeadBandAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
} // namespace

SettingsMenu::SettingsMenu()
{
    using namespace espgui;

    constructMenuItem<makeComponent<MenuItem, TextWithHighlightedValueHelper<TEXT_DEADBAND, DeadBandAccessor>, PushScreenAction<DeadbandChangeScreen>>>();

    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_INVERTLEFTX>, BobbyCheckbox, InvertLeftXAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_INVERTLEFTY>, BobbyCheckbox, InvertLeftYAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_INVERTRIGHTX>, BobbyCheckbox, InvertRightXAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_INVERTRIGHTY>, BobbyCheckbox, InvertRightYAccessor>>();

    constructMenuItem<PushScreenTypeSafeChangeMenuItem<AnalogStickMode, TEXT_ANALOG_STICK_MODE>>(&configs.analogStickMode);
    // constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_REBOOT>, StaticMenuItemIcon<&bobbyicons::reboot>, RebootAction>>();
}

std::string SettingsMenu::text() const
{
    return "Settings";
}

void SettingsMenu::back()
{
    espgui::popScreen();
}
