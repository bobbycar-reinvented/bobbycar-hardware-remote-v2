#pragma once

// 3rdparty lib includes
#include <actioninterface.h>

class BootIntoGamecontrollerAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override;
};
