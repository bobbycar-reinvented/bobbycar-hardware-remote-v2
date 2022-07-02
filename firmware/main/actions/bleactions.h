#pragma once

// 3rdparty lib includes
#include <actioninterface.h>

class StartBleScanAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override;
};
