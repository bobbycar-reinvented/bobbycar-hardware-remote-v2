#pragma once

// 3rdparty lib includes
#include <errorhandlerinterface.h>

struct BobbyErrorHandler : public virtual espgui::ErrorHandlerInterface
{
    void errorOccurred(std::string &&error) override;
};
