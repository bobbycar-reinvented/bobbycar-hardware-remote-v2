#pragma once

// 3rdparty lib includes
#include <actions/toggleboolaction.h>
#include <checkboxicon.h>

// local includes
#include "bobbyerrorhandler.h"

class BobbyCheckbox :
        public virtual espgui::ToggleBoolAction,
        public virtual espgui::CheckboxIcon,
        public virtual BobbyErrorHandler
{
};
