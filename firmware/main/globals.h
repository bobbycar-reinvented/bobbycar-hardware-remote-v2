#pragma once

// system includes
#include <cstdint>
#include <optional>

struct Motor {
    float speed;
    float ampere;
    uint8_t error;
};

struct Board {
    float voltage;
    float temperature;

    Motor left;
    Motor right;
};

struct BobbyStats {
    Board front;
    Board back;

    float avgVoltage;
    float avgSpeed;
    float sumCurrent;
    float sumPower;

    bool valid;
};

extern BobbyStats bobbyStats;
