#pragma once

// local includes
#include "bobbytypesafeenum.h"

#define ANALOG_STICK_VALUES(x) \
    x(LEFT_ONLY) \
    x(RIGHT_ONLY) \
    x(BOTH)

DECLARE_BOBBYTYPESAFE_ENUM(AnalogStickMode, : uint8_t, ANALOG_STICK_VALUES)
