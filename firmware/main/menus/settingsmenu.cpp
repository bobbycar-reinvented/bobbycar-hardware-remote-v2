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
#include "icons/bluetooth.h"
#include "menus/blesettingsmenu.h"
#include "typesafeenumchangemenu.h"

namespace {
constexpr char TEXT_DEADBAND[] = "Deadband";
constexpr char TEXT_INVERTLEFTX[] = "Invert left X";
constexpr char TEXT_INVERTLEFTY[] = "Invert left Y";
constexpr char TEXT_INVERTRIGHTX[] = "Invert right X";
constexpr char TEXT_INVERTRIGHTY[] = "Invert right Y";
constexpr char TEXT_ANALOG_STICK_MODE[] = "Analog stick mode";
constexpr char TEXT_BLE_SETTINGS[] = "BLE Settings";

constexpr char TEXT_FRONT_STEER[] = "Front Steer Pwm";
constexpr char TEXT_BACK_STEER[] = "Back Steer Pwm";
constexpr char TEXT_FRONT_DRIVE[] = "Front Drive Pwm";
constexpr char TEXT_BACK_DRIVE[] = "Back Drive Pwm";


    using namespace espgui;

using DeadbandChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<uint16_t>,
    espgui::StaticText<TEXT_DEADBAND>,
    DeadBandAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

using FrontSteerChangeMenu = espgui::makeComponent<
        BobbyChangeValueDisplay<int16_t>,
        espgui::StaticText<TEXT_FRONT_STEER>,
        FrontSteerAccessor,
        espgui::ConfirmActionInterface<espgui::PopScreenAction>,
        espgui::BackActionInterface<espgui::PopScreenAction>
>;

using BackSteerChangeMenu = espgui::makeComponent<
        BobbyChangeValueDisplay<int16_t>,
        espgui::StaticText<TEXT_BACK_STEER>,
        BackSteerAccessor,
        espgui::ConfirmActionInterface<espgui::PopScreenAction>,
        espgui::BackActionInterface<espgui::PopScreenAction>
>;

using FrontDriveChangeMenu = espgui::makeComponent<
        BobbyChangeValueDisplay<int16_t>,
        espgui::StaticText<TEXT_FRONT_DRIVE>,
        FrontDriveAccessor,
        espgui::ConfirmActionInterface<espgui::PopScreenAction>,
        espgui::BackActionInterface<espgui::PopScreenAction>
>;

using BackDriveChangeMenu = espgui::makeComponent<
        BobbyChangeValueDisplay<int16_t>,
        espgui::StaticText<TEXT_BACK_DRIVE>,
        BackDriveAccessor,
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

    constructMenuItem<makeComponent<MenuItem, TextWithHighlightedValueHelper<TEXT_FRONT_STEER, FrontSteerAccessor>, PushScreenAction<FrontSteerChangeMenu>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithHighlightedValueHelper<TEXT_BACK_STEER, BackSteerAccessor>, PushScreenAction<BackSteerChangeMenu>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithHighlightedValueHelper<TEXT_FRONT_DRIVE, FrontDriveAccessor>, PushScreenAction<FrontDriveChangeMenu>>>();
    constructMenuItem<makeComponent<MenuItem, TextWithHighlightedValueHelper<TEXT_BACK_DRIVE, BackDriveAccessor>, PushScreenAction<BackDriveChangeMenu>>>();

    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BLE_SETTINGS>, PushScreenAction<BleSettingsMenu>, StaticMenuItemIcon<&bobbyicons::bluetooth>>>();
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
