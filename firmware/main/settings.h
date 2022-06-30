#pragma once

// system includes
#include <array>
#include <optional>
#include <string>

// 3rdparty lib includes
#include <configconstraints_base.h>
#include <configconstraints_espchrono.h>
#include <configmanager.h>
#include <configwrapper.h>
#include <espchrono.h>
#include <espwifiutils.h>
#include <fmt/core.h>
#include <makearray.h>

// local includes
#include "enums.h"

constexpr const auto INPUT_MAPPING_NONE = std::numeric_limits<uint8_t>::max();

using namespace espconfig;

class ConfigContainer;

extern ConfigManager<ConfigContainer> configs;

class ConfigContainer
{
public:
    struct : ConfigWrapper<bool>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "displayRot"; }
        bool defaultValue() const override final { return false; }
        ConfigConstraintReturnType checkValue(value_t value) const override final { return {}; }
    } displayRotated;

    // dpad mapping
    struct : ConfigWrapper<uint8_t>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "dpadLeft"; }
        uint8_t defaultValue() const override final { return INPUT_MAPPING_NONE; }
        ConfigConstraintReturnType checkValue(value_t value) const override final { return {}; }
    } dpadLeft;
    struct : ConfigWrapper<uint8_t>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "dpadRight"; }
        uint8_t defaultValue() const override final { return INPUT_MAPPING_NONE; }
        ConfigConstraintReturnType checkValue(value_t value) const override final { return {}; }
    } dpadRight;
    struct : ConfigWrapper<uint8_t>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "dpadUp"; }
        uint8_t defaultValue() const override final { return INPUT_MAPPING_NONE; }
        ConfigConstraintReturnType checkValue(value_t value) const override final { return {}; }
    } dpadUp;
    struct : ConfigWrapper<uint8_t>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "dpadDown"; }
        uint8_t defaultValue() const override final { return INPUT_MAPPING_NONE; }
        ConfigConstraintReturnType checkValue(value_t value) const override final { return {}; }
    } dpadDown;
    struct : ConfigWrapper<uint8_t>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "dpadLeft2"; }
        uint8_t defaultValue() const override final { return INPUT_MAPPING_NONE; }
        ConfigConstraintReturnType checkValue(value_t value) const override final { return {}; }
    } dpadLeft2;
    struct : ConfigWrapper<uint8_t>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "dpadRight2"; }
        uint8_t defaultValue() const override final { return INPUT_MAPPING_NONE; }
        ConfigConstraintReturnType checkValue(value_t value) const override final { return {}; }
    } dpadRight2;
    struct : ConfigWrapper<uint8_t>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "dpadUp2"; }
        uint8_t defaultValue() const override final { return INPUT_MAPPING_NONE; }
        ConfigConstraintReturnType checkValue(value_t value) const override final { return {}; }
    } dpadUp2;
    struct : ConfigWrapper<uint8_t>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "dpadDown2"; }
        uint8_t defaultValue() const override final { return INPUT_MAPPING_NONE; }
        ConfigConstraintReturnType checkValue(value_t value) const override final { return {}; }
    } dpadDown2;

    // analog sticks
    struct : ConfigWrapper<uint8_t>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "analogSamples"; }
        uint8_t defaultValue() const override final { return 15; }
        ConfigConstraintReturnType checkValue(value_t value) const override final { return {}; }
    } sampleCount;

    struct : ConfigWrapper<std::optional<uint16_t>>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "leftXStartCal"; }
        std::optional<uint16_t> defaultValue() const override final { return std::nullopt; }
        ConfigConstraintReturnType checkValue(value_t value) const override final
        {
            if (!value)
                return {};
            return MinMaxOrZeroValue<uint16_t, 0, 4095>(*value);
        }
    } leftStickXStartCal;
    struct : ConfigWrapper<std::optional<uint16_t>>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "leftXEndCal"; }
        std::optional<uint16_t> defaultValue() const override final { return std::nullopt; }
        ConfigConstraintReturnType checkValue(value_t value) const override final
        {
            if (!value)
                return {};
            return MinMaxOrZeroValue<uint16_t, 0, 4095>(*value);
        }
    } leftStickXEndCal;
    struct : ConfigWrapper<std::optional<uint16_t>>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "leftXMiddleCal"; }
        std::optional<uint16_t> defaultValue() const override final { return std::nullopt; }
        ConfigConstraintReturnType checkValue(value_t value) const override final
        {
            if (!value)
                return {};
            return MinMaxOrZeroValue<uint16_t, 0, 4095>(*value);
        }
    } leftStickXMiddleCal;
    struct : ConfigWrapper<std::optional<uint16_t>>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "leftYStartCal"; }
        std::optional<uint16_t> defaultValue() const override final { return std::nullopt; }
        ConfigConstraintReturnType checkValue(value_t value) const override final
        {
            if (!value)
                return {};
            return MinMaxOrZeroValue<uint16_t, 0, 4095>(*value);
        }
    } leftStickYStartCal;
    struct : ConfigWrapper<std::optional<uint16_t>>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "leftYEndCal"; }
        std::optional<uint16_t> defaultValue() const override final { return std::nullopt; }
        ConfigConstraintReturnType checkValue(value_t value) const override final
        {
            if (!value)
                return {};
            return MinMaxOrZeroValue<uint16_t, 0, 4095>(*value);
        }
    } leftStickYEndCal;
    struct : ConfigWrapper<std::optional<uint16_t>>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "leftYMiddleCal"; }
        std::optional<uint16_t> defaultValue() const override final { return std::nullopt; }
        ConfigConstraintReturnType checkValue(value_t value) const override final
        {
            if (!value)
                return {};
            return MinMaxOrZeroValue<uint16_t, 0, 4095>(*value);
        }
    } leftStickYMiddleCal;

    struct : ConfigWrapper<std::optional<uint16_t>>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "rightXStartCal"; }
        std::optional<uint16_t> defaultValue() const override final { return std::nullopt; }
        ConfigConstraintReturnType checkValue(value_t value) const override final
        {
            if (!value)
                return {};
            return MinMaxOrZeroValue<uint16_t, 0, 4095>(*value);
        }
    } rightStickXStartCal;
    struct : ConfigWrapper<std::optional<uint16_t>>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "rightXEndCal"; }
        std::optional<uint16_t> defaultValue() const override final { return std::nullopt; }
        ConfigConstraintReturnType checkValue(value_t value) const override final
        {
            if (!value)
                return {};
            return MinMaxOrZeroValue<uint16_t, 0, 4095>(*value);
        }
    } rightStickXEndCal;
    struct : ConfigWrapper<std::optional<uint16_t>>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "rightXMiddleCal"; }
        std::optional<uint16_t> defaultValue() const override final { return std::nullopt; }
        ConfigConstraintReturnType checkValue(value_t value) const override final
        {
            if (!value)
                return {};
            return MinMaxOrZeroValue<uint16_t, 0, 4095>(*value);
        }
    } rightStickXMiddleCal;
    struct : ConfigWrapper<std::optional<uint16_t>>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "rightYStartCal"; }
        std::optional<uint16_t> defaultValue() const override final { return std::nullopt; }
        ConfigConstraintReturnType checkValue(value_t value) const override final
        {
            if (!value)
                return {};
            return MinMaxOrZeroValue<uint16_t, 0, 4095>(*value);
        }
    } rightStickYStartCal;
    struct : ConfigWrapper<std::optional<uint16_t>>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "rightYEndCal"; }
        std::optional<uint16_t> defaultValue() const override final { return std::nullopt; }
        ConfigConstraintReturnType checkValue(value_t value) const override final
        {
            if (!value)
                return {};
            return MinMaxOrZeroValue<uint16_t, 0, 4095>(*value);
        }
    } rightStickYEndCal;
    struct : ConfigWrapper<std::optional<uint16_t>>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "rightYMiddleCal"; }
        std::optional<uint16_t> defaultValue() const override final { return std::nullopt; }
        ConfigConstraintReturnType checkValue(value_t value) const override final
        {
            if (!value)
                return {};
            return MinMaxOrZeroValue<uint16_t, 0, 4095>(*value);
        }
    } rightStickYMiddleCal;

    struct : ConfigWrapper<uint16_t>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "deadband"; }
        uint16_t defaultValue() const override final { return 200; }
        ConfigConstraintReturnType checkValue(value_t value) const override final
        {
            return MinMaxValue<uint16_t, 0, 4095>(value);
        }
    } analogDeadband;

    struct : ConfigWrapper<bool>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "invertLeftX"; }
        bool defaultValue() const override final { return false; }
        ConfigConstraintReturnType checkValue(value_t value) const override final { return {}; }
    } invertLeftX;
    struct : ConfigWrapper<bool>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "invertLeftY"; }
        bool defaultValue() const override final { return false; }
        ConfigConstraintReturnType checkValue(value_t value) const override final { return {}; }
    } invertLeftY;
    struct : ConfigWrapper<bool>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "invertRightX"; }
        bool defaultValue() const override final { return false; }
        ConfigConstraintReturnType checkValue(value_t value) const override final { return {}; }
    } invertRightX;
    struct : ConfigWrapper<bool>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "invertRightY"; }
        bool defaultValue() const override final { return false; }
        ConfigConstraintReturnType checkValue(value_t value) const override final { return {}; }
    } invertRightY;

    struct : ConfigWrapper<AnalogStickMode>
    {
        bool allowReset() const override final { return true; }
        const char *nvsName() const override final { return "analogMode"; }
        AnalogStickMode defaultValue() const override final { return AnalogStickMode::LEFT_ONLY; }
        ConfigConstraintReturnType checkValue(value_t value) const override final { return {}; }
    } analogStickMode;

    template<typename T>
    void callForEveryConfig(T &&callable)
    {
#define REGISTER_CONFIG(name) \
        if (callable(name)) return;

        REGISTER_CONFIG(displayRotated)

        // dpad mapping
        REGISTER_CONFIG(dpadLeft)
        REGISTER_CONFIG(dpadRight)
        REGISTER_CONFIG(dpadUp)
        REGISTER_CONFIG(dpadDown)
        REGISTER_CONFIG(dpadLeft2)
        REGISTER_CONFIG(dpadRight2)
        REGISTER_CONFIG(dpadUp2)
        REGISTER_CONFIG(dpadDown2)

        // analog sticks
        REGISTER_CONFIG(sampleCount)
        REGISTER_CONFIG(leftStickXStartCal)
        REGISTER_CONFIG(leftStickXMiddleCal)
        REGISTER_CONFIG(leftStickXEndCal)
        REGISTER_CONFIG(leftStickYStartCal)
        REGISTER_CONFIG(leftStickYMiddleCal)
        REGISTER_CONFIG(leftStickYEndCal)
        REGISTER_CONFIG(rightStickXStartCal)
        REGISTER_CONFIG(rightStickXMiddleCal)
        REGISTER_CONFIG(rightStickXEndCal)
        REGISTER_CONFIG(rightStickYStartCal)
        REGISTER_CONFIG(rightStickYMiddleCal)
        REGISTER_CONFIG(rightStickYEndCal)

        // analog deadband
        REGISTER_CONFIG(analogDeadband)

        // invert
        REGISTER_CONFIG(invertLeftX)
        REGISTER_CONFIG(invertLeftY)
        REGISTER_CONFIG(invertRightX)
        REGISTER_CONFIG(invertRightY)

        // analog stick mode
        REGISTER_CONFIG(analogStickMode)
    }
};
