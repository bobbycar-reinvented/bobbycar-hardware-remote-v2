#pragma once

// 3rdparty lib includes
#include <accessorinterface.h>

// local includes
#include "accessorhelpers.h"

// analog stick accessors
struct DeadBandAccessor : public ConfigAccessor<uint16_t> { ConfigWrapper<uint16_t> &getConfig() const override { return configs.analogDeadband; } };
struct InvertLeftXAccessor : public ConfigAccessor<bool> { ConfigWrapper<bool> &getConfig() const override { return configs.invertLeftX; } };
struct InvertLeftYAccessor : public ConfigAccessor<bool> { ConfigWrapper<bool> &getConfig() const override { return configs.invertLeftY; } };
struct InvertRightXAccessor : public ConfigAccessor<bool> { ConfigWrapper<bool> &getConfig() const override { return configs.invertRightX; } };
struct InvertRightYAccessor : public ConfigAccessor<bool> { ConfigWrapper<bool> &getConfig() const override { return configs.invertRightY; } };
