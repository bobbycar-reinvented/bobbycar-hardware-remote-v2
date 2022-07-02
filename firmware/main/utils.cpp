#include "utils.h"

// 3rdparty lib includes
#include <cpputils.h>

// local includes
#include "analog_sticks.h"
#include "settings.h"

#include "esp_log.h"

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

float x_coord()
{
    // ToDo: take config into account
    return analog_sticks::left_stick.x ? analog_sticks::left_stick.x.value() : 0;
}

float y_coord()
{
    return analog_sticks::left_stick.y ? analog_sticks::left_stick.y.value() : 0;
}

MotorPwms calculatePwm()
{
    const auto x = x_coord() / 100.f;
    const auto y = y_coord() / 100.f;

    const auto frontSteer = configs.frontSteer.value();
    const auto backSteer = configs.backSteer.value();
    const auto frontDrive = configs.frontDrive.value();
    const auto backDrive = configs.backDrive.value();

    MotorPwms pwms;

    pwms.frontLeft = (x * frontSteer + y * frontDrive);
    pwms.frontRight = (-x * frontSteer + y * frontDrive);
    pwms.backLeft = (x * backSteer + y * backDrive);
    pwms.backRight = (-x * backSteer + y * backDrive);

    return pwms;
}
} // namespace utils
