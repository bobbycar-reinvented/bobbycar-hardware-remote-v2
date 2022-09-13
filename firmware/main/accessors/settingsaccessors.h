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

// BLE accessors
struct FrontSteerAccessor : public ConfigAccessor<int16_t> { ConfigWrapper<int16_t> &getConfig() const override { return configs.frontSteer; } };
struct BackSteerAccessor : public ConfigAccessor<int16_t> { ConfigWrapper<int16_t> &getConfig() const override { return configs.backSteer; } };
struct FrontDriveAccessor : public ConfigAccessor<int16_t> { ConfigWrapper<int16_t> &getConfig() const override { return configs.frontDrive; } };
struct BackDriveAccessor : public ConfigAccessor<int16_t> { ConfigWrapper<int16_t> &getConfig() const override { return configs.backDrive; } };

// Gamecontroller accessors
struct GamecontrollerBacklightAccessor : public ConfigAccessor<bool> { ConfigWrapper<bool> &getConfig() const override { return configs.gamecontrollerBacklight; } };
