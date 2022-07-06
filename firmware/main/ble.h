#pragma once

// system includes
#include <vector>
#include <optional>

// 3rdparty lib includes
#include <NimBLEAdvertisedDevice.h>
#include <espchrono.h>

namespace ble {

extern std::optional<espchrono::millis_clock::time_point> lastUpdate;
extern std::optional<espchrono::millis_clock::time_point> lastScan;

enum class BLEScanStatus : uint8_t {
    Idle,
    Scanning,
    Done,
};

extern std::vector<NimBLEAdvertisedDevice> scan_results;

void init();
void update();

BLEScanStatus status();
void startScan();
void stopScan();

bool isConnected();
void disableAutoConnect();

void clearResults();
size_t bobbycars_found();

void connect(NimBLEAddress uuid);
void disconnect();
} // namespace ble
