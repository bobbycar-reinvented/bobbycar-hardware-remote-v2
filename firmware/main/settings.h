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

    template<typename T>
    void callForEveryConfig(T &&callable)
    {
#define REGISTER_CONFIG(name) \
        if (callable(name)) return;

        REGISTER_CONFIG(displayRotated)
    }
};
