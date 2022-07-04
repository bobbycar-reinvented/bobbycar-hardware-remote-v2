#include "dpad.h"

namespace buttons {
Dpad<IN_PINS.size(), OUT_PINS.size(), IN_PINS, OUT_PINS> dpad;

void init()
{
    dpad.init();
}

void update()
{
    dpad.update();
}
} // namespace buttons
