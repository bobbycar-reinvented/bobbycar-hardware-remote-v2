#include "blescanmenu.h"

// 3rdparty lib includes
#include <fmt/core.h>
#include <menuitem.h>
#include <screenmanager.h>

// local includes
#include <ble.h>
#include <bobbyerrorhandler.h>
#include <icons/presets.h>

class BleScanMenuItem : public espgui::MenuItem
{
public:
    explicit BleScanMenuItem(NimBLEAdvertisedDevice& device)
        : m_device{device} {}

    void setDevice(NimBLEAdvertisedDevice& device)
        { m_device = device; }

    const espgui::MenuItemIcon *icon() const override
    {
        return &bobbyicons::presets;
    }

    void triggered() override;
    std::string text() const override;

private:
    NimBLEAdvertisedDevice& m_device;
};

void BleScanMenuItem::triggered()
{
    ESP_LOGI("MenuItem", "BleScanMenuItem::triggered() -> %s", text().c_str());
    ble::connect(m_device.getAddress());
    espgui::popScreen();
}

std::string BleScanMenuItem::text() const
{
    if (m_device.haveName())
        return m_device.getName();
    else
        return m_device.getAddress().toString();
    return "You found a bug!";
}

[[nodiscard]] std::string BleScanMenu::text() const
{
    if (ble::status() == ble::BLEScanStatus::Done)
        return "Scan complete";
    return "Scanning...";
}

void BleScanMenu::back()
{
    if (ble::status() == ble::BLEScanStatus::Scanning)
    {
        return;
    }
    espgui::popScreen();
}

void BleScanMenu::initScreen()
{
    Base::initScreen();
    Base::redraw();

    ble::startScan();
    m_results_updated = false;
}

void BleScanMenu::update()
{
    const auto status = ble::status();

    if (status == ble::BLEScanStatus::Done && !m_results_updated)
    {
        m_results_updated = true;
        for (std::size_t i = 0; i < ble::scan_results.size(); i++)
        {
            if (menuItemCount() <= i)
                constructMenuItem<BleScanMenuItem>(ble::scan_results[i]);
            else
                ((BleScanMenuItem*)(&getMenuItem(i)))->setDevice(ble::scan_results[i]);
        }

        while (menuItemCount() > ble::scan_results.size())
            takeLastMenuItem();
    }

    Base::update();
}

void BleScanMenu::buttonPressed(espgui::Button button)
{
    if (ble::status() == ble::BLEScanStatus::Scanning)
    {
        return;
    }
    Base::buttonPressed(button);
}

void BleScanMenu::buttonReleased(espgui::Button button)
{
    if (ble::status() == ble::BLEScanStatus::Scanning)
    {
        return;
    }
    Base::buttonReleased(button);
}
