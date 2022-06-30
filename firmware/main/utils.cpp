#include "utils.h"

// 3rdparty lib includes
#include <cpputils.h>

// local includes
#include "settings.h"

namespace utils {

float mapAnalogStick(uint16_t middle, uint16_t start, uint16_t end, uint16_t raw)
{
    const auto deadband = configs.analogDeadband.value();
    if (std::abs(raw - middle) < deadband)
    {
        return 0;
    }
    else if (raw < middle)
    {
        const auto return_val = cpputils::mapValueClamped<float>(raw, start, middle - deadband, -100, 0);
        if (return_val > 0)
            return 0;
        if (return_val < -100)
            return -100;
        return return_val;
    }
    else
    {
        const auto return_val = cpputils::mapValueClamped<float>(raw, middle + deadband, end, 0, 100);
        if (return_val < 0)
            return 0;
        if (return_val > 100)
            return 100;
        return return_val;
    }
}

} // namespace utils
