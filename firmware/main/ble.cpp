constexpr const char * const TAG = "BOBBY_BLE";
#include "ble.h"

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <ArduinoJson.h>
#include <NimBLEAdvertisedDevice.h>
#include <NimBLEDevice.h>
#include <NimBLEUUID.h>
#include <espchrono.h>

// local includes
#include "analog_sticks.h"
#include "globals.h"
#include "settings.h"
#include "utils.h"

using namespace std::chrono_literals;

namespace ble {

namespace {
BLEScanStatus _status;
bool _isConnected;
bool _triedAutoConnect;
bool _stats_service_discovered;
bool _remote_service_discovered;

espchrono::millis_clock::time_point lastSent;

NimBLEClient* pClient;
NimBLERemoteCharacteristic* pLivestatsCharacteristic;
NimBLERemoteCharacteristic* pRemoteCharacteristic;

const NimBLEUUID bobbycar_service    = NimBLEUUID{"0335e46c-f355-4ce6-8076-017de08cee98"};
const NimBLEUUID pLivestats_uuid     = NimBLEUUID{"a48321ea-329f-4eab-a401-30e247211524"};
const NimBLEUUID pRemotecontrol_uuid = NimBLEUUID{"4201def0-a264-43e6-946b-6b2d9612dfed"};

void livestatsCallback(NimBLERemoteCharacteristic *pRemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify)
{
    if (!pLivestatsCharacteristic->canRead())
    {
        ESP_LOGE(TAG, "Cannot read livestats characteristic");
        return;
    }

    const std::string res = pLivestatsCharacteristic->getValue();
    StaticJsonDocument<384> doc;
    if (const auto err = deserializeJson(doc, res); err)
    {
        ESP_LOGE(TAG, "error serializing");
        return;
    }

    bobbyStats.front.voltage = doc["v"][0];
    bobbyStats.back.voltage  = doc["v"][1];

    bobbyStats.front.temperature = doc["t"][0];
    bobbyStats.back.temperature  = doc["t"][1];

    bobbyStats.front.left.error  = doc["e"][0];
    bobbyStats.front.right.error = doc["e"][1];
    bobbyStats.back.left.error   = doc["e"][2];
    bobbyStats.back.right.error  = doc["e"][3];

    bobbyStats.front.left.speed  = doc["s"][0];
    bobbyStats.front.right.speed = doc["s"][1];
    bobbyStats.back.left.speed   = doc["s"][2];
    bobbyStats.back.right.speed  = doc["s"][3];

    bobbyStats.front.left.ampere  = doc["a"][0];
    bobbyStats.front.right.ampere = doc["a"][1];
    bobbyStats.back.left.ampere   = doc["a"][2];
    bobbyStats.back.right.ampere  = doc["a"][3];

    bobbyStats.avgVoltage = (bobbyStats.front.voltage + bobbyStats.back.voltage) / 2.f;
    bobbyStats.sumCurrent = \
            bobbyStats.front.left.ampere + \
            bobbyStats.front.right.ampere + \
            bobbyStats.back.left.ampere + \
            bobbyStats.back.right.ampere;

    bobbyStats.sumPower = bobbyStats.avgVoltage * bobbyStats.sumCurrent;

    bobbyStats.avgSpeed = (\
            bobbyStats.front.left.speed + \
            bobbyStats.front.right.speed + \
            bobbyStats.back.left.speed + \
            bobbyStats.back.right.speed) / 4;

    bobbyStats.valid = true;
}

void send_inputs()
{
    std::string output;
    const auto pwms = utils::calculatePwm();
    StaticJsonDocument<128> doc;

    bool leftPressed  =  analog_sticks::left_stick.btn_pressed ?  analog_sticks::left_stick.btn_pressed.value() : false;
    bool rightPressed = analog_sticks::right_stick.btn_pressed ? analog_sticks::right_stick.btn_pressed.value() : false;

    uint8_t blink{0};

    if (leftPressed && !rightPressed)
        blink = 1;
    else if (leftPressed && rightPressed)
        blink = 3;
    else if (!leftPressed && rightPressed)
        blink = 2;

    doc["fl"] = pwms.frontLeft;
    doc["fr"] = pwms.frontRight;
    doc["bl"] = pwms.backLeft;
    doc["br"] = pwms.backRight;
    doc["anim"] = blink;

    serializeJson(doc, output);
    // ESP_LOGI(TAG, "json: %s", output.c_str());
    pRemoteCharacteristic->writeValue(output);
}
} // namespace

std::vector<NimBLEAdvertisedDevice> scan_results;

bool isConnected()
{
    return _isConnected;
}

void init()
{
    _status = BLEScanStatus::Idle;
    _isConnected = false;
    _triedAutoConnect = false;
    _stats_service_discovered = false;
    _remote_service_discovered = false;
}

void update()
{
    // handle stuff like scan on boot, then auto connect to saved mac address
    if (!isConnected() && _status == BLEScanStatus::Idle && espchrono::millis_clock::now().time_since_epoch() > 2s)
    {
        startScan();
    }
    else if (!isConnected() && _status == BLEScanStatus::Done && !_triedAutoConnect)
    {
        _triedAutoConnect = true;
        const auto saved = configs.reconnectAddress.value();
        if (saved.empty())
        {
            ESP_LOGW(TAG, "No saved BLE address, not auto connecting");
            return;
        }
        ESP_LOGI(TAG, "Auto connecting to saved BLE address");

        bool contains = false;

        for (size_t i = 0; i < scan_results.size(); i++)
        {
            if (scan_results[i].getAddress() == saved)
            {
                contains = true;
            }
        }

        if (!contains)
            return;

        NimBLEAddress uuid{saved};
        connect(uuid);
    }
    else if (isConnected() && !_stats_service_discovered && pClient)
    {
        if (auto *pService = pClient->getService(bobbycar_service); pService)
        {
            pLivestatsCharacteristic = pService->getCharacteristic(pLivestats_uuid);
            pRemoteCharacteristic = pService->getCharacteristic(pRemotecontrol_uuid);
            if (pLivestatsCharacteristic)
            {
                pLivestatsCharacteristic->subscribe(true, livestatsCallback);
                _stats_service_discovered = true;
                _remote_service_discovered = (pRemoteCharacteristic != nullptr);
            }
            else
            {
                ESP_LOGE(TAG, "Error getting livestat characteristic");
            }
        }
        else
            ESP_LOGE(TAG, "This is not a bobbycar!");
    }

    if (isConnected() && _remote_service_discovered && pRemoteCharacteristic)
    {
        if (!pRemoteCharacteristic->canWrite())
        {
            ESP_LOGI(TAG, "Cant write");
            return;
        }

        if (espchrono::ago(lastSent) > 30ms)
        {
            lastSent = espchrono::millis_clock::now();
            send_inputs();
        }
    }
}

BLEScanStatus status()
{
    return _status;
}

class BLEScanCallback : public NimBLEAdvertisedDeviceCallbacks
{
public:
    void onResult(NimBLEAdvertisedDevice *advertisedDevice) override
    {
        if (advertisedDevice->haveServiceUUID())
        {
            if (advertisedDevice->isAdvertisingService(bobbycar_service))
            {
                scan_results.push_back(*advertisedDevice);
            }
        }
    }
};

void startScan()
{
    if (_status == BLEScanStatus::Scanning)
    {
        ESP_LOGW(TAG, "Already scanning");
        return;
    }

    clearResults();

    ESP_LOGI(TAG, "Starting scan...");

    NimBLEDevice::init(configs.hostname.value());

    auto *pScan = NimBLEDevice::getScan();
    pScan->setAdvertisedDeviceCallbacks(new BLEScanCallback());
    pScan->setActiveScan(true);
    pScan->setInterval(100);
    pScan->setWindow(50);

    _status = BLEScanStatus::Scanning;

    pScan->start(5);
    pScan->clearResults();

    ESP_LOGI(TAG, "Scanning done, found %u", scan_results.size());

    _status = BLEScanStatus::Done;
}

void stopScan()
{
    if (_status != BLEScanStatus::Scanning)
    {
        ESP_LOGW(TAG, "Not scanning");
        return;
    }

    auto *pScan = NimBLEDevice::getScan();
    pScan->stop();
    pScan->clearResults();
    _status = BLEScanStatus::Idle;
}

void clearResults()
{
    scan_results.clear();
}

size_t bobbycars_found()
{
    return scan_results.size();
}

void connect(NimBLEAddress uuid)
{
    if (_isConnected)
    {
        ESP_LOGW(TAG, "Already connected");
        return;
    }

    ESP_LOGI(TAG, "Connecting to %s", uuid.toString().c_str());

    pClient = NimBLEDevice::createClient();
    if (pClient->connect(uuid))
    {
        _isConnected = true;
        // discover statistic characteristic
        ESP_LOGI(TAG, "Connected!");
    }
    else
    {
        ESP_LOGE(TAG, "Failed to connect to %s", uuid.toString().c_str());
    }
}
} // namespace ble
