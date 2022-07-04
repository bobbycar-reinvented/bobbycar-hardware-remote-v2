#pragma once

// system includes
#include <vector>

// 3rdparty lib includes
#include <NimBLEAdvertisedDevice.h>

namespace ble {
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
} // namespace ble
